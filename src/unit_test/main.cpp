/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#include "test_communication/test_data_transfer_objects/test_dto_bpi.hpp"
#include "test_communication/test_communicators/test_serial_port_boost.hpp"
#include "test_communication/test_dto_bpi_through_serial_port_boost.hpp"

int main()
{
    test_dto_bpi_to_arduino();

    test_serial_port_boost();

    test_dto_bpi_through_serial_port_boost();

    return 0;
}
