/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef CHASSIS_H
#define CHASSIS_H

#include "Arduino.h"
#include "control_time_step.h"
#include "control_remote.h"
#include "motor.h"

#define CHASSIS_left_free       0b00
#define CHASSIS_left_keep       0b11
#define CHASSIS_left_forward    0b10
#define CHASSIS_left_back       0b01

#define CHASSIS_right_free      0b00
#define CHASSIS_right_keep      0b11
#define CHASSIS_right_forward   0b01
#define CHASSIS_right_back      0b10

class chassis
{
  unsigned long time_current = micros();
  control *cntrl;
  motor motor_left = motor(32, 8, 4);
  motor motor_right = motor(32, 8, 4);

public:
  chassis();
  void refrash_state();
};

#endif CHASSIS_H

