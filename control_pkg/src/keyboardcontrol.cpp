#include <stdio.h>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

#include "keyboardcontrol.hpp"

Keyboardcontrol::Keyboardcontrol(): Node ("keyboardcontrol")
{
    pub_cmd_vel = this->create_publisher<geometry_msgs::msg::Twist> ("/cmd_vel", 10);
}

Keyboardcontrol::~Keyboardcontrol()
{
    printf("Leaving gently\n");
}

void Keyboardcontrol:: manual_drive_panter()
{

    auto msg = geometry_msgs::msg::Twist();

    // std_msgs::msg::String mensaje;

// Set console raw mode. To avoid pressing enter after each character
    system("stty raw");
    // Read 1 char
    char input = getchar();
    
    switch (input)
    {

// AVANZA HACIA ADELANTE
    case 'w':
    RCLCPP_INFO(this->get_logger(), "FORDWARDS \r\n");

    msg.linear.x = 2.0; // Velocidad en m/s
    msg.angular.z = 0; // Velocidad en rad/s

    pub_cmd_vel -> publish(msg);
    
    // mensaje.data = "FORDWARDS";
    // pub_move -> publish(mensaje);

    break;
// AVANZA HACIA ATRÁS
    
    case 's':
    RCLCPP_INFO(this->get_logger(), "BACKWARD \r\n");

    msg.linear.x = -2.0; // Velocidad en m/s
    msg.angular.z = 0; // Velocidad en rad/s

    pub_cmd_vel -> publish(msg);
    
    // mensaje.data = "BACKWARD";
    // pub_move -> publish(mensaje);

    break;

// GIRA DERECHA
    case 'd':
    RCLCPP_INFO(this->get_logger(), "RIGHT \r\n");

    msg.linear.x = 0; // Velocidad en m/s
    msg.angular.z = -0.5; // Velocidad en rad/s

    pub_cmd_vel -> publish(msg);
    
    // mensaje.data = "RIGHT";
    // pub_move -> publish(mensaje);

    break;

// GIRA IZQUIERDA
    case 'a':
    RCLCPP_INFO(this->get_logger(), "LEFT \r\n");

    msg.linear.x = 0; // Velocidad en m/s
    msg.angular.z = 0.5; // Velocidad en rad/s

    pub_cmd_vel -> publish(msg);
    
    // mensaje.data = "LEFT";
    // pub_move -> publish(mensaje);

    break;


// PARAR
    case 'p':
    RCLCPP_INFO(this->get_logger(), "STOP \r\n");

    msg.linear.x = 0; // Velocidad en m/s
    msg.angular.z = 0; // Velocidad en rad/s

    pub_cmd_vel -> publish(msg);
    
    // mensaje.data = "STOP";
    // pub_move -> publish(mensaje);

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

    auto node = std::make_shared<Keyboardcontrol>();

    rclcpp::Rate loop_rate(5);
  

    while (rclcpp::ok())
    {
        rclcpp::spin_some(node);

        node->manual_drive_panter();
        loop_rate.sleep();
    }
    
    return 0;
}