/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#include "hc_sr04.h"

hc_sr04::hc_sr04(uword pin_echo_cfg_reg,
                 uword pin_echo_cfg_bit,
                 uword pin_echo_dat_reg,
                 uword pin_echo_dat_bit,
                 uword pin_trigger_cfg_reg,
                 uword pin_trigger_cfg_bit,
                 uword pin_trigger_dat_reg,
                 uword pin_trigger_dat_bit,
                 std::shared_ptr<gpio::cpu> _cpu)
    : pin_echo_cfg_reg(pin_echo_cfg_reg),
      pin_echo_cfg_bit(pin_echo_cfg_bit),
      pin_echo_dat_reg(pin_echo_dat_reg),
      pin_echo_dat_bit(pin_echo_dat_bit),
      pin_trigger_cfg_reg(pin_trigger_cfg_reg),
      pin_trigger_cfg_bit(pin_trigger_cfg_bit),
      pin_trigger_dat_reg(pin_trigger_dat_reg),
      pin_trigger_dat_bit(pin_trigger_dat_bit),
      _cpu (_cpu)
{
    _cpu->write_bits(pin_trigger_cfg_reg, pin_trigger_cfg_bit, P_SELECT_OUTPUT, P_SELECT_LENGTH);
    _cpu->write_bits(pin_echo_cfg_reg, pin_echo_cfg_bit, P_SELECT_INPUT, P_SELECT_LENGTH);
}

short hc_sr04::get_distance()
{
    // start time, end time, signal travel time
    static std::chrono::microseconds start, end, duration;

    _cpu->write_bit(pin_trigger_dat_reg, pin_trigger_dat_bit, HIGH);

    usleep(1000);

    _cpu->write_bit(pin_trigger_dat_reg, pin_trigger_dat_bit, LOW);

    int count_error = 1000000;

    while(!_cpu->read_bit(pin_echo_dat_reg, pin_echo_dat_bit) && count_error-- > 0);

    start = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::system_clock::now().time_since_epoch());

    while(_cpu->read_bit(pin_echo_dat_reg, pin_echo_dat_bit) && count_error-- > 0);

    end = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::system_clock::now().time_since_epoch());

    duration = end - start;

    short distance = static_cast<short>(duration.count() / HC_SR04_COEFFICIENT);

    if(distance > HC_SR04_MAX_DISTANCE)
        distance = HC_SR04_MAX_DISTANCE;

    usleep(100000);

    return distance;
}
