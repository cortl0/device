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

#ifdef hc_sr04_rear_on
    hc_sr04_rear.reset(new hc_sr04(CON2_P35_CFG_REG, CON2_P35_CFG_BIT, CON2_P35_DAT_REG, CON2_P35_DAT_BIT,
                                   CON2_P33_CFG_REG, CON2_P33_CFG_BIT, CON2_P33_DAT_REG, CON2_P33_DAT_BIT, _cpu));
#endif

    _word random_array_length_in_power_of_two = 21;
    _word quantity_of_neurons_in_power_of_two = 13;
    _word input_length = 1024;
    _word output_length = 4;

    brn.reset(new bnn::brain(random_array_length_in_power_of_two,
                        quantity_of_neurons_in_power_of_two,
                        input_length,
                        output_length,
                        brain_clock_cycle_handler));

    brn_frnd.reset(new bnn::brain_friend(*brn.get(), lgr));

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

void device::brain_clock_cycle_handler(void* owner)
{
    static_cast<device*>(owner)->log_cycle();

    static_cast<device*>(owner)->write_motor_state();

    static_cast<device*>(owner)->read_sensor_state();
}

void device::thread_button(device* d)
{
    while(d)
    {
        if(d->_cpu.read_bit(CON2_P10_DAT_REG, CON2_P10_DAT_BIT))
        {
            d->lgr->logging(logger::log_level_information, "device::thread_button()", "button down");

            d->brn_frnd->stop();
            d->_cpu.write_bits(PA_DAT, PA00_DAT_BIT, 0b0000, d->brn->get_output_length());
            d->brn_frnd->save(d->lgr);

            while(d->_cpu.read_bit(CON2_P10_DAT_REG, CON2_P10_DAT_BIT));

            sleep(1);

            system("shutdown 0");

            d->stop = true;

            return;
        }

        sleep(1);
    }
}

void device::thread_led(device* d)
{
    _word pow_two;

    while(!(d->stop))
    {
        pow_two = 0;

        for (_word i = 0; i < sizeof(_word) * 8 && !d->stop; i++)
            if(d->brn_frnd->get_quantity_of_initialized_neurons_binary() & (1 << i))
                pow_two = i;

        for (_word i = 0; i < pow_two && !d->stop; i++)
        {
            d->_cpu.write_bit(CON2_P07_DAT_REG, CON2_P07_DAT_BIT, HIGH);
            usleep(250000);

            d->_cpu.write_bit(CON2_P07_DAT_REG, CON2_P07_DAT_BIT, LOW);
            usleep(250000);
        }

        sleep(1);
    }
}

void device::log_cycle()
{
    static int count = 0;

    if(++count >= 128)
    {
        lgr->logging(logger::log_level_trace, "device::log_cycle()", brn_frnd->get_state());
        count = 0;
    }
}

void device::run()
{
    brn_frnd->load(lgr);

    brn->start(this);

    button_thread.reset(new std::thread(thread_button, this));

    button_thread->detach();

    auto g = new std::thread(thread_led, this);
    g->detach();

    lgr->logging(logger::log_level_information, "device::run()", "device started");

    while(!stop)
    {
        hc_sr04_front->run();
        front_distanse = hc_sr04_front->get_distance();

#ifdef hc_sr04_rear_on
        hc_sr04_rear->run();
        rear_distanse = hc_sr04_rear->get_distance();
#endif
    }

    lgr->logging(logger::log_level_information, "device::run()", "device stopped");
}

void device::read_sensor_state()
{
    static uword value_front = 0;

#ifdef hc_sr04_rear_on
    static uword value_rear = 0;
#endif

    if(front_distanse != 0)
    {
        value_front &= static_cast<uword>(front_distanse);
        value_front |= static_cast<uword>(front_distanse);
    }

#ifdef hc_sr04_rear_on
    if(rear_distanse != 0)
    {
        value_rear &= static_cast<uword>(rear_distanse);
        value_rear |= static_cast<uword>(rear_distanse);
    }
#endif

    static uword value;

#ifdef hc_sr04_rear_on
    value = value_front + (value_rear << 16);

    for(unsigned int i = 0; i < 32; i++)
        for(unsigned int j = 0; j < 32; j++)
            brn->set_in(i * 32 + j, (value >> j) & 1);
#else
    value = value_front;

    for(unsigned int i = 0; i < 64; i++)
        for(unsigned int j = 0; j < 16; j++)
            brn->set_in(i * 16 + j, (value >> j) & 1);
#endif
}

void device::write_motor_state()
{
    static uword value;
    value = 0;

    for(unsigned int i = 0; i < brn->get_output_length(); i++)
        value |= static_cast<uword>(brn->get_out(i)) << i;

    _cpu.write_bits(PA_DAT, PA00_DAT_BIT, value, brn->get_output_length());
}
