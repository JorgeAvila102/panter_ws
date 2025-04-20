from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([

        Node(
            package='control_pkg',
            executable='keyboardcontrol',
            name='keyboardcontrol',
            output='screen',
            prefix='xterm -e',  # Esto abre una terminal interactiva
            # remappings=[
            #     ('/model/panter/ET_DCH_joint/sensor/force_torque_sensor/force_torque', '/model/ET_DCH/force_torque')
            # ],
            arguments=['--ros-args', '--log-level', 'info']
        )
    ])

