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
    # urdf_path = os.path.join(description_pkg, 'urdf', 'panter.urdf')  # Ajusta el nombre real de tu URDF
    # controller_config = os.path.join(control_pkg, 'config', 'effort_controllers.yaml')  # Ajusta si es necesario
    bridge_config = os.path.join(description_pkg, 'config', 'ros_gz_bridge.yaml')
    rviz_config = os.path.join(description_pkg, 'rviz', 'panter.rviz')
    # control_yaml = os.path.join(control_pkg, 'config', 'control_config.yaml')


    # Lanzar Gazebo con el robot
    gazebo_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(description_pkg, 'launch', 'sim_urdf.launch.py')
        )
    )

    # Lanzar el nodo de control ROS
    control_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(control_pkg, 'launch', 'ros.launch.py')
        )
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
        gazebo_launch,
        control_launch,
        bridge_node,
        rviz_node
    ])

    # # Spawner del controlador de effort y de posición
    # controller_spawner = Node(
    #     package='controller_manager',
    #     executable='spawner',
    #     arguments=["effort_controller"],
    #     output='screen'
    # )

    # steering_spawner = Node(
    #     package='controller_manager',
    #     executable='spawner',
    #     arguments=['steering_controller'],
    #     output='screen'
    # )