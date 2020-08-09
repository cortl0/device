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

    brn.reset(new brain(random_array_length_in_power_of_two,
                        quantity_of_neurons_in_power_of_two,
                        input_length,
                        output_length,
                        brain_clock_cycle_handler));

    brn_frnd.reset(new brain_friend(*brn.get()));

    _cpu.write_bits(PA_DAT, PA00_DAT_BIT, 0b0000, brn->get_output_length());
}

void device::brain_clock_cycle_handler(void* owner)
{
    static_cast<device*>(owner)->write_motor_state();
    static_cast<device*>(owner)->read_sensor_state();
}

void device::run()
{
    brn->start(this);

    while(1)
    {
        hc_sr04_front->run();
        front_distanse = hc_sr04_front->get_distance();

        hc_sr04_rear->run();
        rear_distanse = hc_sr04_rear->get_distance();

#if DEBUG
        std::cout << "front = " << std::to_string(front_distanse) << "\trear = " << std::to_string(rear_distanse) << std::endl;
#endif
    }
}

void device::read_sensor_state()
{
    static uword value_in = 0;

    if(front_distanse > 0)
    {
        value_in &= static_cast<uword>(~static_cast<unsigned short>(0));
        value_in |= static_cast<uword>(front_distanse);
    }

    if(rear_distanse > 0)
    {
        value_in &= static_cast<uword>(~static_cast<unsigned short>(0)) << 16;
        value_in |= static_cast<uword>(rear_distanse) << 16;
    }

    for(unsigned int i = 0; i < brn->get_input_length(); i++)
        brn->set_in(i, (value_in >> i) & 1);

#if DEBUG
    std::cout << value_in << std::endl;
#endif
}

void device::write_motor_state()
{
    uword value = 0;

    for(unsigned int i = 0; i < brn->get_output_length(); i++)
        value |= static_cast<uword>(brn->get_out(i)) << i;

    _cpu.write_bits(PA_DAT, PA00_DAT_BIT, value, brn->get_output_length());

#if DEBUG
    std::cout << value << std::endl;
#endif
}
