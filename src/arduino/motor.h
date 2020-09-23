/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef MOTOR_H
#define MOTOR_H

#define int_max 4294967295

#include "simple_math.h"

class motor
{
  int velosity = 0;                            // [step/sec]       default = 0
  int velosity_max;                            // [step/sec]       default = 256   max = 2048    not recommended above 2048
  int acceleration;                            // [step/sec/sec]   default = 64
  int acceleration_max;                        // [step/sec/sec]   default = 128
  int acceleration_run_out;                    // [step/sec/sec]   default = 32
  const unsigned long coeff = 1000000;         // [sec/microsec]   default = 1000000
  unsigned long next_step_time_velosity = 0;      // [microsec]
  unsigned long next_step_time_acceleration = 0;  // [microsec]
  const int aspect = 256; // aspect for choose amoumt substeps
  int bit_enable = 0;
  int bit_direction = 2;
  int bit_step = 1;
  int ms_current = 4;
  int ms_array[5] = {
    // substeps per step angle
    // pins: MS3, MS2, MS1
    0b000, // one steps; ms_current = 0
    0b001, // two steps; ms_current = 1
    0b010, // four steps; ms_current = 2
    0b011, // eighth steps; ms_current = 3
    0b111  // sixteen steps; ms_current = 4
  };
  int enable = 0;
  int direction_ = 0;
  int step_ = 0;
  unsigned long temp; // temp variable for refrash_state()
  unsigned long time_micros_old = 0;
  unsigned long delta;
  unsigned long time = 0;

public:
  motor(int velosity_max, int acceleration, int acceleration_run_out);
  int pin_config();
  int get_state();
  void refresh_velosity(int acceleration, int factor);
  void refrash_state(unsigned long timeMicros, int controlValue);
};

#endif //MOTOR_H

