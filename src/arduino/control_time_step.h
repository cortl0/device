/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef CONTROL_TIME_STEP_H
#define CONTROL_TIME_STEP_H

#include "control.h"

#define lf B0010;
#define lr B0001;
#define rf B0100;
#define rr B1000;

class control_time_step : 
public control
{
  unsigned long time_start = micros();
  void algorithm_00(int& left, int& right);
  int time = (micros() - time_start) / 1000000;
public:
  virtual ~control_time_step();
  control_time_step();
  void get_state(int& left, int& right);
};

#endif //CONTROL_TIME_STEP_H

