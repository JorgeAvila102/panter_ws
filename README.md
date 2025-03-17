panter_ws

# Instalaciones realizadas

ROS2 Humble: https://docs.ros.org/en/humble/Installation.html

Gazebo Fortress: https://gazebosim.org/docs/fortress/install_ubuntu/

sudo apt install ros-humble-joint-state-publisher

sudo apt install gedit

sudo apt install terminator

# Creación estrucutra del proyecto

mkdir panter_ws

mkdir src

(cd panter_ws)

colcon build

ros2 pkg create --build-type ament_cmake description_pkg

ros2 pkg create --build-type ament_cmake control_pkg --dependencies rclcpp geometry_msgs

ros2 pkg create --build-type ament_cmake bringup_pkg --dependencies rclpy launch_ros

cd description_pkg

mkdir sdf meshes world urdf launch rviz

cd control_pkg

mkdir launch config


# Instalaciones git

sudo apt update

sudo apt install git -y

git config --global user.name "jorge"

git config --global user.email "j.avilaorero@gmail.com"

