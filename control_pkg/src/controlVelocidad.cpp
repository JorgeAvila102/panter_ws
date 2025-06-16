#include <stdio.h>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/wrench.hpp"

#include "controlVelocidad.hpp"

using std::placeholders::_1;

ControlVelocidad::ControlVelocidad(): Node ("controlVelocidad")
{
    pub_cmd_vel = this->create_publisher<geometry_msgs::msg::Twist> ("/cmd_vel", 10);
    
    sub_ED_IZQ = this->create_subscription<geometry_msgs::msg::Wrench>(
        "/model/panter/ED_IZQ_joint/sensor/force_torque_sensor/force_torque", 10,
                            std::bind(&ControlVelocidad::ED_IZQ_callback, this, _1));

    sub_ED_DCH = this->create_subscription<geometry_msgs::msg::Wrench>(
        "/model/panter/ED_DCH_joint/sensor/force_torque_sensor/force_torque", 10,
                            std::bind(&ControlVelocidad::ED_DCH_callback, this, _1));

    sub_ET_IZQ = this->create_subscription<geometry_msgs::msg::Wrench>(
         "/model/panter/ET_IZQ_joint/sensor/force_torque_sensor/force_torque", 10,
                            std::bind(&ControlVelocidad::ET_IZQ_callback, this, _1));

    sub_ET_DCH = this->create_subscription<geometry_msgs::msg::Wrench>(
        "/model/panter/ET_DCH_joint/sensor/force_torque_sensor/force_torque", 10,
                            std::bind(&ControlVelocidad::ET_DCH_callback, this, _1));

    sensor_timer_ = this->create_wall_timer(
        std::chrono::milliseconds(4000),
        [this]() {

            std::cout << "\rED_IZQ: " << ED_IZQ_dato.torque.y
                    << " | ED_DCH: " << ED_DCH_dato.torque.y
                    << " | ET_IZQ: " << ET_IZQ_dato.torque.y
                    << " | ET_DCH: " << ET_DCH_dato.torque.y
                    << " [Nm]   " << std::flush;
        }
    );

}

ControlVelocidad::~ControlVelocidad()
{
    printf("Leaving gently\n");
}

void ControlVelocidad::ED_IZQ_callback(const geometry_msgs::msg::Wrench::SharedPtr msg)
{
    ED_IZQ_dato.force.x = msg->force.x;
    ED_IZQ_dato.force.y = msg->force.y;
    ED_IZQ_dato.force.z = msg->force.z;

    ED_IZQ_dato.torque.x = msg->torque.x;
    ED_IZQ_dato.torque.y = msg->torque.y;
    ED_IZQ_dato.torque.z = msg->torque.z;
}

void ControlVelocidad::ET_IZQ_callback(const geometry_msgs::msg::Wrench::SharedPtr msg)
{
    ET_IZQ_dato.force.x = msg->force.x;
    ET_IZQ_dato.force.y = msg->force.y;
    ET_IZQ_dato.force.z = msg->force.z;

    ET_IZQ_dato.torque.x = msg->torque.x;
    ET_IZQ_dato.torque.y = msg->torque.y;
    ET_IZQ_dato.torque.z = msg->torque.z;
}

void ControlVelocidad::ET_DCH_callback(const geometry_msgs::msg::Wrench::SharedPtr msg)
{
    ET_DCH_dato.force.x = msg->force.x;
    ET_DCH_dato.force.y = msg->force.y;
    ET_DCH_dato.force.z = msg->force.z;

    ET_DCH_dato.torque.x = msg->torque.x;
    ET_DCH_dato.torque.y = msg->torque.y;
    ET_DCH_dato.torque.z = msg->torque.z;
}

void ControlVelocidad::ED_DCH_callback(const geometry_msgs::msg::Wrench::SharedPtr msg)
{
    ED_DCH_dato.force.x = msg->force.x;
    ED_DCH_dato.force.y = msg->force.y;
    ED_DCH_dato.force.z = msg->force.z;

    ED_DCH_dato.torque.x = msg->torque.x;
    ED_DCH_dato.torque.y = msg->torque.y;
    ED_DCH_dato.torque.z = msg->torque.z;

}

