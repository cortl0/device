#ifndef DEVICE_H
#define DEVICE_H

#include "brain_friend.h"
#include "../gpio/src/cpu.h"

class device
{
    _word random_array_length_in_power_of_two = 16;
    _word quantity_of_neurons_in_power_of_two = 16;
    _word input_length = 32;
    _word output_length = 4;

    std::unique_ptr<brain> brn;
    std::unique_ptr<brain_friend> brn_frnd;

    cpu _cpu;

    static void brain_clock_cycle_handler(void* owner);
    void write_motor_state();

public:
    device();
    void start();
};

#endif // DEVICE_H
