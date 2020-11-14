/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef BRAIN_FRIEND_DEV_H
#define BRAIN_FRIEND_DEV_H

#include <list>
#include <experimental/filesystem>

#include "../binary_neurons_network/src/brain_friend.h"
#include "../logger/src/logger.h"

namespace fs = std::experimental::filesystem;

namespace bnn
{

struct brain_friend_dev : brain_friend
{
    std::shared_ptr<logger::logger> lgr;

    brain_friend_dev() = delete;
    brain_friend_dev(bnn::brain &brain_, std::shared_ptr<logger::logger>);
    _word get_quantity_of_initialized_neurons_binary();
    std::string get_state();
    void load(std::shared_ptr<logger::logger>);
    void save(std::shared_ptr<logger::logger>);
};

} // namespace bnn

#endif // BRAIN_FRIEND_DEV_H
