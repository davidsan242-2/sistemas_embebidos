# 1. Challenge-1 (Simulación e implementación)

Design and implement an embedded system that controls a sequence of four LEDs using a single push button.\
The system should allow the user to start, stop, and resume the sequence with the same button.\
The system must meet the following conditions:\
- When the button is pressed for the first time, the four LEDs should start blinking one after another from left to right, changing every one second (1 s).\
- When the button is pressed again, the sequence should pause at its current position.\
- When the button is pressed a third time, the sequence should resume from where it stopped.\
- The behavior should be cyclic: every press of the button toggles between “running” and “stopped” states.
