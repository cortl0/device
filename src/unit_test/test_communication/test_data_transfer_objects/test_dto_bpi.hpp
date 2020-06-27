/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef TEST_DTO_BPI_TO_ARDUINO_HPP
#define TEST_DTO_BPI_TO_ARDUINO_HPP

#include <iostream>
#include <cassert>

#include "../../../bananapi/communication/data_transfer_objects/dto_bpi.h"

void test_dto_bpi_to_arduino()
{
    char array[256];

    short offset_0 = 0;
    short offset_1 = 0;

    dto_bpi test_0;
    test_0.x = 5;
    test_0.y = 8;

    dto_bpi test_1;
    test_1.x = 50;
    test_1.y = 80;

    test_0.serialize(array, offset_0);

    assert(test_1.deserialize(array, offset_1));

    assert(offset_0 == offset_1);

    assert(test_0.x == test_1.x);
    assert(test_0.y == test_1.y);

    std::cout << "test_dto_bpi_to_arduino() passed" << std::endl;
}

#endif // TEST_DTO_BPI_TO_ARDUINO_HPP
