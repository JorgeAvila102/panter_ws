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

    void vel_drive_panter();
    void ED_IZQ_callback(const geometry_msgs::msg::Wrench::SharedPtr msg);
    void ED_DCH_callback(const geometry_msgs::msg::Wrench::SharedPtr msg);
    void ET_IZQ_callback(const geometry_msgs::msg::Wrench::SharedPtr msg);
    void ET_DCH_callback(const geometry_msgs::msg::Wrench::SharedPtr msg);

    geometry_msgs::msg::Wrench ED_IZQ_dato;
    geometry_msgs::msg::Wrench ED_DCH_dato;
    geometry_msgs::msg::Wrench ET_IZQ_dato;    
    geometry_msgs::msg::Wrench ET_DCH_dato;

    rclcpp::TimerBase::SharedPtr sensor_timer_;

    double vel_actual = 1.0;
    double giro_actual = 0.5;
    double inc = 0.5;

private:
    
    // PUBLICADORES

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub_cmd_vel;

    // SUBSCRIPCIONES

    rclcpp::Subscription<geometry_msgs::msg::Wrench>::SharedPtr sub_ED_IZQ;
    rclcpp::Subscription<geometry_msgs::msg::Wrench>::SharedPtr sub_ED_DCH;
    rclcpp::Subscription<geometry_msgs::msg::Wrench>::SharedPtr sub_ET_IZQ;
    rclcpp::Subscription<geometry_msgs::msg::Wrench>::SharedPtr sub_ET_DCH;

    const double vel_max = 10.0; // 36 Km/h
    const double giro_max = 5.0; // 36 Km/h
};