/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#include "dto_bpi.h"

dto_bpi::~dto_bpi()
{

}

dto_bpi::dto_bpi()
{
    name[14] = '1';
}

void dto_bpi::serialize(char* array, short& offset)
{
    crc = 0;
    converter::object_to_bytes(array, offset, reinterpret_cast<void*>(&name), NAME_LENGTH, crc);
    converter::object_to_bytes(array, offset, reinterpret_cast<void*>(&x), sizeof (x), crc);
    converter::object_to_bytes(array, offset, reinterpret_cast<void*>(&y), sizeof (y), crc);
    converter::object_to_bytes(array, offset, reinterpret_cast<void*>(&crc), sizeof (crc), crc);
}

bool dto_bpi::deserialize(char* array, short& offset)
{
    char name_data[NAME_LENGTH], crc_data, crc_dummy;
    crc = 0;
    converter::bytes_to_object(array, offset, reinterpret_cast<void*>(&name_data), NAME_LENGTH, crc);
    for(int i = 0; i < NAME_LENGTH; i++)
        if(name_data[i] != name[i])
            return false;
    converter::bytes_to_object(array, offset, reinterpret_cast<void*>(&x), sizeof (x), crc);
    converter::bytes_to_object(array, offset, reinterpret_cast<void*>(&y), sizeof (y), crc);
    converter::bytes_to_object(array, offset, reinterpret_cast<void*>(&crc_data), sizeof (crc_data), crc_dummy);
    return crc_data == crc;
}
