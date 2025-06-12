from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import TimerAction  # <-- Import necesario
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution
import os
from ament_index_python.packages import get_package_share_directory
from launch_ros.substitutions import FindPackageShare
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.actions import RegisterEventHandler
from launch.event_handlers import OnProcessExit
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution

from launch_ros.actions import Node

def generate_launch_description():

    control_pkg = get_package_share_directory('control_pkg')
    description_pkg = get_package_share_directory('description_pkg')
    controller_config = os.path.join(description_pkg, 'config', 'control_config.yaml')
    urdf_file = os.path.join(get_package_share_directory('description_pkg'),'urdf', 'panter.urdf')
    #world_file = os.path.join(get_package_share_directory('description_pkg'),'worlds', 'my_world.sdf')
    world_file = os.path.join(get_package_share_directory('description_pkg'),'worlds', 'tierra_world.sdf')
    bridge_config = os.path.join(description_pkg, 'config', 'ros_gz_bridge.yaml')
    rviz_config = os.path.join(description_pkg, 'rviz', 'panter.rviz')

    use_sim_time = LaunchConfiguration('use_sim_time', default=True)

    with open(urdf_file, 'r') as infp:
        robot_desc = infp.read()

    robot_description = {
        'robot_description': robot_desc
    }

    robot_controllers = PathJoinSubstitution(
        [
            #FindPackageShare('gz_ros2_control_demos'),
            description_pkg,
            'config',
            'control_config.yaml',
        ]
    )

    node_robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[robot_description]
    )

    # node_joint_state_publisher = Node(

    #     package= 'joint_state_publisher',
    #     executable= 'joint_state_publisher',
    #     name= 'joint_state_publisher',
    #     parameters=["/robot_description"],
    #     arguments=[urdf_file]
    # )   

    spawn_panter = Node(
        package='ros_gz_sim',
        executable='create',
        output='screen',
        arguments=["-topic", "robot_description", "-y", "0.0","-z", "0.33"]
    )

    joint_state_broadcaster_spawner = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['joint_state_broadcaster'],
    )

    ackermann_steering_controller_spawner = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['ackermann_steering_controller',
                   '--param-file',
                   robot_controllers,
                   ],
    )

    # Nodo de control por teclado en ROS 
    keyboard_control = Node(
        package='control_pkg',
        executable='keyboardcontrolTorque',
        name='keyboardcontrolTorque',
        output='screen',
        prefix='xterm -e',
        arguments=['--ros-args', '--log-level', 'info']
    )

    # Lanzar el puente ros_gz_bridge
    bridge_node = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        parameters=[{'config_file': bridge_config}],
        output='screen'
    )

    # 4) RViz, lanzado al final

    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        output='screen',
        arguments=['-d', rviz_config],
        parameters=[{'use_sim_time': True}],
    )
    

    return LaunchDescription([
            bridge_node,
            # Launch gazebo environment
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(
                    [PathJoinSubstitution([FindPackageShare('ros_gz_sim'),
                                        'launch',
                                        'gz_sim.launch.py'])]),
                launch_arguments=[('gz_args', world_file)]),
            RegisterEventHandler(
                event_handler=OnProcessExit(
                    target_action=spawn_panter,
                    on_exit=[joint_state_broadcaster_spawner],
                )
            ),
            RegisterEventHandler(
                event_handler=OnProcessExit(
                    target_action=joint_state_broadcaster_spawner,
                    on_exit=[ackermann_steering_controller_spawner],
                )
            ),
            
            node_robot_state_publisher,
            spawn_panter,
            keyboard_control,
            # Launch Arguments
            DeclareLaunchArgument(
                'use_sim_time',
                default_value=use_sim_time,
                description='If true, use simulated clock'),
        ]
        # rviz_node
        )




