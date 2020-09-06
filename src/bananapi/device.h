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
#include "../logger/src/logger.h"
#include "sensors/hc_sr04.h"

class device
{
    short front_distanse = 0;
    short rear_distanse = 0;

    bool stop = false;

    cpu _cpu;

    std::unique_ptr<hc_sr04> hc_sr04_front;
    std::unique_ptr<hc_sr04> hc_sr04_rear;

    std::unique_ptr<brain> brn;
    std::unique_ptr<brain_friend> brn_frnd;
    std::unique_ptr<std::thread> button_thread;

    logger::logger lgr = logger::logger(logger::log_level_trace, "log.txt");

    static void brain_clock_cycle_handler(void* owner);
    static void button_thread_func(device* owner);
    void read_sensor_state();
    void write_motor_state();

public:
    device();
    cpu& get_cpu();
    uword get_quantity_of_initialized_neurons_binary();
    void log_cycle();
    void run();
};

#endif // DEVICE_H
