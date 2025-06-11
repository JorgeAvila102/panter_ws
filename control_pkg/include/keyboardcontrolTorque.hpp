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

    static constexpr double T_continuo = 32.0;  // N·m
    static constexpr double alpha = 0.5236;     // Radianes
    static constexpr double pendiente_grad = 15;
    
    static constexpr double pendiente = pendiente_grad * M_PI / 180.0;
    static constexpr double g = 9.81;
    static constexpr double masa = 343;     // Kg
    static constexpr double R_rueda = 0.343;     // Kg

    // Fuerza total necesaria para subir la pendiente: F = m·g·sin(α)
    static constexpr double F_total = masa * g * std::sin(pendiente);

    // Torque total en el eje de ruedas: T = F·R
    static constexpr double T_total = F_total * R_rueda;

    // Repartir entre las 4 ruedas
    static constexpr double T_por_rueda = T_total / 4.0;

    // Limitar al par continuo; si se pide más, usar máximo continuo
    // static constexpr double T_cmd = std::min(T_por_rueda, T_continuo);
    static constexpr double T_cmd = 34.0; //= T_por_rueda;
    
};