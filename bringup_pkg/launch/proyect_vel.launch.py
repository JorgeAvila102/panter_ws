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
    rviz_file = os.path.join(get_package_share_directory('description_pkg'),'rviz', 'panter.rviz')

    # Rutas a archivos
    urdf_path = os.path.join(description_pkg, 'urdf', 'panter_vel.urdf')
    bridge_config = os.path.join(description_pkg, 'config', 'ros_gz_bridge.yaml')

    # Lanzar Gazebo con el robot
    gazebo_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(description_pkg, 'launch', 'simVel.launch.py')
        )
    )

    # Lanzar el puente ros_gz_bridge
    bridge_node = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        parameters=[{'config_file': bridge_config}],
        output='screen'
    )

    # Lanzar el nodo de control (C++ con teclado)
    control_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(control_pkg, 'launch', 'ros_vel.launch.py')
        )
    )
    node_rviz = Node(

        package= 'rviz2',
        executable= 'rviz2',
        name= 'rviz2',
        output= 'screen',
        arguments=['-d', rviz_file]
    ) 

    return LaunchDescription([
        gazebo_launch,
        control_launch,
        bridge_node,
        node_rviz
    ])
