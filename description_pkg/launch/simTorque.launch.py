# LAUNCH PRUEBAS

import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, ExecuteProcess
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch.conditions import IfCondition

from launch_ros.actions import Node

def generate_launch_description():

    urdf_file = os.path.join(get_package_share_directory('description_pkg'),'urdf', 'panter_torque.urdf')
    world_file = os.path.join(get_package_share_directory('description_pkg'),'worlds', 'my_world.sdf')
    rviz_file = os.path.join(get_package_share_directory('description_pkg'),'rviz', 'panter.rviz')

    
    with open (urdf_file, 'r') as infp:
         robot_desc = infp.read()

    params = {'robot_description': robot_desc}

    robot_state_publisher = Node(
         
      package='robot_state_publisher',
      executable='robot_state_publisher',
      parameters=[params, {'use_sim_time': True}],
    )


    joint_state_publisher = Node(

        package= 'joint_state_publisher',
        executable= 'joint_state_publisher',
        name= 'joint_state_publisher',
        parameters=[
            {'robot_description': robot_desc},
            {'use_sim_time': True}],
        arguments=[urdf_file]
    )   

    joint_state_publisher_gui = Node(

        package= 'joint_state_publisher_gui',
        executable= 'joint_state_publisher_gui',
        name= 'joint_state_publisher_gui',
        arguments=[urdf_file]
        # condition= IfCondition(LaunchConfiguration('gui'))
    )   

    launch_gazebo = IncludeLaunchDescription(
      PythonLaunchDescriptionSource(
        os.path.join(
          get_package_share_directory('ros_gz_sim'),
          'launch',
          'gz_sim.launch.py'
        )
      ),
      launch_arguments={'ign_args': world_file}.items()
    )

    spawn_panter = Node(
      package='ros_gz_sim',
      executable='create',
      arguments=['-topic', 'robot_description', '-gazebo-ros-control', '-z', '0.08'],
      output='screen'
    )

# # Gazebo launcher
#     launch_gazebo = IncludeLaunchDescription(
#         PythonLaunchDescriptionSource([get_package_share_directory('ros_gz_sim'), '/launch', '/gz_sim.launch.py']),
#         launch_arguments={
#         'ign_args' : world_file
#         # 'ign_args': f"{world_file} --physics-engine bullet"
#     }.items(),
#     )

# # Robot Panter spawner in Gazebo
#     spawn_panter = Node(
#         package='ros_gz_sim',
#         executable='create',
#         output='screen',
#         arguments=["-topic", "robot_description","-z", "0.08"]
#     )

    node_rviz = Node(

        package= 'rviz2',
        executable= 'rviz2',
        name= 'rviz2',
        output= 'screen',
        parameters=[{'use_sim_time': True}],
        arguments=['-d', rviz_file]
    ) 

    return LaunchDescription([
         
        robot_state_publisher,
        joint_state_publisher_gui, 
        joint_state_publisher,
        node_rviz,
        launch_gazebo,
        spawn_panter
    ])


