#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp" // Para velocidad linear y angular
#include "geometry_msgs/msg/wrench.hpp"
#include <string>
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float64.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"


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

    rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr pub_torque;
    rclcpp::Publisher<std_msgs::msg::Float64MultiArray>::SharedPtr pub_giro;

    // SUBSCRIPCIONES

    rclcpp::Subscription<geometry_msgs::msg::Wrench>::SharedPtr sub_ED_IZQ;
    rclcpp::Subscription<geometry_msgs::msg::Wrench>::SharedPtr sub_ED_DCH;
    rclcpp::Subscription<geometry_msgs::msg::Wrench>::SharedPtr sub_ET_IZQ;
    rclcpp::Subscription<geometry_msgs::msg::Wrench>::SharedPtr sub_ET_DCH;

    static constexpr double REDUCCION = 15.0;
    static constexpr double T_continuo = 34.0 * REDUCCION; // 510 Nm
    static constexpr double T_pico     = 120.0 * REDUCCION; // 1800 Nm

    static constexpr double alpha     = 0.50; // Ángulo de giro de unos ~30 grados.

    double limitarTorque(double par_calculado) {
        if (par_calculado > T_pico)
            return T_pico;
        else if (par_calculado > T_continuo)
            return T_continuo;
        else if (par_calculado < -T_pico)
            return -T_pico;
        else if (par_calculado < -T_continuo)
            return -T_continuo;
        else
            return par_calculado;
    }
    
};