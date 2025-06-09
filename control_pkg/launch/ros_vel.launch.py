from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import TimerAction  # <-- Import necesario
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():

    control_pkg = get_package_share_directory('control_pkg')
    robot_controllers = os.path.join(control_pkg, 'config', 'control_config.yaml')
    urdf_file = os.path.join(get_package_share_directory('description_pkg'),'urdf', 'panter.urdf')

    control_node = Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[robot_controllers],
        output="both",
        remappings=[
            ("~/robot_description", "/robot_description"),
        ],
    )

    ros_control = Node(
        package='control_pkg',
        executable='controlVelocidad',
        name='controlVelocidad',
        output='screen',
        prefix='xterm -e',
        arguments=['--ros-args', '--log-level', 'info']
    )

    node_joint_state_publisher = Node(

        package= 'joint_state_publisher',
        executable= 'joint_state_publisher',
        name= 'joint_state_publisher',
        parameters=["/robot_description"],
        arguments=[urdf_file]
    )   

    return LaunchDescription([
        ros_control,
        control_node,
        node_joint_state_publisher
    ])
