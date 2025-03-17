#include <stdio.h>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

#include "controller.hpp"

Controller::Controller(): Node ("panter_controller")
{
    pub_cmd_vel = this->create_publisher<geometry_msgs::msg::Twist> ("/cmd_vel", 10);
}

Controller::~Controller()
{
    printf("Leaving gently\n");
}

void Controller:: auto_drive_panter()
{

    auto msg = geometry_msgs::msg::Twist();
    msg.linear.x = 2.0; // Velocidad en m/s
    msg.angular.z = 0.5; // Velocidad en rad/s

    pub_cmd_vel -> publish(msg);
    RCLCPP_INFO(this->get_logger(), "Enviando cmd_vel: linear=%.2f, angular=%.2f",
                msg.linear.x, msg.angular.z);

}

int main ( int argc, char * argv[] )
{
    rclcpp::init (argc, argv);

    auto node = std::make_shared<Controller>();

    rclcpp::Rate loop_rate(5);
  
    printf("Hello world\n");

    while (rclcpp::ok())
    {
        rclcpp::spin_some(node);

        node->auto_drive_panter();
        loop_rate.sleep();
    }
    
    return 0;
}