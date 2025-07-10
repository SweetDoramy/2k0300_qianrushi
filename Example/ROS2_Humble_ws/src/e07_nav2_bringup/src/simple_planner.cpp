#include "e07_nav2_bringup/simple_planner.hpp"
#include "tf2/utils.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "tf2_ros/transform_listener.h"  // 包含 TF 监听器头文件

using namespace std::chrono_literals;

namespace e07_nav2_bringup {

void SimplePlanner::configure(
  const rclcpp_lifecycle::LifecycleNode::WeakPtr &parent,
  std::string name,
  std::shared_ptr<tf2_ros::Buffer> tf,
  std::shared_ptr<nav2_costmap_2d::Costmap2DROS> costmap_ros)
{
  // 消除未使用参数警告
  (void)costmap_ros;
  
  node_ = parent.lock();  // 获取节点句柄
  name_ = name;           // 记录规划器名称
  tf_buffer_ = tf;        // 保存TF缓冲区
  
  // 创建 TF 监听器
  tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

  // 声明参数（带默认值）
  node_->declare_parameter(name + ".lookahead_dist", 0.3);
  node_->declare_parameter(name + ".max_speed", 0.5);
  node_->declare_parameter(name + ".kp", 0.8);
  node_->declare_parameter(name + ".goal_tolerance", 0.1);

  // 获取参数值
  lookahead_dist_ = node_->get_parameter(name + ".lookahead_dist").as_double();
  max_speed_ = node_->get_parameter(name + ".max_speed").as_double();
  kp_ = node_->get_parameter(name + ".kp").as_double();
  goal_tolerance_ = node_->get_parameter(name + ".goal_tolerance").as_double();

  // 注册参数回调函数
  params_callback_handle_ = node_->add_on_set_parameters_callback(
    [this](const std::vector<rclcpp::Parameter> &parameters) 
    -> rcl_interfaces::msg::SetParametersResult 
    {
      auto result = rcl_interfaces::msg::SetParametersResult();
      result.successful = true;
      
      // 加锁确保线程安全
      std::lock_guard<std::mutex> lock(params_mutex_);
      
      for (const auto &param : parameters) {
        // 检查是否是我们关心的参数
        if (param.get_name() == name_ + ".lookahead_dist") {
          lookahead_dist_ = param.as_double();
          RCLCPP_INFO(
            node_->get_logger(), 
            "更新前瞻距离: %.2f m", lookahead_dist_
          );
        } 
        else if (param.get_name() == name_ + ".max_speed") {
          max_speed_ = param.as_double();
          RCLCPP_INFO(
            node_->get_logger(), 
            "更新最大速度: %.2f m/s", max_speed_
          );
        }
        else if (param.get_name() == name_ + ".kp") {
          kp_ = param.as_double();
          RCLCPP_INFO(
            node_->get_logger(), 
            "更新转向系数: %.2f", kp_
          );
        }
        else if (param.get_name() == name_ + ".goal_tolerance") {
          goal_tolerance_ = param.as_double();
          RCLCPP_INFO(
            node_->get_logger(), 
            "更新目标容差: %.2f m", goal_tolerance_
          );
        }
      }
      
      return result;
    }
  );

  RCLCPP_INFO(node_->get_logger(), "SimplePlanner 配置完成");
}

void SimplePlanner::setPlan(const nav_msgs::msg::Path &path) {
  global_path_ = path;  // 保存全局路径
  
  // 添加调试日志 - 关键修复
  RCLCPP_INFO(node_->get_logger(), 
             "收到新全局路径，坐标系: %s, 点数: %zu", 
             global_path_.header.frame_id.c_str(),
             global_path_.poses.size());
  
  // 确保坐标系不为空
  if (global_path_.header.frame_id.empty()) {
    RCLCPP_ERROR(node_->get_logger(), "警告: 全局路径缺少坐标系!");
  }
}

geometry_msgs::msg::TwistStamped SimplePlanner::computeVelocityCommands(
  const geometry_msgs::msg::PoseStamped &pose,
  const geometry_msgs::msg::Twist &velocity,
  nav2_core::GoalChecker *goal_checker)
{
  // 加锁确保参数访问安全
  std::lock_guard<std::mutex> lock(params_mutex_);
  
  // 创建空速度命令
  geometry_msgs::msg::TwistStamped cmd_vel;
  cmd_vel.header.stamp = node_->now();
  cmd_vel.header.frame_id = "base_link";

  // 检查路径是否有效
  if (global_path_.poses.empty()) {
    RCLCPP_WARN(node_->get_logger(), "全局路径为空!");
    return cmd_vel;
  }
  
  // 检查全局路径坐标系 - 关键修复
  if (global_path_.header.frame_id.empty()) {
    RCLCPP_ERROR(node_->get_logger(), "全局路径缺少坐标系! 无法进行坐标变换");
    return cmd_vel;
  }

  // 目标点检查
  if (goal_checker->isGoalReached(pose.pose, global_path_.poses.back().pose, velocity)) {
    RCLCPP_INFO(node_->get_logger(), "已到达目标点");
    return cmd_vel;  // 返回零速度
  }

  try {
    // 1. 搜索前瞻目标点
    geometry_msgs::msg::PointStamped target_point;
    bool target_found = false;

    for (size_t i = 0; i < global_path_.poses.size(); ++i) {
      // 确保路径点有坐标系 - 关键修复
      auto path_pose = global_path_.poses[i];
      
      // 如果路径点没有坐标系，使用全局路径的坐标系
      if (path_pose.header.frame_id.empty()) {
        path_pose.header.frame_id = global_path_.header.frame_id;
        RCLCPP_WARN_ONCE(node_->get_logger(), 
                        "路径点 %zu 缺少坐标系，使用全局路径坐标系: %s", 
                        i, global_path_.header.frame_id.c_str());
      }
      
      // 跳过仍然没有坐标系的点
      if (path_pose.header.frame_id.empty()) {
        RCLCPP_WARN(node_->get_logger(), "路径点 %zu 仍然缺少坐标系，跳过", i);
        continue;
      }
      
      try {
        // 转换到base_link坐标系 - 关键修复
        geometry_msgs::msg::PoseStamped transformed;
        
        // 使用正确的转换方法
        transformed = tf_buffer_->transform(
          path_pose, 
          "base_link",
          tf2::durationFromSec(0.5)  // 增加超时时间
        );

        // 计算距离
        double dist = std::hypot(
          transformed.pose.position.x,
          transformed.pose.position.y
        );

        // 找到第一个超过前瞻距离的点
        if (dist > lookahead_dist_) {
          target_point.point = transformed.pose.position;
          target_point.header = transformed.header;
          target_found = true;
          break;
        }
      } catch (const tf2::TransformException &ex) {
        RCLCPP_ERROR(node_->get_logger(), "TF转换失败: %s", ex.what());
        RCLCPP_ERROR(node_->get_logger(), "源坐标系: %s", 
                    path_pose.header.frame_id.c_str());
      }
    }

    // 2. 计算控制命令
    if (target_found) {
      // 纯追踪算法计算转向
      double angle = atan2(target_point.point.y, target_point.point.x);
      cmd_vel.twist.angular.z = kp_ * angle;

      // 动态速度控制（离目标越近速度越慢）
      double dist_to_target = std::hypot(
        target_point.point.x, 
        target_point.point.y
      );
      cmd_vel.twist.linear.x = std::min(
        max_speed_, 
        max_speed_ * (dist_to_target / lookahead_dist_)
      );
    } else {
      // 接近终点时减速
      auto& last_pose = global_path_.poses.back().pose.position;
      double dist_to_goal = std::hypot(last_pose.x, last_pose.y);
      
      if (dist_to_goal > goal_tolerance_) {
        cmd_vel.twist.linear.x = max_speed_ * 0.3;
        cmd_vel.twist.angular.z = kp_ * atan2(last_pose.y, last_pose.x);
      }
    }
  } catch (const std::exception &ex) {
    RCLCPP_ERROR(node_->get_logger(), "计算控制命令异常: %s", ex.what());
  }

  return cmd_vel;
}

}  // namespace e07_nav2_bringup

// ========== 插件导出宏 ==========
#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(
  e07_nav2_bringup::SimplePlanner, 
  nav2_core::Controller
)