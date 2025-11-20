# 1. Challenge-1 (Simulation and Implementation)

Design and implement an embedded system that controls a sequence of four LEDs using a single push button.\
The system should allow the user to start, stop, and resume the sequence with the same button.\
The system must meet the following conditions:
- When the button is pressed for the first time, the four LEDs should start blinking one after another from left to right, changing every one second (1 s).
- When the button is pressed again, the sequence should pause at its current position.
- When the button is pressed a third time, the sequence should resume from where it stopped.
- The behavior should be cyclic: every press of the button toggles between “running” and “stopped” states.

## Metodología de diseño e implementación de sistemas embebidos
### 1. Análisis de requerimientos
#### Requerimientos funcionales

- FR-01: Al iniciar el sistema debe comenzar en estado de pausa, con todos los LEDs apagados
- FR-02: Al presionar el botón por primera vez el sistema debe encender los LEDs en secuencia a intervalos de 1s, solamente puede haber un LED encendido a la vez
- FR-03: Al presionar el botón por segunda vez el sistema debe quedar en pausa. El LED que estaba encendido cuando se presionó el botón debe quedar encendido.
- FR-04: Al presionar el botón por tercera vez el sistema debe seguir encendiendo y apagando los LEDS, comenzando por donde terminó.
- FR-05: El botón debe implementar algún método de antirrebote para evitar inconsistencias en el comportamiento del sistema
- FR-06: El botón debe activarse con un pulso negativo

#### Requerimientos no funcionales

- NFR-01: El sistema debe iniciar automaticamente al conectarse a la energía
- NFR-02: Los LEDs deben proveer el tipo de luminosidad de chorro
- NFR-03: Los LEDSs debe arreglarse en línea recta con un espaciado uniforme
- NFR-04: El color de los 4 LEDs debe ser distinto

#### Restricciones

- CON-01: Los LEDs debe conectarse a los pines GPIO y activarse con señales digitales
- CON-02: Debe implementarse una resistencia pull-Up interna para el botón
- CON-03: Los LEDs debe conectarse en serie con una resistencia para limitar la corriente
- CON-04: Se debe entregar el código del sistema en lenguaje Arduino C++ en un archivo con extensión **.ino**
- CON-05: Se dbe hacer una simulación usando [Wokwi](https://wokwi.com/)
- CON-06: Se debe hacer la implementación del sistema usando una placa de desarrollo ESP32 S3 y una protoboard

- 
[Enlace a la simulación](https://wokwi.com/projects/445808350528060417)

