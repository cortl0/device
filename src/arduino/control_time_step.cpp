/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#include "control_time_step.h"

control_time_step::~control_time_step()
{

}

control_time_step::control_time_step()
{

}

void control_time_step::algorithm_00(int& left, int& right) {
  time = (micros() - time_start) / 1000000;
  switch (time)
  {
  case 0:

    break;
  case 1:

    break;
  case 2:
    left = lf;
    right = rf;
    break;
  case 3:
    left = lf;
    right = rf;
    break;
  case 4:
    right = rf;
    break;
  case 5:
    right = rf;
    break;
  case 6:
    left = lf;
    break;
  case 7:
    left = lf;
    break;
  case 8:

    break;
  case 9:
    left = lr;
    right = rr;
    break;
  case 10:
    left = lr;
    right = rr;
    break;
  case 11:

    break;
  case 12:
    left = lr;
    right = rf;
    break;
  default:
    time_start = micros();
    break;
  }
}

void control_time_step::get_state(int& left, int& right)
{
  algorithm_00(left, right);
}

