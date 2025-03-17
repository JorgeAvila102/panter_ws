#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp" // Para velocidad linear y angular
#include <string>

class Controller : public rclcpp::Node
{

public:

    Controller();          // CONSTRUCTOR
    ~Controller();         // DESTRUCTOR

    void auto_drive_panter();
    // void manual_drive_panter();

private:
    
    // PUBLICADORES

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_cmd_vel;

    // SUBSCRIPCIONES

};