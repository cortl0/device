/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#include "brain_friend_dev.h"

#include <fstream>

namespace bnn
{

brain_friend_dev::brain_friend_dev(bnn::brain &brain_, std::shared_ptr<logger::logger> lgr) : brain_friend(brain_), lgr(lgr)
{

}

_word brain_friend_dev::get_quantity_of_initialized_neurons_binary()
{
    return brain_.quantity_of_initialized_neurons_binary;
}

std::string brain_friend_dev::get_state()
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

void brain_friend_dev::load(std::shared_ptr<logger::logger> lgr)
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

    if(brain_friend::load(in))
    {
        lgr->logging(logger::log_level_trace, "brain_friend::load()",  "file " + std::string(fs::current_path() / l.back()) + " loaded");

        if(l.size() > 8)
            fs::remove_all(fs::current_path() / l.front());
    }

    lgr->logging(logger::log_level_information, "brain_friend::load()", "completed");
}

void brain_friend_dev::save(std::shared_ptr<logger::logger> lgr)
{
    lgr->logging(logger::log_level_information, "brain_friend::save()", "begin");

    static char time_buffer[15];
    std::time_t time = std::time(nullptr);
    std::strftime(time_buffer, 15, "%Y%m%d%H%M%S", std::localtime(&time));
    std::string s(time_buffer);
    s += ".bnn";

    std::ofstream out(fs::current_path() / s, std::ios::binary);

    if(brain_friend::save(out))
        lgr->logging(logger::log_level_trace, "brain_friend::save()",  "file " + std::string(fs::current_path() / s) + " saved");

    lgr->logging(logger::log_level_information, "brain_friend::save()", "completed");
}

} // namespace bnn
