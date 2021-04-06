/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef DEVICE_H
#define DEVICE_H

#include <stdexcept>
#include <stdlib.h>
#include <algorithm>
#include <experimental/filesystem>
#include <vector>

#include "brain_friend_dev.h"
#include "../gpio/src/board/bpi_p2_zero/bpi_p2_zero.h"
#include "../logger/src/logger.h"
#include "sensors/hc_sr04.h"

namespace fs = std::experimental::filesystem;

class device
{
    _word input_length = 1024;
    _word output_length = 4;

    volatile bool stop = false;

    std::shared_ptr<gpio::cpu> _cpu;

    std::vector<std::shared_ptr<hc_sr04>> sensors;

    std::vector<short> distanses;

    std::unique_ptr<bnn::brain> brn;
    std::unique_ptr<bnn::brain_friend_dev> brn_frnd;

    std::shared_ptr<logger::logger> lgr = std::shared_ptr<logger::logger>(new logger::logger(logger::log_level_trace, (fs::current_path() / "log.txt").string()));

    static void brain_clock_cycle_handler(void* owner);
    static void thread_button(device*);
    static void thread_led(device*);
    void read_sensor_state();
    static void write_motor_state(device* d);

public:
    device();
    void log_cycle();
    void run();
};

#endif // DEVICE_H
