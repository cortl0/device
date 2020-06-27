/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#include "chassis.h"

chassis::chassis()
{
  DDRB = DDRB | motor_left.pin_config();
  DDRD = DDRD | motor_right.pin_config() << 2;
  PORTB = PORTB & B11000000;
  PORTD = PORTD & B11;
  cntrl = new control_time_step();
  //cntrl = new control_remote();
}

void chassis::refrash_state()
{
  time_current = micros();

  int left = 0;
  int right = 0;

  cntrl->get_state(left, right);

  motor_left.refrash_state(time_current, left);
  motor_right.refrash_state(time_current, right);

  int state = motor_left.get_state() | (motor_right.get_state() << 6);
  int state_left = state & B111111;
  int state_right = ((state >> 6) & B111111) << 2;
  
  PORTB = (PORTB & B11000000) | state_left;
  PORTD = (PORTD & B11) | state_right;
}

