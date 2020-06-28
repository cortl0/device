#include "device.h"

device::device()
{
    _cpu.write_bits(CON2_P13_CFG_REG, CON2_P13_CFG_BIT, P_SELECT_OUTPUT, 3);
    _cpu.write_bits(CON2_P11_CFG_REG, CON2_P11_CFG_BIT, P_SELECT_OUTPUT, 3);
    _cpu.write_bits(CON2_P22_CFG_REG, CON2_P22_CFG_BIT, P_SELECT_OUTPUT, 3);
    _cpu.write_bits(CON2_P15_CFG_REG, CON2_P15_CFG_BIT, P_SELECT_OUTPUT, 3);

    _cpu.write_bits(PA_DAT, PA00_DAT_BIT, 0b0000, brn->get_output_length());

    brn.reset(new brain(random_array_length_in_power_of_two,
                        quantity_of_neurons_in_power_of_two,
                        input_length,
                        output_length,
                        brain_clock_cycle_handler));

    brn_frnd.reset(new brain_friend(*brn.get()));
}

void device::brain_clock_cycle_handler(void* owner)
{
    static_cast<device*>(owner)->write_motor_state();
}

void device::start()
{
    brn->start(this, false);
}

void device::write_motor_state()
{
    uword value = 0;

    for(unsigned int i = 0; i < brn->get_output_length(); i++)
        value |= static_cast<uword>(brn->get_out(i)) << i;

    _cpu.write_bits(PA_DAT, PA00_DAT_BIT, value, brn->get_output_length());
}
