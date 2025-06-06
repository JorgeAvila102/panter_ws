# LAUNCH PRUEBAS

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource

from launch_ros.actions import Node

def generate_launch_description():

    urdf_file = os.path.join(get_package_share_directory('description_pkg'),'urdf', 'panter.urdf')
    # world_file = os.path.join(get_package_share_directory('description_pkg'),'worlds', 'my_world.sdf')
    world_file = os.path.join(get_package_share_directory('description_pkg'),'worlds', 'tierra_world.sdf')
    
    with open (urdf_file, 'r') as infp:
         robot_desc = infp.read()

    params = [
        {'robot_description': robot_desc},
        {'use_sim_time': True},
    ]

    # Publica URDF y TF
    node_robot_state_publisher = Node(
        package= 'robot_state_publisher',
        executable= 'robot_state_publisher',
        output= 'screen',
        parameters=params,
        arguments=[urdf_file]
    )


    launch_gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(
               get_package_share_directory('ros_gz_sim'),
               'launch',
               'gz_sim.launch.py'
            )
        ),
        launch_arguments=[('gz_args', world_file)],
    )

    # Robot Panter spawner in Gazebo
    spawn_panter = Node(
        package='ros_gz_sim',
        executable='create',
        output='screen',
        arguments=["-topic", "robot_description", "-y", "28.0","-z", "0.08"]
    )

    return LaunchDescription([
         
        node_robot_state_publisher,
        launch_gazebo,
        spawn_panter
    ])




    # params = {'robot_description' : robot_desc}

    # Se aplica cuando no utilizamos ros2_control
    # node_joint_state_publisher = Node(

    #     package= 'joint_state_publisher',
    #     executable= 'joint_state_publisher',
    #     name= 'joint_state_publisher',
    #     parameters=[params],
    #     arguments=[urdf_file]
    # )   

    # node_joint_state_publisher_gui = Node(

    #     package= 'joint_state_publisher_gui',
    #     executable= 'joint_state_publisher_gui',
    #     name= 'joint_state_publisher_gui',
    #     arguments=[urdf_file]
    #     # condition= IfCondition(LaunchConfiguration('gui'))
    # )   

# Gazebo launcher
    # launch_gazebo = IncludeLaunchDescription(
    #     PythonLaunchDescriptionSource([get_package_share_directory('ros_gz_sim'), 
    #                                    '/launch', '/gz_sim.launch.py']),
    #     # launch_arguments={
    #     # 'ign_args' : world_file}.items(),
    #     launch_arguments=[
    #         ('ign_args', world_file),
    #     ],
    # )


# # 2) Spawner del joint_state_broadcaster
#     node_joint_state_broadcaster = Node(
#         package='controller_manager',
#         executable='spawner',
#         arguments=['joint_state_broadcaster', '--controller-manager', '/controller_manager'],
#         output='screen'
#     )

#     node_rviz = Node(

#         package= 'rviz2',
#         executable= 'rviz2',
#         name= 'rviz2',
#         output= 'screen',
#         arguments=['-d', rviz_file]
#     ) 

        # node_joint_state_publisher_gui, 
        # node_joint_state_publisher,
        # node_joint_state_broadcaster,
        # node_rviz,