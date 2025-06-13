#include <stdio.h>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/wrench.hpp"

#include "keyboardcontrolTorque.hpp"

using std::placeholders::_1;

KeyboardcontrolTorque::KeyboardcontrolTorque(): Node ("keyboardcontrolTorque")
{
    pub_torque = this->create_publisher<std_msgs::msg::Float64MultiArray>(
        "/effort_controller/commands", 10);

    pub_giro = this->create_publisher<std_msgs::msg::Float64MultiArray>(
        "/steering_controller/commands", 10);

    sub_ED_IZQ = this->create_subscription<geometry_msgs::msg::Wrench>(
        "/model/panter/ED_IZQ_joint/sensor/force_torque_sensor/force_torque", 10,
                            std::bind(&KeyboardcontrolTorque::ED_IZQ_callback, this, _1));

    sub_ED_DCH = this->create_subscription<geometry_msgs::msg::Wrench>(
        "/model/panter/ED_DCH_joint/sensor/force_torque_sensor/force_torque", 10,
                            std::bind(&KeyboardcontrolTorque::ED_DCH_callback, this, _1));

    sub_ET_IZQ = this->create_subscription<geometry_msgs::msg::Wrench>(
         "/model/panter/ET_IZQ_joint/sensor/force_torque_sensor/force_torque", 10,
                            std::bind(&KeyboardcontrolTorque::ET_IZQ_callback, this, _1));

    sub_ET_DCH = this->create_subscription<geometry_msgs::msg::Wrench>(
        "/model/panter/ET_DCH_joint/sensor/force_torque_sensor/force_torque", 10,
                            std::bind(&KeyboardcontrolTorque::ET_DCH_callback, this, _1));

    // sensor_timer_ = this->create_wall_timer(
    //     std::chrono::seconds(5),
    //     [this]() {
    //         // RCLCPP_INFO(this->get_logger(),
    //         // "\n--- Torque Y (tracción) de cada rueda [Nm] ---\n"
    //         // "  ED_IZQ: %.2f\n"
    //         // "  ED_DCH: %.2f\n"
    //         // "  ET_IZQ: %.2f\n"
    //         // "  ET_DCH: %.2f\n",
    //         // ED_IZQ_dato.torque.y,
    //         // ED_DCH_dato.torque.y,
    //         // ET_IZQ_dato.torque.y,
    //         // ET_DCH_dato.torque.y
    //         // );
    //         RCLCPP_INFO(this->get_logger(),
    //             "ED_IZQ: %.2f | ED_DCH: %.2f | ET_IZQ: %.2f | ET_DCH: %.2f [Nm]",
    //             ED_IZQ_dato.torque.y,
    //             ED_DCH_dato.torque.y,
    //             ET_IZQ_dato.torque.y,
    //             ET_DCH_dato.torque.y
    //         );
    //     }
    // );
    // PANEL EN TIEMPO REAL DE TORQUE
    sensor_timer_ = this->create_wall_timer(
        std::chrono::milliseconds(100), // 10Hz
        [this]() {
            std::cout << "\rED_IZQ: " << ED_IZQ_dato.torque.y
                      << " | ED_DCH: " << ED_DCH_dato.torque.y
                      << " | ET_IZQ: " << ET_IZQ_dato.torque.y
                      << " | ET_DCH: " << ET_DCH_dato.torque.y
                      << " [Nm]   " << std::flush;
        }
    );
}

KeyboardcontrolTorque::~KeyboardcontrolTorque()
{
    printf("Leaving gently\n");
}

void KeyboardcontrolTorque::ED_IZQ_callback(const geometry_msgs::msg::Wrench::SharedPtr msg)
{
    ED_IZQ_dato.force.x = msg->force.x;
    ED_IZQ_dato.force.y = msg->force.y;
    ED_IZQ_dato.force.z = msg->force.z;

    ED_IZQ_dato.torque.x = msg->torque.x;
    ED_IZQ_dato.torque.y = msg->torque.y;
    ED_IZQ_dato.torque.z = msg->torque.z;
}

