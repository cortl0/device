/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#include "device.h"

device::device()
{
    hc_sr04_front.reset(new hc_sr04(CON2_P31_CFG_REG, CON2_P31_CFG_BIT, CON2_P31_DAT_REG, CON2_P31_DAT_BIT,
                                    CON2_P29_CFG_REG, CON2_P29_CFG_BIT, CON2_P29_DAT_REG, CON2_P29_DAT_BIT, _cpu));

    hc_sr04_rear.reset(new hc_sr04(CON2_P35_CFG_REG, CON2_P35_CFG_BIT, CON2_P35_DAT_REG, CON2_P35_DAT_BIT,
                                   CON2_P33_CFG_REG, CON2_P33_CFG_BIT, CON2_P33_DAT_REG, CON2_P33_DAT_BIT, _cpu));

    _word random_array_length_in_power_of_two = 20;
    _word quantity_of_neurons_in_power_of_two = 12;
    _word input_length = 1024;
    _word output_length = 4;

    brn.reset(new brain(random_array_length_in_power_of_two,
                        quantity_of_neurons_in_power_of_two,
                        input_length,
                        output_length,
                        brain_clock_cycle_handler));

    brn_frnd.reset(new brain_friend(*brn.get()));

    // brain output to arduino input for motors
    _cpu.write_bits(PA_CFG0, PA00_CFG_BIT, P_SELECT_OUTPUT, P_SELECT_LENGTH);
    _cpu.write_bits(PA_CFG0, PA01_CFG_BIT, P_SELECT_OUTPUT, P_SELECT_LENGTH);
    _cpu.write_bits(PA_CFG0, PA02_CFG_BIT, P_SELECT_OUTPUT, P_SELECT_LENGTH);
    _cpu.write_bits(PA_CFG0, PA03_CFG_BIT, P_SELECT_OUTPUT, P_SELECT_LENGTH);
    _cpu.write_bits(PA_DAT, PA00_DAT_BIT, 0b0000, brn->get_output_length());

    // stop and save button
    _cpu.write_bits(CON2_P10_CFG_REG, CON2_P10_CFG_BIT, P_SELECT_INPUT, P_SELECT_LENGTH);
    _cpu.write_bits(CON2_P10_PUL_REG, CON2_P10_PUL_BIT, P_PULL_DOWN, P_PULL_LENGTH);

    // led count binary neurons init
    _cpu.write_bits(CON2_P07_CFG_REG, CON2_P07_CFG_BIT, P_SELECT_OUTPUT, P_SELECT_LENGTH);
    _cpu.write_bit(CON2_P07_DAT_REG, CON2_P07_DAT_BIT, LOW);
}

cpu& device::get_cpu()
{
    return _cpu;
}

uword device::get_quantity_of_initialized_neurons_binary()
{
    return brn_frnd->get_quantity_of_initialized_neurons_binary();
}

void device::brain_clock_cycle_handler(void* owner)
{
    static_cast<device*>(owner)->log_cycle();

    static_cast<device*>(owner)->write_motor_state();

    static_cast<device*>(owner)->read_sensor_state();
}

void device::button_thread_func(device* owner)
{
    while(owner)
    {
        if(owner->_cpu.read_bit(CON2_P10_DAT_REG, CON2_P10_DAT_BIT))
        {
            owner->brn_frnd->stop();
            owner->_cpu.write_bits(PA_DAT, PA00_DAT_BIT, 0b0000, owner->brn->get_output_length());
            owner->brn_frnd->save();

            owner->stop = true;

            return;
        }

        sleep(1);
    }
}

static void led_work(device* d, bool* stop)
{
    while(!(*stop))
    {
        for (_word i = 0; i < sizeof(_word) * 8 && !stop; i++)
        {
            if((d->get_quantity_of_initialized_neurons_binary() & (1 << i)) == LOW)
                d->get_cpu().write_bit(CON2_P07_DAT_REG, CON2_P07_DAT_BIT, LOW);
            else {
                d->get_cpu().write_bit(CON2_P07_DAT_REG, CON2_P07_DAT_BIT, HIGH);
            }
            usleep(250000);
        }
        d->get_cpu().write_bit(CON2_P07_DAT_REG, CON2_P07_DAT_BIT, LOW);
        sleep(1);
        d->get_cpu().write_bit(CON2_P07_DAT_REG, CON2_P07_DAT_BIT, HIGH);
        sleep(1);
        d->get_cpu().write_bit(CON2_P07_DAT_REG, CON2_P07_DAT_BIT, LOW);
        sleep(1);
    }
}

void device::log_cycle()
{
    static int count = 0;

    if(++count >= 0)
    {
        lgr.logging(logger::log_level_trace, "device::log_cycle()", brn_frnd->get_state());
        count = 0;
    }
}

void device::run()
{
    brn_frnd->load();

    brn->start(this);

    button_thread.reset(new std::thread(button_thread_func, this));

    button_thread->detach();

    auto g = new std::thread(led_work, this, &stop);
    g->detach();

    while(!stop)
    {
        hc_sr04_front->run();
        front_distanse = hc_sr04_front->get_distance();

        hc_sr04_rear->run();
        rear_distanse = hc_sr04_rear->get_distance();
    }
}

void device::read_sensor_state()
{
    static uword value_front = 0;
    static uword value_rear = 0;

    if(front_distanse != 0)
    {
        value_front &= static_cast<uword>(front_distanse);
        value_front |= static_cast<uword>(front_distanse);
    }

    if(rear_distanse != 0)
    {
        value_rear &= static_cast<uword>(rear_distanse);
        value_rear |= static_cast<uword>(rear_distanse);
    }

    static uword value;
    value = value_front + (value_rear << 16);

    for(unsigned int i = 0; i < 32; i++)
        for(unsigned int j = 0; j < 32; j++)
            brn->set_in(i * 32 + j, (value >> j) & 1);
}

void device::write_motor_state()
{
    static uword value;
    value = 0;

    for(unsigned int i = 0; i < brn->get_output_length(); i++)
        value |= static_cast<uword>(brn->get_out(i)) << i;

    _cpu.write_bits(PA_DAT, PA00_DAT_BIT, value, brn->get_output_length());
}
