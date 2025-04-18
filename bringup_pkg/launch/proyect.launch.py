from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    # Rutas a los paquetes
    description_pkg = get_package_share_directory('description_pkg')
    control_pkg = get_package_share_directory('control_pkg')

    # Lanzar Gazebo con el robot en SDF
    gazebo_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            #os.path.join(description_pkg, 'launch', 'sim_sdf.launch.py')
            os.path.join(description_pkg, 'launch', 'sim_urdf.launch.py')
        )
    )

    # Lanzar el puente de comunicación entre ROS 2 y Gazebo (ros_gz_bridge)
    bridge_node = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        parameters=[{
            'config_file': os.path.join(description_pkg, 'config', 'ros_gz_bridge.yaml')
            #'qos_overrides./tf_static.publisher.durability': 'transient_local',
        }],
        #arguments=['/cmd_vel@geometry_msgs/msg/Twist@gz.msgs.Twist'],
        output='screen'
    )

    # Lanzar el nodo de control en C++
    control_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(control_pkg, 'launch', 'ros.launch.py')
        )
    )

    return LaunchDescription([
        
        bridge_node,
        control_launch,
        gazebo_launch
    ])