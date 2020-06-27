/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef TEST_SERIAL_PORT_BOOST_HPP
#define TEST_SERIAL_PORT_BOOST_HPP

#include <iostream>
#include <cassert>
#include <thread>

#include "../../../bananapi/communication/communicators/serial_port_boost.h"

void test_serial_port_boost()
{
    const std::size_t size = 8;

    char array_0[size]{  0,  1,  2,  3,  4,  5, 6, 7 };
    char array_1[size]{ 15, 14, 13, 12, 11, 10, 9, 8 };

    serial_port_boost sp;

    std::thread([](serial_port_boost *sp, char* rx, std::size_t size)
    {
        assert(sp->read(rx, size) == size);
    },
    &sp, array_1, size).detach();

    usleep(100000);

    assert(sp.write(array_0, size) == size);

    usleep(100000);

    for (std::size_t i = 0; i < size; i++)
    {
        assert(array_0[i] == array_1[i]);
    }

    std::cout << "test_serial_port_boost() passed" << std::endl;
}

#endif // TEST_SERIAL_PORT_BOOST_HPP
