from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import TimerAction  # <-- Import necesario
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():

    control_pkg = get_package_share_directory('control_pkg')
    controller_config = os.path.join(control_pkg, 'config', 'control_config.yaml')
    urdf_file = os.path.join(get_package_share_directory('description_pkg'),'urdf', 'panter.urdf')


    # Controlador de ros2_control
    control_node = Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[
            controller_config,
            {'use_sim_time': True},
        ],
        output="both",
        remappings=[
            ("~/robot_description", "/robot_description"),
        ],
    )

    node_joint_state_publisher = Node(

        package= 'joint_state_publisher',
        executable= 'joint_state_publisher',
        name= 'joint_state_publisher',
        parameters=["/robot_description"],
        arguments=[urdf_file]
    )   

    # joint_state_broadcaster = TimerAction(
    #     period=2.0,
    #     actions=[ Node(
    #         package='controller_manager',
    #         executable='spawner',
    #         arguments=['joint_state_broadcaster'],
    #         output='screen',
    #         parameters=["/robot_description"],
    #         arguments=[urdf_file]
    #     ) ]
    # )

    # Spawner del controlador de effort
    effort_spawner = TimerAction(
        period=4.0,
        actions=[ Node(
            package='controller_manager',
            executable='spawner',
            arguments=['effort_controller'],
            output='screen'
        ) ]
    )

    # Spawner del controlador de posición de la dirección
    steering_spawner = TimerAction(
        period=6.0,
        actions=[ Node(
            package='controller_manager',
            executable='spawner',
            arguments=['steering_controller'],
            output='screen'
        ) ]
    )

    # Nodo de control por teclado en ROS 
    ros_control = Node(
        package='control_pkg',
        executable='keyboardcontrolTorque',
        name='keyboardcontrolTorque',
        output='screen',
        prefix='xterm -e',
        arguments=['--ros-args', '--log-level', 'info']
    )

    return LaunchDescription([
        ros_control,
        node_joint_state_publisher,
        # joint_state_broadcaster,
        effort_spawner,
        steering_spawner,
        control_node
    ])









    # node_robot_state_publisher = Node(

    #     package= 'robot_state_publisher',
    #     executable= 'robot_state_publisher',
    #     output= 'screen',
    #     parameters=[params],
    #     arguments=[urdf_file]
    # )

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