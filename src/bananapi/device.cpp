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
    /*
    hc_sr04_front.push_back(
                std::shared_ptr<hc_sr04>(
                    new hc_sr04(
                        CON2_P31_CFG_REG, CON2_P31_CFG_BIT, CON2_P31_DAT_REG, CON2_P31_DAT_BIT,
                        CON2_P29_CFG_REG, CON2_P29_CFG_BIT, CON2_P29_DAT_REG, CON2_P29_DAT_BIT, _cpu)));
*/

    sensors.push_back(
                std::shared_ptr<hc_sr04>(
                    new hc_sr04(
                        CON2_P35_CFG_REG, CON2_P35_CFG_BIT, CON2_P35_DAT_REG, CON2_P35_DAT_BIT,
                        CON2_P33_CFG_REG, CON2_P33_CFG_BIT, CON2_P33_DAT_REG, CON2_P33_DAT_BIT, _cpu)));

    sensors.push_back(
                std::shared_ptr<hc_sr04>(
                    new hc_sr04(
                        CON2_P37_CFG_REG, CON2_P37_CFG_BIT, CON2_P37_DAT_REG, CON2_P37_DAT_BIT,
                        CON2_P27_CFG_REG, CON2_P27_CFG_BIT, CON2_P27_DAT_REG, CON2_P27_DAT_BIT, _cpu)));

    _word random_array_length_in_power_of_two = 24;
    _word quantity_of_neurons_in_power_of_two = 16;
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

    // led power
    _cpu.write_bits(CON2_P05_CFG_REG, CON2_P05_CFG_BIT, P_SELECT_OUTPUT, P_SELECT_LENGTH);

    _cpu.write_bits(CON2_P03_CFG_REG, CON2_P03_CFG_BIT, P_SELECT_INPUT, P_SELECT_LENGTH);
    _cpu.write_bits(CON2_P03_PUL_REG, CON2_P03_PUL_BIT, P_PULL_DOWN, P_PULL_LENGTH);
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

        //        // led power
        //        d->_cpu.write_bit(CON2_P05_DAT_REG, CON2_P05_DAT_BIT,
        //                          d->_cpu.read_bit(CON2_P03_DAT_REG, CON2_P03_DAT_BIT));
    }
}

void device::log_cycle()
{
    static int count = 0;

    if(++count >= 128)
    {
        std::string s;

        std::for_each(distanses.begin(), distanses.end(), [&](short d)
        {
            s += std::to_string(d) + "\t";
        });

        lgr->logging(logger::log_level_trace, "device::log_cycle()", brn_frnd->get_state() + "\t" + s);
        count = 0;
    }
}

void device::run()
{
    _cpu.write_bits(PA_DAT, PA00_DAT_BIT, 0b0000, brn->get_output_length());

    brn_frnd->load(lgr);

    brn->start(this);

    button_thread.reset(new std::thread(thread_button, this));

    button_thread->detach();

    auto g = new std::thread(thread_led, this);
    g->detach();

    lgr->logging(logger::log_level_information, "device::run()", "device started");

    std::for_each(sensors.begin(), sensors.end(), [&](std::shared_ptr<hc_sr04>)
    {
        distanses.push_back(0);
    });

    auto it = distanses.begin();

    short temp;

    while(!stop)
    {
        it = distanses.begin();

        std::for_each(sensors.begin(), sensors.end(), [&](std::shared_ptr<hc_sr04> sensor)
        {
            sensor->run();
            temp = sensor->get_distance();
            if(temp)
                *(it++) = temp;
        });
    }

    lgr->logging(logger::log_level_information, "device::run()", "device stopped");
}

void device::read_sensor_state()
{
    _word length = 8;

    _word i = 0;

    _word c = 1024 / (length * static_cast<_word>(distanses.size()));

    c -= 1;

    c *= length * static_cast<_word>(distanses.size());

    while(i < c)
        std::for_each(distanses.begin(), distanses.end(), [&](short d)
        {
            for(unsigned int j = 0; j < length; j++)
                brn->set_in(i++, (d >> j) & 1);
        });
}

void device::write_motor_state()
{
    static uword value;
    value = 0;

    for(unsigned int i = 0; i < brn->get_output_length(); i++)
        value |= static_cast<uword>(brn->get_out(i)) << i;

    _cpu.write_bits(PA_DAT, PA00_DAT_BIT, value, brn->get_output_length());
}
