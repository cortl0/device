/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef HC_SR04_H
#define HC_SR04_H

#include <chrono>

#include <unistd.h>

#include "../../gpio/src/board/bpi_p2_zero/bpi_p2_zero.h"

#define HC_SR04_SPEED_OF_SOUND_IN_AIR 343 // [m/s] at 20'C
#define HC_SR04_CORRECTION_FACTOR 25 // %
#define HC_SR04_COEFFICIENT (HC_SR04_SPEED_OF_SOUND_IN_AIR * 2 / 10 * 100 / (100 + HC_SR04_CORRECTION_FACTOR)) // [m/s]
#define HC_SR04_MAX_DISTANCE 127 // [cm]

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

    // distance in centimeters
    short distance;

    gpio::cpu& _cpu;

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
            gpio::cpu& _cpu);
    
    // get the distance in centimeters
    short get_distance();

    void run();
};

#endif // HC_SR04_H
