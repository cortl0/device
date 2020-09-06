/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

//#define under_development

#include "brain_friend.h"

#include <fstream>

brain_friend::brain_friend(brain &brain_) : brain_(brain_) {}

_word brain_friend::get_quantity_of_initialized_neurons_binary()
{
    return brain_.quantity_of_initialized_neurons_binary;
}

std::string brain_friend::get_state()
{
    std::string s("in = ");
    for (_word i = 0; i < brain_.get_input_length(); i++)
        s += std::to_string(brain_.world_input[i]);

    s+= " out = ";
    for (_word i = 0; i < brain_.get_output_length(); i++)
        s += std::to_string(brain_.world_output[i]);

    s += " quantity_of_initialized_neurons_binary = ";

    s += std::to_string(brain_.quantity_of_initialized_neurons_binary);

    s += " iteration = ";

    s += std::to_string(brain_.iteration);

    return s;
}

void brain_friend::load()
{
#ifdef under_development
    std::ifstream in(fs::current_path() / "storage.bnn", std::ios::binary);

    if(in.is_open())
    {
        in >> brain_.quantity_of_neurons_in_power_of_two;
        in >> brain_.quantity_of_neurons;
        in >> brain_.quantity_of_neurons_binary;
        in >> brain_.quantity_of_neurons_sensor;
        in >> brain_.quantity_of_neurons_motor;
        in >> brain_.work;
        in >> brain_.iteration;
        in >> brain_.quantity_of_initialized_neurons_binary;
        in >> brain_.debug_soft_kill;

        brain_.world_input.resize(brain_.quantity_of_neurons_sensor);

        bool b;
        for(_word i = 0; i < brain_.quantity_of_neurons_sensor; i++)
        {
            in >> b;
            brain_.world_input[i] = b;
        }

        brain_.world_output.resize(brain_.quantity_of_neurons_motor);

        for(_word i = 0; i < brain_.quantity_of_neurons_motor; i++)
        {
            in >> b;
            brain_.world_output[i] = b;
        }

        brain_.us.resize(brain_.quantity_of_neurons);

        for(_word i = 0; i < brain_.quantity_of_neurons; i++)
            for(_word j = 0; j < sizeof(brain::union_storage) / sizeof(_word); j++)
                in >> brain_.us[i].words[j];

        _word rndmLength;

        in >> rndmLength;

        if(rndmLength!=brain_.rndm->get_length())
            brain_.rndm.reset(new random_put_get(rndmLength));

        for(_word i = 0; i < brain_.rndm->get_length(); i++)
            in >> brain_.rndm->get_array()[i];

        in >> brain_.rndm->debug_count_put;
        in >> brain_.rndm->debug_count_get;
    }
#endif
}

void brain_friend::resize(_word brainBits_)
{
#ifdef under_development
    brain_.mtx.lock();
    brain_.work = false;
    brain_.mtx.unlock();

    usleep(200);

    brain_.mtx.lock();
    if(brainBits_ > brain_.quantity_of_neurons_in_power_of_two)
    {
        _word quantity_of_neuron_end_temp = 1 << (brainBits_);

        std::vector<brain::union_storage> us_temp = std::vector<brain::union_storage>(quantity_of_neuron_end_temp);

        for(_word i = 0; i < brain_.quantity_of_neurons; i++)
            for(_word j = 0; j < sizeof(brain::union_storage) / sizeof(_word); j++)
                us_temp[i].words[j] = brain_.us[i].words[j];

        for (_word i = brain_.quantity_of_neurons; i < quantity_of_neuron_end_temp; i++)
            us_temp[i].binary_ = brain::binary();

        std::swap(brain_.us, us_temp);

        brain_.quantity_of_neurons_in_power_of_two = brainBits_;
        brain_.quantity_of_neurons = quantity_of_neuron_end_temp;
        brain_.quantity_of_neurons_binary = brain_.quantity_of_neurons - brain_.quantity_of_neurons_sensor - brain_.quantity_of_neurons_motor;
        brain_.reaction_rate = brain_.quantity_of_neurons;
    }
    brain_.mtx.unlock();
#endif
}

void brain_friend::save()
{
#ifdef under_development
    std::ofstream out(fs::current_path() / "storage.bnn", std::ios::binary);

    if(out.is_open())
    {
        out << brain_.quantity_of_neurons_in_power_of_two;
        out << brain_.quantity_of_neurons;
        out << brain_.quantity_of_neurons_binary;
        out << brain_.quantity_of_neurons_sensor;
        out << brain_.quantity_of_neurons_motor;
        out << brain_.work;
        out << brain_.iteration;
        out << brain_.quantity_of_initialized_neurons_binary;
        out << brain_.debug_soft_kill;

        for(_word i = 0; i < brain_.quantity_of_neurons_sensor; i++)
            out << brain_.world_input[i];

        for(_word i = 0; i < brain_.quantity_of_neurons_motor; i++)
            out << brain_.world_output[i];

        for(_word i = 0; i < brain_.quantity_of_neurons; i++)
            for(_word j = 0; j < sizeof(brain::union_storage) / sizeof(_word); j++)
                out << brain_.us[i].words[j];

        out << brain_.rndm->get_length();

        for(_word i = 0; i < brain_.rndm->get_length(); i++)
            out << brain_.rndm->get_array()[i];

        out << brain_.rndm->debug_count_put;
        out << brain_.rndm->debug_count_get;
    }
#endif
}

void brain_friend::stop()
{
    brain_.stop();
}
