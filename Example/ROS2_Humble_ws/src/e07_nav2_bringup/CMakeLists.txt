cmake_minimum_required(VERSION 3.8)
project(e07_nav2_bringup)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译器选项
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 查找依赖
find_package(ament_cmake REQUIRED)
find_package(nav2_bringup REQUIRED)
find_package(nav2_common REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav2_core REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(pluginlib REQUIRED)

# 安装目录
install(DIRECTORY 
  launch 
  params 
  maps 
  rviz 
  bts
  DESTINATION share/${PROJECT_NAME}
)

# 添加包含路径
include_directories(
  include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 添加自定义规划器库
add_library(simple_planner SHARED
  src/simple_planner.cpp
)

# 设置目标包含目录
target_include_directories(simple_planner PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# 链接依赖库
target_link_libraries(simple_planner
  ${rclcpp_LIBRARIES}
  ${nav_msgs_LIBRARIES}
  ${geometry_msgs_LIBRARIES}
  ${tf2_ros_LIBRARIES}
  ${pluginlib_LIBRARIES}
  ${nav2_core_LIBRARIES}
)

# 安装规则
install(TARGETS simple_planner
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# 安装头文件
install(DIRECTORY include/
  DESTINATION include
)

# 导出插件
pluginlib_export_plugin_description_file(nav2_core plugins.xml)

# 测试配置
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()