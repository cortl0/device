/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef BRAIN_TOOLS_DEV_H
#define BRAIN_TOOLS_DEV_H

#include <list>
#include <experimental/filesystem>

#include "../bnn/src/brain_tools.h"
#include "../logger/src/logger.h"

namespace fs = std::experimental::filesystem;

namespace bnn
{

struct brain_tools_dev : brain_tools
{
    std::shared_ptr<logger::logger> lgr;

    brain_tools_dev() = delete;
    brain_tools_dev(_word random_array_length_in_power_of_two,
                    _word quantity_of_neurons_in_power_of_two,
                    _word input_length,
                    _word output_length,
                    _word threads_count_in_power_of_two,
                    std::shared_ptr<logger::logger>);
    _word get_quantity_of_initialized_neurons_binary();
    std::string get_state();
    void load(std::shared_ptr<logger::logger>);
    void save(std::shared_ptr<logger::logger>);
};

} // namespace bnn

#endif // BRAIN_TOOLS_DEV_H
