/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#include "serial_port_boost.h"

serial_port_boost::~serial_port_boost()
{
    port.close();
}

serial_port_boost::serial_port_boost()
    : communicator_base()
    , port(boost::asio::serial_port(io))
{
    port.open("/dev/ttyUSB0");
    port.set_option(boost::asio::serial_port_base::baud_rate(115200));
}

std::size_t serial_port_boost::write(char *tx, std::size_t size)
{
    tx_mtx.lock();
    std::size_t return_value = boost::asio::write(port, boost::asio::buffer(tx, size));
    tx_mtx.unlock();
    return return_value;
}

std::size_t serial_port_boost::read(char *rx, std::size_t size)
{
    rx_mtx.lock();
    std::size_t return_value =  boost::asio::read(port, boost::asio::buffer(rx, size));
    rx_mtx.unlock();
    return return_value;
}