void ControlVelocidad:: vel_drive_panter()
{

    auto msg = geometry_msgs::msg::Twist();

    msg.linear.x = 0.0; // Velocidad en m/s
    msg.angular.z = 0;

// Set console raw mode
    system("stty raw");

    while (rclcpp::ok()) {

        char input = getchar();

        switch (input)
        {

            // AUMENTA PAR

        case '+':   RCLCPP_INFO(this->get_logger(), "Aumenta Velocidad \r\n");

            if(vel_actual < vel_max){

                vel_actual = vel_actual + inc;

                RCLCPP_INFO(this->get_logger(), "Velocidad actual: %.2f m/s", vel_actual);
            }else
            {
                RCLCPP_INFO(this->get_logger(), "Velocidad actual: %.2f m/s", vel_actual);
            }  

        break;

    // DISMINUYE PAR

        case '-':   RCLCPP_INFO(this->get_logger(), "Disminuye Velocidad \r\n");

            if(vel_actual > 0){

                vel_actual = vel_actual - inc;

                RCLCPP_INFO(this->get_logger(), "Velocidad actual: %.2f m/s", vel_actual);
            }else
            {
                RCLCPP_INFO(this->get_logger(), "Velocidad actual: %.2f m/s", vel_actual);
            } 
           
        break;

        // AUMENTA PAR

        case '1':   RCLCPP_INFO(this->get_logger(), "Aumenta Giro \r\n");

            if(giro_actual < vel_max){

                giro_actual = giro_actual + inc;

                RCLCPP_INFO(this->get_logger(), "Giro actual: %.2f rad/s", giro_actual);
            }else
            {
                RCLCPP_INFO(this->get_logger(), "Giro actual: %.2f rad/s", giro_actual);
            }  

        break;

    // DISMINUYE PAR

        case '0':   RCLCPP_INFO(this->get_logger(), "Disminuye Giro \r\n");

            if(giro_actual > 0){

                giro_actual = giro_actual - inc;

                RCLCPP_INFO(this->get_logger(), "Giro actual: %.2f rad/s", giro_actual);
            }else
            {
                RCLCPP_INFO(this->get_logger(), "Giro actual: %.2f rad/s", giro_actual);
            } 
           
        break;

    // AVANZA HACIA ADELANTE
        case 'w':
        RCLCPP_INFO(this->get_logger(), "FORDWARDS \r\n");

        msg.linear.x = vel_actual; // Velocidad en m/s
        msg.angular.z = 0; // Velocidad en rad/s

        break;

    // AVANZA HACIA ATRÁS
        
        case 's':
        RCLCPP_INFO(this->get_logger(), "BACKWARD \r\n");

        msg.linear.x = -vel_actual; // Velocidad en m/s
        msg.angular.z = 0; // Velocidad en rad/s

        break;

    // GIRA DERECHA AVANZANDO
        case 'd':
        RCLCPP_INFO(this->get_logger(), "RIGHT \r\n");

        msg.linear.x = vel_actual; // Velocidad en m/s
        msg.angular.z = -giro_actual; // Velocidad en rad/s

        break;

    // GIRA IZQUIERDA AVANZANDO
        case 'a':
        RCLCPP_INFO(this->get_logger(), "LEFT \r\n");

        msg.linear.x = vel_actual; // Velocidad en m/s
        msg.angular.z = giro_actual; // Velocidad en rad/s

        break;

        // GIRA DERECHA RETROCEDIENDO

        case 'c':
        RCLCPP_INFO(this->get_logger(), "RIGHT BACK \r\n");

        msg.linear.x = -vel_actual; // Velocidad en m/s
        msg.angular.z = giro_actual; // Velocidad en rad/s

        break;

        // GIRA DERECHA RETROCEDIENDO

        case 'z':
        RCLCPP_INFO(this->get_logger(), "LEFT BACK \r\n");

        msg.linear.x = -vel_actual; // Velocidad en m/s
        msg.angular.z = -giro_actual; // Velocidad en rad/s

        break;

    // PARAR
        case 'x':
        RCLCPP_INFO(this->get_logger(), "STOP \r\n");

        msg.linear.x = 0; // Velocidad en m/s
        msg.angular.z = 0; // Velocidad en rad/s

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

            pub_cmd_vel -> publish(msg);
        }

        pub_cmd_vel -> publish(msg);
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ControlVelocidad>();

    std::thread keyboard_thread(&ControlVelocidad::vel_drive_panter, node);

    rclcpp::spin(node);
    keyboard_thread.join();

    rclcpp::shutdown();
    return 0;
}