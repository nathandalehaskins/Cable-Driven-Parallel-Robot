# Cable-Driven-Parallel-Robot
Program to control a Cable Driven Parallel Robot.
Basic functionality of the program:

- Firstly, prompts user for the current position of end effector e.g. (100,100,0) in mm.
- Secondly, prompts user for next position of end effector e.g. (200,300,100) in mm.
- End effector moves to position.
- Prompts user for new position.
- etc.

To replicate my setup, the parts you will need:
1. 4 x 48 mm Nema 17 motors
2. 4 x a4988 stepper motor driver modules.
3. 1 x Arduino UNO / ESP 32 dev board.
4. 12 V power supply. 
5. 5 V power supply (and or voltage regulated from 12 V supply).
6. 4 x 100uF decoupling caps to protect motor driver modules. 
7. 4 x winches connected to motors (M1,M2,M3,M4)
8. 4 x Pulleys. 