void KeyboardcontrolTorque::ED_DCH_callback(const geometry_msgs::msg::Wrench::SharedPtr msg)
{
    ED_DCH_dato.force.x = msg->force.x;
    ED_DCH_dato.force.y = msg->force.y;
    ED_DCH_dato.force.z = msg->force.z;

    ED_DCH_dato.torque.x = msg->torque.x;
    ED_DCH_dato.torque.y = msg->torque.y;
    ED_DCH_dato.torque.z = msg->torque.z;
}

void KeyboardcontrolTorque::ET_IZQ_callback(const geometry_msgs::msg::Wrench::SharedPtr msg)
{
    ET_IZQ_dato.force.x = msg->force.x;
    ET_IZQ_dato.force.y = msg->force.y;
    ET_IZQ_dato.force.z = msg->force.z;

    ET_IZQ_dato.torque.x = msg->torque.x;
    ET_IZQ_dato.torque.y = msg->torque.y;
    ET_IZQ_dato.torque.z = msg->torque.z;
}

void KeyboardcontrolTorque::ET_DCH_callback(const geometry_msgs::msg::Wrench::SharedPtr msg)
{
    ET_DCH_dato.force.x = msg->force.x;
    ET_DCH_dato.force.y = msg->force.y;
    ET_DCH_dato.force.z = msg->force.z;

    ET_DCH_dato.torque.x = msg->torque.x;
    ET_DCH_dato.torque.y = msg->torque.y;
    ET_DCH_dato.torque.z = msg->torque.z;
}

void KeyboardcontrolTorque::keyboard_loop()
{
    std_msgs::msg::Float64MultiArray torque_msg;
    std_msgs::msg::Float64MultiArray giro_msg;

// Set console raw mode. To avoid pressing enter after each character
    system("stty raw");

    while (rclcpp::ok()) {

        char input = getchar();

        switch (input)
        {


    // AVANZA HACIA ADELANTE

        case 'w':   RCLCPP_INFO(this->get_logger(), "FORDWARDS \r\n");

        torque_msg.data = {T_continuo, T_continuo, T_continuo, T_continuo};   
        giro_msg.data = {0, 0};
           
        break;

    // AVANZA HACIA ATRÁS
        
        case 's':
        RCLCPP_INFO(this->get_logger(), "BACKWARD \r\n");

        torque_msg.data = {-T_continuo, -T_continuo, -T_continuo, -T_continuo}; 
        giro_msg.data = {0, 0}; 

        break;

    // GIRA DERECHA AVANZANDO

        case 'd':
        RCLCPP_INFO(this->get_logger(), "RIGHT \r\n");

        torque_msg.data = {T_continuo, T_continuo, T_continuo, T_continuo}; 
        giro_msg.data = {-alpha, -alpha}; 

        break;

    // GIRA IZQUIERDA AVANZANDO

        case 'a':
        RCLCPP_INFO(this->get_logger(), "LEFT \r\n");

        torque_msg.data = {T_continuo, T_continuo, T_continuo, T_continuo};  
        giro_msg.data = {alpha, alpha};

        break;

    // GIRA DERECHA RETROCEDIENDO

        case 'c':
        RCLCPP_INFO(this->get_logger(), "RIGHT BACK \r\n");

        torque_msg.data = {-T_continuo, -T_continuo, -T_continuo, -T_continuo};
        giro_msg.data = {-alpha, -alpha};

        break;

    // GIRA IZQUIERDA RETROCEDIENDO

        case 'z':
        RCLCPP_INFO(this->get_logger(), "LEFT BACK \r\n");

        torque_msg.data = {-T_continuo, -T_continuo, -T_continuo, -T_continuo};
        giro_msg.data = {alpha, alpha};

        break;

    // PARAR

        case 'x':
        RCLCPP_INFO(this->get_logger(), "STOP \r\n");

        torque_msg.data = {-ED_IZQ_dato.torque.y, -ED_DCH_dato.torque.y, -ET_IZQ_dato.torque.y, -ET_DCH_dato.torque.y};
        giro_msg.data = {0, 0};
        
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

        pub_torque->publish(torque_msg);
        pub_giro  ->publish(giro_msg);
    }
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<KeyboardcontrolTorque>();

    std::thread keyboard_thread(&KeyboardcontrolTorque::keyboard_loop, node);

    rclcpp::spin(node);
    keyboard_thread.join();

    rclcpp::shutdown();
    return 0;
}