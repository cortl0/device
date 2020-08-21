/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef BRAIN_FRIEND_H
#define BRAIN_FRIEND_H

#include <experimental/filesystem>

#include "../binary_neurons_network/src/brain/brain.h"

namespace fs = std::experimental::filesystem;

struct brain_friend
{
    brain &brain_;

    brain_friend() = delete;
    brain_friend(brain &brain_);
    std::string get_state();
    void load();
    void resize(_word brainBits);
    void save();
    void stop();
};

#endif // BRAIN_FRIEND_H
