#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp" // Para velocidad linear y angular
#include "geometry_msgs/msg/wrench.hpp"
#include <string>
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float32.hpp"

class ControlVelocidad : public rclcpp::Node
{

public:

    ControlVelocidad();          // CONSTRUCTOR
    ~ControlVelocidad();         // DESTRUCTOR

    void manual_drive_panter();
    void ET_DCH_callback(const geometry_msgs::msg::Wrench::SharedPtr msg);
    void ED_DCH_callback(const geometry_msgs::msg::Wrench::SharedPtr msg);
    geometry_msgs::msg::Wrench ET_DCH_dato;
    geometry_msgs::msg::Wrench ED_DCH_dato;

private:
    
    // PUBLICADORES

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_cmd_vel;

    // SUBSCRIPCIONES

    rclcpp::Subscription<geometry_msgs::msg::Wrench>::SharedPtr sub_ET_DCH;
    rclcpp::Subscription<geometry_msgs::msg::Wrench>::SharedPtr sub_ED_DCH;
};