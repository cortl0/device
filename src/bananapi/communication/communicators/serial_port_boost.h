/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef SERIAL_PORT_BOOST_H
#define SERIAL_PORT_BOOST_H

#include <boost/asio.hpp>

#include "communicator_base.h"

class serial_port_boost : public communicator_base
{
    boost::asio::io_service io;
    boost::asio::serial_port port;
public:
    serial_port_boost();
    virtual ~serial_port_boost();
    virtual std::size_t write(char *tx, std::size_t size);
    virtual std::size_t read(char *rx, std::size_t size);
};

#endif // SERIAL_PORT_BOOST_H
