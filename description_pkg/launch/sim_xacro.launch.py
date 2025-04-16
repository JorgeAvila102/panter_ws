# LAUNCH PRUEBAS

import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, ExecuteProcess
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, Command
from launch.conditions import IfCondition
from launch_ros.parameter_descriptions import ParameterValue

from launch_ros.actions import Node

def generate_launch_description():
    
    description_pkg_path = get_package_share_directory('description_pkg')
    xacro_file = os.path.join(description_pkg_path, 'urdf', 'panter.urdf.xacro')
    world_file = os.path.join(get_package_share_directory('description_pkg'),'worlds', 'my_world.sdf')
    rviz_file = os.path.join(get_package_share_directory('description_pkg'),'rviz', 'panter.rviz')

    robot_description = ParameterValue(
        Command(['xacro ', xacro_file]),
        value_type=str
    )

    params = {'robot_description': robot_description}

    node_robot_state_publisher = Node(

        package= 'robot_state_publisher',
        executable= 'robot_state_publisher',
        output= 'screen',
        parameters=[params],
        arguments=[xacro_file]
    )

    node_joint_state_publisher = Node(

        package= 'joint_state_publisher',
        executable= 'joint_state_publisher',
        name= 'joint_state_publisher',
        parameters=[params],
        arguments=[xacro_file]
    )   

    node_joint_state_publisher_gui = Node(

        package= 'joint_state_publisher_gui',
        executable= 'joint_state_publisher_gui',
        name= 'joint_state_publisher_gui',
        arguments=[xacro_file]
        # condition= IfCondition(LaunchConfiguration('gui'))
    )   

# Gazebo launcher
    launch_gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([get_package_share_directory('ros_gz_sim'), '/launch', '/gz_sim.launch.py']),
        launch_arguments={
        'ign_args' : world_file
        # 'ign_args': f"{world_file} --physics-engine bullet"
    }.items(),
    )

# Robot Panter spawner in Gazebo
    spawn_panter = Node(
        package='ros_gz_sim',
        executable='create',
        output='screen',
        arguments=["-file", Command(['xacro ', xacro_file]), "-z", "0.08"]
    )

    node_rviz = Node(

        package= 'rviz2',
        executable= 'rviz2',
        name= 'rviz2',
        output= 'screen',
        arguments=['-d', rviz_file]
    ) 

    return LaunchDescription([
         
        node_robot_state_publisher,
        node_joint_state_publisher_gui, # Hace que tenemos la posibilidad de modificar las posiciones de los joints a mano
        node_joint_state_publisher,
        node_rviz,
        launch_gazebo,
        spawn_panter
    ])


