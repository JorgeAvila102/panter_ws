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

    # Rutas a archivos
    urdf_path = os.path.join(description_pkg, 'urdf', 'panter.urdf')  # Ajusta el nombre real de tu URDF
    controller_config = os.path.join(control_pkg, 'config', 'effort_controllers.yaml')  # Ajusta si es necesario
    bridge_config = os.path.join(description_pkg, 'config', 'ros_gz_bridge.yaml')

    # Lanzar Gazebo con el robot
    gazebo_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(description_pkg, 'launch', 'sim_urdf.launch.py')
        )
    )

    # Lanzar el puente ros_gz_bridge
    bridge_node = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        parameters=[{'config_file': bridge_config}],
        output='screen'
    )

    # Nodo de ros2_control
    # ros2_control_node = Node(
    #     package='controller_manager',
    #     executable='ros2_control_node',
    #     parameters=[controller_config],
    #     output='screen'
    # )

    # Spawner del controlador
    controller_spawner = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['effort_controller'],
        output='screen'
    )

    # Lanzar el nodo de control (C++ con teclado)
    control_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(control_pkg, 'launch', 'ros.launch.py')
        )
    )

    return LaunchDescription([
        gazebo_launch,
        bridge_node,
        # ros2_control_node,
        controller_spawner,
        control_launch
    ])
