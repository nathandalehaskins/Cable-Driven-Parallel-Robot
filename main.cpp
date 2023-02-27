#include <AccelStepper.h>
#include <MultiStepper.h>
#include "motor_pins.h"
#include "math.h"
#include <string.h>

int speed = 500; // steps/s
int acceleration = 0; // steps/s^2

const int stepsPerRevolution = 200; // varies based on motor
double section_length = (15 * M_PI) / stepsPerRevolution;

// positions of pulleys relative (0,0,0) in the environment
double P1[3] = {0, 0, 400}; // in mm
double P2[3] = {0, 455, 400};
double P3[3] = {990, 455, 400};
double P4[3] = {990, 0, 400};

bool reel_state1, reel_state2, reel_state3, reel_state4;
double step1, step2, step3, step4;
long positions[4]; 
const unsigned int MAX_MESSAGE_LENGTH = 12; 

MultiStepper steppers; 

void multistepper_init(){
  steppers.addStepper(M1);
  steppers.addStepper(M2);
  steppers.addStepper(M3);
  steppers.addStepper(M4);  
}

double distance(double first_point[3], double second_point[3]){
    double xDiff = second_point[0] - first_point[0];
    double yDiff = second_point[1] - first_point[1];
    double zDiff = second_point[2] - first_point[2];
    return sqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);
}

void updateDistances(double P1[3], double P2[3], double P3[3], 
                     double P4[3], double cur_pos[3], 
                     double new_pos[3], double section_length){
double points[4][3] = {{P1[0], P1[1], P1[2]}, 
                       {P2[0], P2[1], P2[2]}, 
                       {P3[0], P3[1], P3[2]}, 
                       {P4[0], P4[1], P4[2]}};
    double old_distances[4];
    double new_distances[4];
    for(int i = 0; i < 4; i++) {
        old_distances[i] = distance(points[i], cur_pos);
        new_distances[i] = distance(points[i], new_pos);
    }
    
    step1 = abs(new_distances[0]-old_distances[0])/section_length;
    step2 = abs(new_distances[1]-old_distances[1])/section_length;
    step3 = abs(new_distances[2]-old_distances[2])/section_length;
    step4 = abs(new_distances[3]-old_distances[3])/section_length;

    reel_state1 = old_distances[0] > new_distances[0];
    reel_state2 = old_distances[1] > new_distances[1];
    reel_state3 = old_distances[2] > new_distances[2];
    reel_state4 = old_distances[3] > new_distances[3];

}

void steppers_move(){

  positions[0] = reel_state1 ? step1 : -1 * step1;
  positions[1] = reel_state2 ? step2 : -1 * step2;
  positions[2] = reel_state3 ? step3 : -1 * step3;
  positions[3] = reel_state4 ? step4 : -1 * step4;

  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); 

  M1.setCurrentPosition(0);
  M2.setCurrentPosition(0);
  M3.setCurrentPosition(0);
  M4.setCurrentPosition(0);

}

void motor_parameters(){
  M1.setMaxSpeed(speed);
  M2.setMaxSpeed(speed);
  M3.setMaxSpeed(speed);
  M4.setMaxSpeed(speed);
  M1.setAcceleration(acceleration);
  M2.setAcceleration(acceleration);
  M3.setAcceleration(acceleration);
  M4.setAcceleration(acceleration);
}

/* the following function was modified from: 
https://www.programmingelectronics.com/serial-read/ 
and as such is not entirely my work.*/
void get_coordinates(double coordinates[3]) {
  
  static char message[MAX_MESSAGE_LENGTH];
  static unsigned int message_pos = 0;
  while(true) {
    if (Serial.available() > 0){
      char inByte = Serial.read();
      if (inByte !='\n' && (message_pos < MAX_MESSAGE_LENGTH-1) ){    
        message[message_pos] = inByte;
        message_pos++;
      }
      else{
        message[message_pos] = '\0';
        int i = 0;
        char *p = strtok(message, " "); 
        while (p != NULL){
          coordinates[i] = atof(p); 
          i++;
          p = strtok(NULL, " ");
        }
        
        message_pos = 0;
        break; 
      }
    }
  }
}

double cur_pos[3];

void setup() {
  Serial.begin(9600);
  motor_parameters();
  multistepper_init();

  Serial.println("Enter current position coordinates (x y z): ");
  get_coordinates(cur_pos); // 495 228 80 = my starting pos
}

void loop() {
  
  double new_pos[3];

  Serial.println("Enter new position coordinates (x y z): ");
  get_coordinates(new_pos); // 200 200 200 = new pos
  
  updateDistances(P1,P2,P3,P4,cur_pos,new_pos,section_length);
  steppers_move();

  cur_pos[0] = new_pos[0];
  cur_pos[1] = new_pos[1];
  cur_pos[2] = new_pos[2];
}
