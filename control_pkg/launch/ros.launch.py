from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package="control_pkg",
            executable="controller",
            # parameters=["config/ros2_controllers.yaml"],
            output="screen"
        )
    ])