# Simulador Panter en Gazebo

Este proyecto consiste en un Simulador de un robot movil en Gazebo.

![PanterenEscenario](https://github.com/user-attachments/assets/94668fd8-8737-40dc-ad20-ccca80fc2957)

El robot se llama Panter y pertenece a la Universidad de Málaga.
Se desplaza mediante direccionamiento de Ackermann y tiene tracción en las 4 ruedas.

## Instalación del Simulador de Panter

El sistema operativo en el que se diseña el simulador de Panter es Ubuntu 22.04.5, sobreél, hay que hacer las siguientes instalaciones.

• Instalación de ROS 2 Humble de la ([web oficial ROS](https://docs.ros.org/en/humble/Installation/Ubuntu-Install-Debs.html)).

• Instalación de controlador gz_ros2_control de la ([web oficial](https://control.ros.org/humble/doc/gz_ros2_control/doc/index.html)):

```
sudo apt install ros-humble-gz-ros2-control ros-humble-gz-ros2-control-demos
```

• Instalación de Gazebo Fortress de la ([web oficial Gazebo](https://gazebosim.org/docs/fortress/install_ubuntu/)).

Luego, para tener el proyecto en el dispositivo, es necesario crear un clon de este proyecto
de GitHub. 

Clonamos el repositorio del proyecto:
```
git clone https://github.com/JorgeAvila102/panter_ws.git
```

## Configurar bash
```
echo source /opt/ros/humble/setup.bash >> ~/.bashrc

echo source $HOME/panter_ws/install/setup.bash >> ~/.bashrc

echo export IGN_GAZEBO_RESOURCE_PATH="$HOME/panter_ws/install/description_pkg/share/description_pkg/worlds:$HOME/panter_ws/install/description_pkg/share/description_pkg/models:$IGN_GAZEBO_RESOURCE_PATH" >> ~/.bashrc

echo export GZ_SIM_RESOURCE_PATH="$HOME/panter_ws/install/description_pkg/share/description_pkg/worlds:$HOME/panter_ws/install/description_pkg/share/description_pkg/models:$GZ_SIM_RESOURCE_PATH" >> ~/.bashrc

echo export IGN_GAZEBO_SYSTEM_PLUGIN_PATH="$IGN_GAZEBO_SYSTEM_PLUGIN_PATH:/opt/ros/humble/lib" >> ~/.bashrc

echo export IGN_GAZEBO_PHYSICS_ENGINE_PATH="/usr/lib/x86_64-linux-gnu/ignition/physics" >> ~/.bashrc

echo export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/opt/ros/humble/lib:$HOME/panter_ws/install/gz_ros2_control/lib" >> ~/.bashrc
```
## WorkSpace y compilación

Abrimos una nueva terminal:
```
cd panter_ws
mkdir src
mv bringup_pkg control_pkg description_pkg src/
mv doc README.md src/
mkdir -p ~/panter_ws/src/bringup_pkg/include
mkdir -p ~/panter_ws/src/description_pkg/include
cd ~/panter_ws
colcon build

```

## Otras instalaciones

• Visual Studio Code:

```
cd ~
sudo apt install ./code_1.72.0-1664925838_arm64.deb
```

• Xterm:

```
cd ~
sudo apt update
sudo apt-get install xterm
```

Entorno de trabajo de Visual Studio Code:

```
cd panter_ws
mkdir .vscode
```

Y podemos añadir en la carpeta .vscode, los siguientes archivos de configuración que facilitarán su uso y compilación:

• c_cpp_properties.json:
```
{
  "configurations": [
    {
      "name": "Linux",
        "includePath": [
        "${workspaceFolder}/**",
        "/opt/ros/humble/include/**"
        ],
        "defines": [],
        "compilerPath": "/usr/bin/gcc",
        "cStandard": "c17",
        "cppStandard": "gnu++17",
        "intelliSenseMode": "clang-x64"
      }
  ],
    "version": 4
}
```
• tasks.json: permite compilar pulsando las teclas ctrl + Shift + b:
```

{
  "version": "2.0.0",
  "tasks": [
        {
        "type": "shell",
        "command": "colcon",
        "args": [
        "build", "--symlink-install"
        ],
        "group": "build",
        "label": "colcon: build",
        "presentation": {
        72
        "panel": "new",
        "focus": true,
        "reveal": "silent",
        "clear": true
} } ] }
```

## Modificación en URDF
Para el correcto funcionamiento, debemos realizar el siguiente cambio en el archivo pan-ter.urdf: cambiar el usuario jorge, por el nuevo usuario.
```
$ sed -i "s|/home/jorge/panter_ws|$HOME/panter_ws|g"~/panter_ws/src/control_pkg/urdf/panter.urdf
```

# Uso del Simulador
Una vez las herramientas se instalan, y el entorno de trabajo se modifica, podemos utilizar el simulador.

## Compilación del WorkSpace
```
cd ~/panter_ws
colcon build
```

## Lanzamiento del simulador

• Control de velocidad:
```
cd ~/panter_ws/src
ros2 launch bringup_pkg proyect_vel.launch.py
```
• Control de torque:
```
cd ~/panter_ws/src
ros2 launch bringup_pkg proyect.launch.py
```

IMPORTANTE en el control de torque: se debe pulsar el play en Gazebo
antes de 5 segundos, en el caso de que no sean pulsados, los controladores dan
error y es necesario volver a lanzarlo.

## Control por teclado

| Tecla | Acción                   |
|:-----:|:-------------------------|
| `w`   | Avanzar recto            |
| `d`   | Avanzar derecha          |
| `a`   | Avanzar izquierda        |
| `s`   | Retroceder recto         |
| `c`   | Retroceder derecha       |
| `z`   | Retroceder izquierda     |
| `x`   | Detener el robot         |
| `+`   | Aumentar par             |
| `-`   | Disminuir par            |
| `1`   | Aumentar ángulo de giro  |
| `0`   | Disminuir ángulo de giro |








