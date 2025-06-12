#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp" // Para velocidad linear y angular
#include "geometry_msgs/msg/wrench.hpp"
#include <string>
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float64.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
#include <geometry_msgs/msg/twist_stamped.hpp>


class KeyboardcontrolTorque : public rclcpp::Node
{

public:

    KeyboardcontrolTorque();          // CONSTRUCTOR
    ~KeyboardcontrolTorque();         // DESTRUCTOR

    void keyboard_loop();

    void ED_IZQ_callback(const geometry_msgs::msg::Wrench::SharedPtr msg);
    void ED_DCH_callback(const geometry_msgs::msg::Wrench::SharedPtr msg);
    void ET_IZQ_callback(const geometry_msgs::msg::Wrench::SharedPtr msg);
    void ET_DCH_callback(const geometry_msgs::msg::Wrench::SharedPtr msg);

    geometry_msgs::msg::Wrench ED_IZQ_dato;
    geometry_msgs::msg::Wrench ED_DCH_dato;
    geometry_msgs::msg::Wrench ET_IZQ_dato;    
    geometry_msgs::msg::Wrench ET_DCH_dato;

    rclcpp::TimerBase::SharedPtr sensor_timer_;

private:
    
    // PUBLICADORES

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_cmd_vel;
    rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr ackermann_publisher;

    // SUBSCRIPCIONES

    rclcpp::Subscription<geometry_msgs::msg::Wrench>::SharedPtr sub_ED_IZQ;
    rclcpp::Subscription<geometry_msgs::msg::Wrench>::SharedPtr sub_ED_DCH;
    rclcpp::Subscription<geometry_msgs::msg::Wrench>::SharedPtr sub_ET_IZQ;
    rclcpp::Subscription<geometry_msgs::msg::Wrench>::SharedPtr sub_ET_DCH;

    static constexpr double vel_l = 1.0; 
    static constexpr double vel_w = 0.5; 
    
};