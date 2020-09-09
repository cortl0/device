/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef BRAIN_FRIEND_H
#define BRAIN_FRIEND_H

#include <list>
#include <experimental/filesystem>

#include "../binary_neurons_network/src/brain/brain.h"
#include "../logger/src/logger.h"

namespace fs = std::experimental::filesystem;

namespace bnn
{

struct brain_friend
{
    bnn::brain &brain_;
    std::shared_ptr<logger::logger> lgr;

    brain_friend() = delete;
    brain_friend(bnn::brain &brain_, std::shared_ptr<logger::logger>);
    _word get_quantity_of_initialized_neurons_binary();
    std::string get_state();
    void load();
    void resize(_word brainBits);
    void save();
    void stop();
};

}

#endif // BRAIN_FRIEND_H
