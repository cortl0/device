/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef COMMUNICATOR_BASE_H
#define COMMUNICATOR_BASE_H

#include <mutex>

class communicator_base
{
protected:
    std::mutex rx_mtx;
    std::mutex tx_mtx;
public:
    communicator_base();
    virtual ~communicator_base();
    virtual std::size_t write(char *tx, std::size_t size) = 0;
    virtual std::size_t read(char *rx, std::size_t size) = 0;
};

#endif // COMMUNICATOR_BASE_H
