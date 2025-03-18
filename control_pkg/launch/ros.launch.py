from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package="control_pkg",
            # executable="controller",
            executable="keyboardcontrol",
            # parameters=["config/ros2_controllers.yaml"],
            output="screen"
        ),
        Node(
            package='control_pkg',
            executable='keyboardcontrol',
            name='keyboardcontrol',
            output='screen',
            prefix='xterm -e'  # Esto abre una terminal interactiva
        )
    ])