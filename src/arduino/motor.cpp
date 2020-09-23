/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#include "motor.h"

motor::motor(int velosity_max, int acceleration, int acceleration_run_out) : 
velosity_max(velosity_max), acceleration(acceleration), acceleration_run_out(acceleration_run_out)
{

}

int motor::pin_config()
{
  return (1 << bit_enable)
    | (1 << bit_direction)
      | (1 << bit_step)
        | (0b111 << 3);
}

int motor::get_state()
{
  return (enable << bit_enable)
    | (direction_ << bit_direction)
      | (step_ << bit_step)
        | (ms_array[ms_current] << 3);
}

void motor::refresh_velosity(int acceleration, int factor)
{
  velosity += acceleration * factor;
  if (velosity > velosity_max) velosity = velosity_max;
  if (velosity < -velosity_max) velosity = -velosity_max;
  if (velosity >= 0) direction_ = 0;
  if (velosity < 0) direction_ = 1;
  ms_current = 0;
  if (simple_math::Abs(velosity) < aspect * 16) ms_current = 0;
  if (simple_math::Abs(velosity) < aspect * 8) ms_current = 1;
  if (simple_math::Abs(velosity) < aspect * 4) ms_current = 2;
  if (simple_math::Abs(velosity) < aspect * 2) ms_current = 3;
  if (simple_math::Abs(velosity) < aspect * 1) ms_current = 4;
}

void motor::refrash_state(unsigned long time_micros, int controlValue)
{
  delta = time_micros - time_micros_old;

  if(time_micros_old > time_micros)
  {
    delta = int_max - time_micros_old;
    delta += time_micros;
  }

  time_micros_old = time_micros;

  time += delta;

  if (controlValue == 0)
  {
    temp = (coeff / acceleration_run_out);
    if (time > next_step_time_acceleration + temp)
    {
      next_step_time_acceleration += temp;
      refresh_velosity(1, -simple_math::sign0(velosity));
    }
  }
  else
  {
    temp = (coeff / acceleration);
    if (time > next_step_time_acceleration + temp)
    {
      next_step_time_acceleration += temp;
      refresh_velosity(1, (controlValue & 1) - (controlValue >> 1));
    }
  }
  
  if (velosity == 0)
    next_step_time_velosity = time;
  else
  {
    temp = (coeff / simple_math::Abs(velosity)) / (1 << ms_current);
    if (time > next_step_time_velosity + temp)
    {
      next_step_time_velosity += temp;
      step_ = !step_;
    }
  }

  temp = int_max;
  
  if(temp > next_step_time_acceleration)
    temp = next_step_time_acceleration;
  if(temp > next_step_time_velosity)
    temp = next_step_time_velosity;      
  if(temp > time)
    temp = time;
    
  next_step_time_acceleration -= temp;
  next_step_time_velosity -= temp;
  time -= temp;
}



