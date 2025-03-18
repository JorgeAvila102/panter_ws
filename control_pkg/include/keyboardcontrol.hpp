#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp" // Para velocidad linear y angular
#include <string>
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float32.hpp"

class Keyboardcontrol : public rclcpp::Node
{

public:

    Keyboardcontrol();          // CONSTRUCTOR
    ~Keyboardcontrol();         // DESTRUCTOR

    void manual_drive_panter();

private:
    
    // PUBLICADORES

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_cmd_vel;
    // rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub_move;

    // SUBSCRIPCIONES

};