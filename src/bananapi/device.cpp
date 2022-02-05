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
    _cpu.reset(new gpio::cpu(BASE_ADDRESS_GPIO));

    sensors.push_back(
                std::shared_ptr<hc_sr04>(
                    new hc_sr04(
                        CON2_P31_CFG_REG, CON2_P31_CFG_BIT, CON2_P31_DAT_REG, CON2_P31_DAT_BIT,
                        CON2_P29_CFG_REG, CON2_P29_CFG_BIT, CON2_P29_DAT_REG, CON2_P29_DAT_BIT, _cpu)));

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

    std::for_each(sensors.begin(), sensors.end(), [&](std::shared_ptr<hc_sr04>)
    {
        distanses.push_back(0);
    });

    _word random_array_length_in_power_of_two = 24;
    _word quantity_of_neurons_in_power_of_two = 16;

    brain_.reset(new bnn::brain_tools_dev(random_array_length_in_power_of_two,
                                          quantity_of_neurons_in_power_of_two,
                                          input_length,
                                          output_length,
                                          1,
                                          lgr));

    // brain output to arduino input for motors
    _cpu->write_bits(PA_CFG0, PA00_CFG_BIT, P_SELECT_OUTPUT, P_SELECT_LENGTH);
    _cpu->write_bits(PA_CFG0, PA01_CFG_BIT, P_SELECT_OUTPUT, P_SELECT_LENGTH);
    _cpu->write_bits(PA_CFG0, PA02_CFG_BIT, P_SELECT_OUTPUT, P_SELECT_LENGTH);
    _cpu->write_bits(PA_CFG0, PA03_CFG_BIT, P_SELECT_OUTPUT, P_SELECT_LENGTH);
    _cpu->write_bits(PA_DAT, PA00_DAT_BIT, 0b0000, output_length);

    // stop and save button
    _cpu->write_bits(CON2_P10_CFG_REG, CON2_P10_CFG_BIT, P_SELECT_INPUT, P_SELECT_LENGTH);
    _cpu->write_bits(CON2_P10_PUL_REG, CON2_P10_PUL_BIT, P_PULL_DOWN, P_PULL_LENGTH);

    // led count binary neurons init
    _cpu->write_bits(CON2_P07_CFG_REG, CON2_P07_CFG_BIT, P_SELECT_OUTPUT, P_SELECT_LENGTH);
    _cpu->write_bit(CON2_P07_DAT_REG, CON2_P07_DAT_BIT, LOW);

    /*
    // led power
    _cpu.write_bits(CON2_P05_CFG_REG, CON2_P05_CFG_BIT, P_SELECT_OUTPUT, P_SELECT_LENGTH);

    _cpu.write_bits(CON2_P03_CFG_REG, CON2_P03_CFG_BIT, P_SELECT_INPUT, P_SELECT_LENGTH);
    _cpu.write_bits(CON2_P03_PUL_REG, CON2_P03_PUL_BIT, P_PULL_DOWN, P_PULL_LENGTH);
    */
}

void device::brain_clock_cycle_handler(void* owner)
{
    static_cast<device*>(owner)->log_cycle();
}

void device::thread_button(device* d)
{
    while(d)
    {
        if(d->_cpu->read_bit(CON2_P10_DAT_REG, CON2_P10_DAT_BIT))
        {
            d->lgr->log(logger::logger::level::info, LOGGER_DATE_TIME_MESSAGE_PLACE_STRING("button down"));

            d->brain_->stop();
            d->_cpu->write_bits(PA_DAT, PA00_DAT_BIT, 0b0000, d->output_length);
            d->brain_->save(d->lgr);

            while(d->_cpu->read_bit(CON2_P10_DAT_REG, CON2_P10_DAT_BIT));

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
            if(d->brain_->get_quantity_of_initialized_neurons_binary() & (1 << i))
                pow_two = i;

        for (_word i = 0; i < pow_two && !d->stop; i++)
        {
            d->_cpu->write_bit(CON2_P07_DAT_REG, CON2_P07_DAT_BIT, HIGH);
            usleep(250000);

            d->_cpu->write_bit(CON2_P07_DAT_REG, CON2_P07_DAT_BIT, LOW);
            usleep(250000);
        }

        sleep(1);

        /*
        // led power
        d->_cpu.write_bit(CON2_P05_DAT_REG, CON2_P05_DAT_BIT,
                          d->_cpu.read_bit(CON2_P03_DAT_REG, CON2_P03_DAT_BIT));
        */
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

        lgr->log(logger::logger::level::trace, LOGGER_DATE_TIME_MESSAGE_PLACE_STRING(brain_->get_state() + "\t" + s));
        count = 0;
    }
}

void device::run()
{
    _cpu->write_bits(PA_DAT, PA00_DAT_BIT, 0b0000, output_length);

    brain_->load(lgr);

    brain_->start();

    lgr->log(logger::logger::level::info, LOGGER_DATE_TIME_MESSAGE_PLACE_STRING("device started"));

    std::thread b(thread_button, this);
    b.detach();

    std::thread l(thread_led, this);
    l.detach();

    std::thread m(write_motor_state, this);
    m.detach();

    read_sensor_state();

    sleep(1);

    lgr->log(logger::logger::level::info, LOGGER_DATE_TIME_MESSAGE_PLACE_STRING("device stopped"));
}

void device::read_sensor_state()
{
    _word byte_length = 8;

    _word i;

    _word c = input_length / (byte_length * static_cast<_word>(distanses.size()));

    c *= byte_length * static_cast<_word>(distanses.size());

    auto it = distanses.begin();

    short temp;

    while(!stop)
    {
        i = 0;

        it = distanses.begin();

        std::for_each(sensors.begin(), sensors.end(), [&](std::shared_ptr<hc_sr04> sensor)
        {
            temp = sensor->get_distance();
            if(temp)
                *(it) = temp;
            it++;
        });

        while(i + distanses.size() <= c)
            std::for_each(distanses.begin(), distanses.end(), [&](short d)
            {
                for(_word j = 0; j < byte_length; j++)
                    brain_->set_input(i++, (d >> j) & 1);
            });
    }
}

void device::write_motor_state(device* d)
{
    uword value;

    while(!d->stop)
    {
        value = 0;

        for(unsigned int i = 0; i < d->output_length; i++)
            value |= static_cast<uword>(d->brain_->get_output(i)) << i;

        d->_cpu->write_bits(PA_DAT, PA00_DAT_BIT, value, d->output_length);

        usleep(100000);
    }
}
