/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef TEST_DTO_BPI_THROUGH_SERIAL_PORT_BOOST_HPP
#define TEST_DTO_BPI_THROUGH_SERIAL_PORT_BOOST_HPP

#include <iostream>
#include <cassert>
#include <thread>

#include "../bananapi/communication/communicators/serial_port_boost.h"
#include "../bananapi/communication/data_transfer_objects/dto_bpi.h"

void test_dto_bpi_through_serial_port_boost()
{
    const std::size_t size = 256;

    char array_0[size];
    char array_1[size];

    short offset_0 = 0;
    short offset_1 = 0;

    //    dto_bpi_to_arduino* test_0 = new dto_bpi_to_arduino;
    //    dto_arduino_to_bpi* test_1 = new dto_arduino_to_bpi;

    dto_bpi test_0;
    dto_bpi test_1;
    test_0.x = 5;
    test_0.y = 8;
    test_1.x = 50;
    test_1.y = 80;

    test_0.serialize(array_0, offset_0);

    serial_port_boost sp;

    std::thread([](serial_port_boost *sp, char* rx, std::size_t size)
    {
        assert(sp->read(rx, size) == size);
    },
    &sp, array_1, static_cast<std::size_t>(offset_0)).detach();

    usleep(100000);

    assert(sp.write(array_0, static_cast<std::size_t>(offset_0)) == static_cast<std::size_t>(offset_0));

    usleep(100000);

    assert(test_1.deserialize(array_1, offset_1));

    assert(offset_0 == offset_1);

    assert(test_0.x == test_1.x);
    assert(test_0.y == test_1.y);

    std::cout << "test_dto_bpi_to_arduino_through_serial_port_boost() passed" << std::endl;
}

#endif // TEST_DTO_BPI_THROUGH_SERIAL_PORT_BOOST_HPP
