#include <AccelStepper.h>
#ifndef MOTOR_PINS_H
#define MOTOR_PINS_H

#define M1_DIR_PIN 4
#define M1_STEP_PIN 3
#define M2_DIR_PIN 6
#define M2_STEP_PIN 5
#define M3_DIR_PIN 8
#define M3_STEP_PIN 7
#define M4_DIR_PIN 10
#define M4_STEP_PIN 9

AccelStepper M1(1, M1_STEP_PIN, M1_DIR_PIN);
AccelStepper M2(1, M2_STEP_PIN, M2_DIR_PIN);
AccelStepper M3(1, M3_STEP_PIN, M3_DIR_PIN);
AccelStepper M4(1, M4_STEP_PIN, M4_DIR_PIN);

#endif
