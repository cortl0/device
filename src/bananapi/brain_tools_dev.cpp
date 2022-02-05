/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#include "brain_tools_dev.h"

#include <fstream>

namespace bnn
{

brain_tools_dev::brain_tools_dev(_word random_array_length_in_power_of_two,
                                 _word quantity_of_neurons_in_power_of_two,
                                 _word input_length,
                                 _word output_length,
                                 _word threads_count_in_power_of_two,
                                 std::shared_ptr<logger::logger> lgr)
    : brain_tools(random_array_length_in_power_of_two,
                  quantity_of_neurons_in_power_of_two,
                  input_length,
                  output_length,
                  threads_count_in_power_of_two),
      lgr(lgr)
{

}

_word brain_tools_dev::get_quantity_of_initialized_neurons_binary()
{
    return quantity_of_initialized_neurons_binary;
}

std::string brain_tools_dev::get_state()
{
    std::string s("in = ");
    for (_word i = 0; i < 24/*brain_.get_input_length()*/; i++)
        s += std::to_string(world_input[i]);

    s += " out = ";
    for (_word i = 0; i < world_output.size(); i++)
        s += std::to_string(world_output[i]);

    s += " qnp2 = " + std::to_string(quantity_of_neurons_in_power_of_two);

    s += " init = " + std::to_string(quantity_of_initialized_neurons_binary);

    s += " iter = " + std::to_string(get_iteration());

    return s;
}

void brain_tools_dev::load(std::shared_ptr<logger::logger> lgr)
{
    lgr->log(logger::logger::level::info, LOGGER_DATE_TIME_MESSAGE_PLACE_STRING("begin"));

    std::list<std::string> l;

    for (auto & p : fs::directory_iterator(fs::current_path()))
        if(p.path().filename().extension() == ".bnn")
            l.push_back(p.path().filename());

    if(!l.size())
        return;

    l.sort();

    std::ifstream in(fs::current_path() / l.back(), std::ios::binary);

    if(brain_tools::load(in))
    {
        lgr->log(logger::logger::level::trace, LOGGER_DATE_TIME_MESSAGE_PLACE_STRING("file " + std::string(fs::current_path() / l.back()) + " loaded"));

        if(l.size() > 8)
            fs::remove_all(fs::current_path() / l.front());
    }

    lgr->log(logger::logger::level::info, LOGGER_DATE_TIME_MESSAGE_PLACE_STRING("completed"));
}

void brain_tools_dev::save(std::shared_ptr<logger::logger> lgr)
{
    lgr->log(logger::logger::level::info, LOGGER_DATE_TIME_MESSAGE_PLACE_STRING("begin"));

    static char time_buffer[15];
    std::time_t time = std::time(nullptr);
    std::strftime(time_buffer, 15, "%Y%m%d%H%M%S", std::localtime(&time));
    std::string s(time_buffer);
    s += ".bnn";

    std::ofstream out(fs::current_path() / s, std::ios::binary);

    if(brain_tools::save(out))
        lgr->log(logger::logger::level::trace,  LOGGER_DATE_TIME_MESSAGE_PLACE_STRING("file " + std::string(fs::current_path() / s) + " saved"));

    lgr->log(logger::logger::level::info, LOGGER_DATE_TIME_MESSAGE_PLACE_STRING("completed"));
}

} // namespace bnn
