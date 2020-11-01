/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#include "brain_friend.h"

#include <fstream>

namespace bnn
{

brain_friend::brain_friend(bnn::brain &brain_, std::shared_ptr<logger::logger> lgr) : brain_(brain_), lgr(lgr) {}

_word brain_friend::get_quantity_of_initialized_neurons_binary()
{
    return brain_.quantity_of_initialized_neurons_binary;
}

std::string brain_friend::get_state()
{
    std::string s("in = ");
    for (_word i = 0; i < 24/*brain_.get_input_length()*/; i++)
        s += std::to_string(brain_.world_input[i]);

    s += " out = ";
    for (_word i = 0; i < brain_.get_output_length(); i++)
        s += std::to_string(brain_.world_output[i]);

    s += " init = ";

    s += std::to_string(brain_.quantity_of_initialized_neurons_binary);

    s += " iter = ";

    s += std::to_string(brain_.iteration);

    return s;
}

void brain_friend::load(std::shared_ptr<logger::logger> lgr)
{
    lgr->logging(logger::log_level_information, "brain_friend::load()", "begin");

    std::list<std::string> l;

    for (auto & p : fs::directory_iterator(fs::current_path()))
        if(p.path().filename().extension() == ".bnn")
            l.push_back(p.path().filename());

    if(!l.size())
        return;

    l.sort();

    std::ifstream in(fs::current_path() / l.back(), std::ios::binary);

    if(in.is_open())
    {
        in.read(reinterpret_cast<char*>(&brain_.quantity_of_neurons_in_power_of_two), sizeof (brain_.quantity_of_neurons_in_power_of_two));
        in.read(reinterpret_cast<char*>(&brain_.quantity_of_neurons), sizeof (brain_.quantity_of_neurons));
        in.read(reinterpret_cast<char*>(&brain_.quantity_of_neurons_binary), sizeof (brain_.quantity_of_neurons_binary));
        in.read(reinterpret_cast<char*>(&brain_.quantity_of_neurons_sensor), sizeof (brain_.quantity_of_neurons_sensor));
        in.read(reinterpret_cast<char*>(&brain_.quantity_of_neurons_motor), sizeof (brain_.quantity_of_neurons_motor));
        in.read(reinterpret_cast<char*>(&brain_.work), sizeof (brain_.work));
        in.read(reinterpret_cast<char*>(&brain_.iteration), sizeof (brain_.iteration));
        in.read(reinterpret_cast<char*>(&brain_.quantity_of_initialized_neurons_binary), sizeof (brain_.quantity_of_initialized_neurons_binary));
        in.read(reinterpret_cast<char*>(&brain_.debug_soft_kill), sizeof (brain_.debug_soft_kill));

        brain_.world_input.resize(brain_.quantity_of_neurons_sensor);

        bool b;

        for(_word i = 0; i < brain_.quantity_of_neurons_sensor; i++)
        {
            in.read(reinterpret_cast<char*>(&b), sizeof (b));
            brain_.world_input[i] = b;
        }

        brain_.world_output.resize(brain_.quantity_of_neurons_motor);

        for(_word i = 0; i < brain_.quantity_of_neurons_motor; i++)
        {
            in.read(reinterpret_cast<char*>(&b), sizeof (b));
            brain_.world_output[i] = b;
        }

        brain_.us.resize(brain_.quantity_of_neurons);

        _word w;

        for(_word i = 0; i < brain_.quantity_of_neurons; i++)
            for(_word j = 0; j < sizeof(brain::union_storage) / sizeof(_word); j++)
            {
                in.read(reinterpret_cast<char*>(&w), sizeof (w));
                brain_.us[i].words[j] = w;
            }

        _word rndmLength;

        in.read(reinterpret_cast<char*>(&rndmLength), sizeof (rndmLength));

        if(rndmLength != brain_.rndm->get_length())
            brain_.rndm.reset(new random_put_get(rndmLength, 3));

        for(_word i = 0; i < brain_.rndm->get_length(); i++)
        {
            in.read(reinterpret_cast<char*>(&w), sizeof (w));
            brain_.rndm->get_array()[i] = w;
        }

        in.read(reinterpret_cast<char*>(&brain_.rndm->debug_count_put), sizeof (brain_.rndm->debug_count_put));
        in.read(reinterpret_cast<char*>(&brain_.rndm->debug_count_get), sizeof (brain_.rndm->debug_count_get));

        lgr->logging(logger::log_level_trace, "brain_friend::load()",  "file " + std::string(fs::current_path() / l.back()) + " loaded");

        if(l.size() > 8)
            fs::remove_all(fs::current_path() / l.front());
    }

    lgr->logging(logger::log_level_information, "brain_friend::load()", "completed");
}

void brain_friend::resize(_word brainBits_)
{
    brain_.mtx.lock();
    brain_.work = false;
    brain_.mtx.unlock();

    usleep(200);

    brain_.mtx.lock();

    if(brainBits_ > brain_.quantity_of_neurons_in_power_of_two)
    {
        _word quantity_of_neuron_end_temp = 1 << (brainBits_);

        std::vector<bnn::brain::union_storage> us_temp = std::vector<brain::union_storage>(quantity_of_neuron_end_temp);

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
}

void brain_friend::save(std::shared_ptr<logger::logger> lgr)
{
    lgr->logging(logger::log_level_information, "brain_friend::save()", "begin");

    static char time_buffer[15];
    std::time_t time = std::time(nullptr);
    std::strftime(time_buffer, 15, "%Y%m%d%H%M%S", std::localtime(&time));
    std::string s(time_buffer);
    s += ".bnn";

    std::ofstream out(fs::current_path() / s, std::ios::binary);

    if(out.is_open())
    {
        out.write(reinterpret_cast<char*>(&brain_.quantity_of_neurons_in_power_of_two), sizeof (brain_.quantity_of_neurons_in_power_of_two));
        out.write(reinterpret_cast<char*>(&brain_.quantity_of_neurons), sizeof (brain_.quantity_of_neurons));
        out.write(reinterpret_cast<char*>(&brain_.quantity_of_neurons_binary), sizeof (brain_.quantity_of_neurons_binary));
        out.write(reinterpret_cast<char*>(&brain_.quantity_of_neurons_sensor), sizeof (brain_.quantity_of_neurons_sensor));
        out.write(reinterpret_cast<char*>(&brain_.quantity_of_neurons_motor), sizeof (brain_.quantity_of_neurons_motor));
        out.write(reinterpret_cast<char*>(&brain_.work), sizeof (brain_.work));
        out.write(reinterpret_cast<char*>(&brain_.iteration), sizeof (brain_.iteration));
        out.write(reinterpret_cast<char*>(&brain_.quantity_of_initialized_neurons_binary), sizeof (brain_.quantity_of_initialized_neurons_binary));
        out.write(reinterpret_cast<char*>(&brain_.debug_soft_kill), sizeof (brain_.debug_soft_kill));

        bool b;

        for(_word i = 0; i < brain_.quantity_of_neurons_sensor; i++)
        {
            b = brain_.world_input[i];
            out.write(reinterpret_cast<char*>(&b), sizeof (b));
        }

        for(_word i = 0; i < brain_.quantity_of_neurons_motor; i++)
        {
            b = brain_.world_output[i];
            out.write(reinterpret_cast<char*>(&b), sizeof (b));
        }

        _word w;

        for(_word i = 0; i < brain_.quantity_of_neurons; i++)
            for(_word j = 0; j < sizeof(brain::union_storage) / sizeof(_word); j++)
            {
                w = brain_.us[i].words[j];
                out.write(reinterpret_cast<char*>(&w), sizeof (w));
            }

        w = brain_.rndm->get_length();
        out.write(reinterpret_cast<char*>(&w), sizeof (w));

        for(_word i = 0; i < brain_.rndm->get_length(); i++)
        {
            w = brain_.rndm->get_array()[i];
            out.write(reinterpret_cast<char*>(&w), sizeof (w));
        }

        out.write(reinterpret_cast<char*>(&brain_.rndm->debug_count_put), sizeof (brain_.rndm->debug_count_put));
        out.write(reinterpret_cast<char*>(&brain_.rndm->debug_count_get), sizeof (brain_.rndm->debug_count_get));

        lgr->logging(logger::log_level_trace, "brain_friend::save()",  "file " + std::string(fs::current_path() / s) + " saved");
    }

    lgr->logging(logger::log_level_information, "brain_friend::save()", "completed");
}

void brain_friend::stop()
{
    brain_.stop();
}

} // !namespace bnn
