/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef HC_SR04_H
#define HC_SR04_H

#include "../../gpio/src/cpu.h"

#include <unistd.h>
#include <chrono>

#define HC_SR04_SPEED_OF_SOUND_IN_AIR 343 // [m/s] at 20'C
#define HC_SR04_CORRECTION_FACTOR 25 // %
#define HC_SR04_COEFFICIENT (HC_SR04_SPEED_OF_SOUND_IN_AIR * 2 / 10 * 100 / (100 + HC_SR04_CORRECTION_FACTOR)) // [m/s]

class hc_sr04
{
    uword pin_echo_cfg_reg;
    uword pin_echo_cfg_bit;
    uword pin_echo_dat_reg;
    uword pin_echo_dat_bit;

    uword pin_trigger_cfg_reg;
    uword pin_trigger_cfg_bit;
    uword pin_trigger_dat_reg;
    uword pin_trigger_dat_bit;

    cpu& _cpu;

    std::chrono::microseconds start, end;

    // signal travel time
    std::chrono::microseconds duration;
    
    // distance in centimeters
    short distance = 0;
public:
    enum
    {
        hc_sr04_state_not_new_data = 0,
        hc_sr04_state_in_work = 1,
        hc_sr04_state_allow_new_data = 2,
    } state;

    hc_sr04() = delete;

    hc_sr04(uword pin_echo_cfg_reg, uword pin_echo_cfg_bit,
            uword pin_echo_dat_reg, uword pin_echo_dat_bit,
            uword pin_trigger_cfg_reg, uword pin_trigger_cfg_bit,
            uword pin_trigger_dat_reg, uword pin_trigger_dat_bit,
            cpu& _cpu);

    void run();
    
    // get the distance in centimeters
    short get_distance();
};

#endif // HC_SR04_H
