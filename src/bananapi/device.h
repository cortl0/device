/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef DEVICE_H
#define DEVICE_H

#include "brain_friend.h"
#include "../gpio/src/cpu.h"
#include "sensors/hc_sr04.h"

class device
{
    short front_distanse = 0;
    short rear_distanse = 0;
    _word random_array_length_in_power_of_two = 20;
    _word quantity_of_neurons_in_power_of_two = 14;
    _word input_length = 32;
    _word output_length = 4;

    cpu _cpu;

    std::unique_ptr<hc_sr04> hc_sr04_front;
    std::unique_ptr<hc_sr04> hc_sr04_rear;

    std::unique_ptr<brain> brn;
    std::unique_ptr<brain_friend> brn_frnd;

    static void brain_clock_cycle_handler(void* owner);
    void read_sensor_state();
    void write_motor_state();

public:
    device();
    [[noreturn]] void run();
};

#endif // DEVICE_H
