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

KeyboardcontrolTorque::~KeyboardcontrolTorque()
{
    printf("Leaving gently\n");
}

void KeyboardcontrolTorque::ED_IZQ_callback(const geometry_msgs::msg::Wrench::SharedPtr msg)
{
    ED_IZQ_dato.force.x = msg->force.x;
    ED_IZQ_dato.torque.y = msg->torque.y;
}

void KeyboardcontrolTorque::ED_DCH_callback(const geometry_msgs::msg::Wrench::SharedPtr msg)
{
    ED_DCH_dato.force.x = msg->force.x;
    ED_DCH_dato.torque.y = msg->torque.y;
}

void KeyboardcontrolTorque::ET_IZQ_callback(const geometry_msgs::msg::Wrench::SharedPtr msg)
{
    ET_IZQ_dato.force.x = msg->force.x;
    ET_IZQ_dato.torque.y = msg->torque.y;
}

void KeyboardcontrolTorque::ET_DCH_callback(const geometry_msgs::msg::Wrench::SharedPtr msg)
{
    ET_DCH_dato.force.x = msg->force.x;
    ET_DCH_dato.torque.y = msg->torque.y;
}


void KeyboardcontrolTorque::keyboard_loop()
{
    std_msgs::msg::Float64MultiArray torque_msg;
    std_msgs::msg::Float64MultiArray giro_msg;

// Set console raw mode
    system("stty raw");

    while (rclcpp::ok()) {

        char input = getchar();

        switch (input)
        {

    // AUMENTA PAR

        case '+':   RCLCPP_INFO(this->get_logger(), "Aumenta PAR \r\n");

            if(par_actual < par_max){

                par_actual = par_actual + inc;

                RCLCPP_INFO(this->get_logger(), "Par actual: %.2f Nm", par_actual);
            }else
            {
                RCLCPP_INFO(this->get_logger(), "Par actual: %.2f Nm", par_actual);
            }  

        break;

    // DISMINUYE PAR

        case '-':   RCLCPP_INFO(this->get_logger(), "Disminuye PAR \r\n");

            if(par_actual > 0){

                par_actual = par_actual - inc;

                RCLCPP_INFO(this->get_logger(), "Par actual: %.2f Nm", par_actual);
            }else
            {
                RCLCPP_INFO(this->get_logger(), "Par actual: %.2f Nm", par_actual);
            } 
           
        break;

        // AUMENTA GIRO

        case '1':   RCLCPP_INFO(this->get_logger(), "Aumenta Giro \r\n");

            if(giro_actual < giro_max){

                giro_actual = giro_actual + inc_giro;

                RCLCPP_INFO(this->get_logger(), "Giro actual: %.2f grad", (giro_actual*180)/PI);
            }else
            {
                RCLCPP_INFO(this->get_logger(), "Giro actual: %.2f grad", (giro_actual*180)/PI);
            }  

        break;

    // DISMINUYE PAR

        case '0':   RCLCPP_INFO(this->get_logger(), "Disminuye Giro \r\n");

            if(giro_actual > 10){

                giro_actual = giro_actual - inc_giro;

                RCLCPP_INFO(this->get_logger(), "Giro actual: %.2f grad", (giro_actual*180)/PI);
            }else
            {
                RCLCPP_INFO(this->get_logger(), "Giro actual: %.2f grad", (giro_actual*180)/PI);
            } 
           
        break;

    // AVANZA HACIA ADELANTE

        case 'w':   RCLCPP_INFO(this->get_logger(), "FORDWARDS \r\n");

        torque_msg.data = {par_actual, par_actual, par_actual, par_actual};   
        giro_msg.data = {0, 0};
           
        break;

    // AVANZA HACIA ATRÁS
        
        case 's':
        RCLCPP_INFO(this->get_logger(), "BACKWARD \r\n");

        torque_msg.data = {-par_actual, -par_actual, -par_actual, -par_actual}; 
        giro_msg.data = {0, 0}; 

        break;

    // GIRA DERECHA AVANZANDO

        case 'd':
        RCLCPP_INFO(this->get_logger(), "RIGHT \r\n");

        torque_msg.data = {par_actual, par_actual, par_actual, par_actual}; 
        giro_msg.data = {-giro_actual, -giro_actual}; 

        break;

    // GIRA IZQUIERDA AVANZANDO

        case 'a':
        RCLCPP_INFO(this->get_logger(), "LEFT \r\n");

        torque_msg.data = {par_actual, par_actual, par_actual, par_actual};  
        giro_msg.data = {giro_actual, giro_actual};

        break;

    // GIRA DERECHA RETROCEDIENDO

        case 'c':
        RCLCPP_INFO(this->get_logger(), "RIGHT BACK \r\n");

        torque_msg.data = {-par_actual, -par_actual, -par_actual, -par_actual};
        giro_msg.data = {-giro_actual, -giro_actual};

        break;

    // GIRA IZQUIERDA RETROCEDIENDO

        case 'z':
        RCLCPP_INFO(this->get_logger(), "LEFT BACK \r\n");

        torque_msg.data = {-par_actual, -par_actual, -par_actual, -par_actual};
        giro_msg.data = {giro_actual, giro_actual};

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