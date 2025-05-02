#include <stdio.h>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/wrench.hpp"

#include "keyboardcontrolTorque.hpp"

using std::placeholders::_1;

KeyboardcontrolTorque::KeyboardcontrolTorque(): Node ("keyboardcontrolTorque")
{
    pub_cmd_vel = this->create_publisher<geometry_msgs::msg::Twist> ("/cmd_vel", 10);

    pub_torque = this->create_publisher<std_msgs::msg::Float64MultiArray>(
        "/effort_controller/commands", 10);

    sub_ET_DCH = this->create_subscription<geometry_msgs::msg::Wrench>(
         "/model/panter/ET_DCH_joint/sensor/force_torque_sensor/force_torque", 10,
        //  "/model/ET_DCH/force_torque", 10,
                            std::bind(&KeyboardcontrolTorque::ET_DCH_callback, this, _1));

    sub_ED_DCH = this->create_subscription<geometry_msgs::msg::Wrench>(
        "/model/panter/ED_DCH_joint/sensor/force_torque_sensor/force_torque", 10,
        //  "/model/ED_DCH/force_torque", 10,
                            std::bind(&KeyboardcontrolTorque::ED_DCH_callback, this, _1));
}

KeyboardcontrolTorque::~KeyboardcontrolTorque()
{
    printf("Leaving gently\n");
}

void KeyboardcontrolTorque::ET_DCH_callback(const geometry_msgs::msg::Wrench::SharedPtr msg)
{
    ET_DCH_dato.force.x = msg->force.x;
    ET_DCH_dato.force.y = msg->force.y;
    ET_DCH_dato.force.z = msg->force.z;

    ET_DCH_dato.torque.x = msg->torque.x;
    ET_DCH_dato.torque.y = msg->torque.y;
    ET_DCH_dato.torque.z = msg->torque.z;
    
    printf("ET_DCH force: %f.\r\n", ET_DCH_dato.force.x );

    RCLCPP_INFO(this->get_logger(), 
        "ET_DCH force: [%.2f, %.2f, %.2f], torque: [%.2f, %.2f, %.2f]",
        ET_DCH_dato.force.x, ET_DCH_dato.force.y, ET_DCH_dato.force.z,
        ET_DCH_dato.torque.x, ET_DCH_dato.torque.y, ET_DCH_dato.torque.z);

}

void KeyboardcontrolTorque::ED_DCH_callback(const geometry_msgs::msg::Wrench::SharedPtr msg)
{
    ED_DCH_dato.force.x = msg->force.x;
    ED_DCH_dato.force.y = msg->force.y;
    ED_DCH_dato.force.z = msg->force.z;

    ED_DCH_dato.torque.x = msg->torque.x;
    ED_DCH_dato.torque.y = msg->torque.y;
    ED_DCH_dato.torque.z = msg->torque.z;
    
    printf("ED_DCH force: %f.\r\n", ED_DCH_dato.force.x );

    RCLCPP_INFO(this->get_logger(), 
        "ED_DCH force: [%.2f, %.2f, %.2f], torque: [%.2f, %.2f, %.2f]",
        ED_DCH_dato.force.x, ED_DCH_dato.force.y, ED_DCH_dato.force.z,
        ED_DCH_dato.torque.x, ED_DCH_dato.torque.y, ED_DCH_dato.torque.z);
}


void KeyboardcontrolTorque:: manual_drive_panter()
{
    std_msgs::msg::Float64MultiArray torque_msg;
    auto msg = geometry_msgs::msg::Twist();

// Set console raw mode. To avoid pressing enter after each character
    system("stty raw");
    // Read 1 char
    char input = getchar();
    
    switch (input)
    {

// AVANZA HACIA ADELANTE
    case 'w':
    RCLCPP_INFO(this->get_logger(), "FORDWARDS \r\n");

    msg.angular.z = 0; // Velocidad en rad/s
    torque_msg.data = {100.0, 100.0, 100.0, 100.0};
    pub_torque->publish(torque_msg);
    pub_cmd_vel -> publish(msg);
    
    break;
// AVANZA HACIA ATRÁS
    
    case 's':
    RCLCPP_INFO(this->get_logger(), "BACKWARD \r\n");

    msg.angular.z = 0; // Velocidad en rad/s
    torque_msg.data = {-100.0, -100.0, -100.0, -100.0};
    pub_torque->publish(torque_msg);
    pub_cmd_vel -> publish(msg);

    break;

// GIRA DERECHA
    case 'd':
    RCLCPP_INFO(this->get_logger(), "RIGHT \r\n");

    msg.angular.z = -0.5; // Velocidad en rad/s
    torque_msg.data = {10.0, 50.0, 10.0, 50.0};
    pub_torque->publish(torque_msg);
    pub_cmd_vel -> publish(msg);

    break;

// GIRA IZQUIERDA
    case 'a':
    RCLCPP_INFO(this->get_logger(), "LEFT \r\n");

    msg.angular.z = 0.5; // Velocidad en rad/s
    torque_msg.data = {50.0, 10.0, 50.0, 10.0};
    pub_torque->publish(torque_msg);
    pub_cmd_vel -> publish(msg);

    break;

// PARAR
    case 'p':
    RCLCPP_INFO(this->get_logger(), "STOP \r\n");

    msg.angular.z = 0; // Velocidad en rad/s
    torque_msg.data = {0.0, 0.0, 0.0, 0.0};
    pub_torque->publish(torque_msg);
    pub_cmd_vel -> publish(msg);

    break;

// SALIR DE MODO RAW
    case 0x20:
    RCLCPP_INFO(this->get_logger(), "Detected SPACE = Exit");
    
    //restore the console
    system("stty cooked");
    break;

    default:
    // ignoramos caracteres no deseados
    RCLCPP_INFO(this->get_logger(), "Non valid KEY\r\n");
    break;
    
    }
}

int main ( int argc, char * argv[] )
{
    rclcpp::init (argc, argv);
    auto node = std::make_shared<KeyboardcontrolTorque>();
    rclcpp::Rate loop_rate(5);

    geometry_msgs::msg::Wrench dato;
  
    while (rclcpp::ok())
    {
        rclcpp::spin_some(node);

        node->manual_drive_panter();
        loop_rate.sleep();
    }
    
    return 0;
}