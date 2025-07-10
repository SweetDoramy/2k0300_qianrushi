#pragma once
#include "nav2_core/controller.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "nav_msgs/msg/path.hpp"
#include "tf2_ros/buffer.h"
#include <mutex>

namespace e07_nav2_bringup {

class SimplePlanner : public nav2_core::Controller {
public:
  void configure(
    const rclcpp_lifecycle::LifecycleNode::WeakPtr &parent,
    std::string name, std::shared_ptr<tf2_ros::Buffer> tf,
    std::shared_ptr<nav2_costmap_2d::Costmap2DROS> costmap_ros) override;

  void setPlan(const nav_msgs::msg::Path &path) override;

  geometry_msgs::msg::TwistStamped computeVelocityCommands(
    const geometry_msgs::msg::PoseStamped &pose,
    const geometry_msgs::msg::Twist &velocity,
    nav2_core::GoalChecker *goal_checker) override;

  // 未使用的接口空实现
  void activate() override {}
  void deactivate() override {}
  void cleanup() override {}
  
  // 消除未使用参数警告
  void setSpeedLimit(const double &speed_limit, const bool &percentage) override 
  {
    (void)speed_limit;
    (void)percentage;
  }

private:
  // 核心参数
  double lookahead_dist_;
  double max_speed_;
  double kp_;
  double goal_tolerance_;

  // 系统组件
  std::string name_;
  nav_msgs::msg::Path global_path_;
  std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;  // 添加 TF 监听器
  std::shared_ptr<rclcpp_lifecycle::LifecycleNode> node_;
  
  // 动态参数回调成员
  rclcpp::node_interfaces::OnSetParametersCallbackHandle::SharedPtr params_callback_handle_;
  std::mutex params_mutex_;
};

}  // namespace e07_nav2_bringup