/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#include "converter.h"

void converter::object_to_bytes(char* array, short& offset, void* value, int size, char& crc)
{
    for (short i = 0; i < size; i++)
    {
        array[offset] = reinterpret_cast<char*>(value)[i];
        crc += array[offset++];
    }
}

void converter::bytes_to_object(char* array, short& offset, void* value, int size, char& crc)
{
    for (short i = 0; i < size; i++)
    {
        reinterpret_cast<char*>(value)[i] = array[offset];
        crc += array[offset++];
    }
}

#if 0
#include<iostream>
//#define assertm(exp, msg) assert(((void)msg, exp))
void developing()
{
        const std::type_info& ti1 = typeid(test_0);
        std::cout << ti1.name() << std::endl;

        const std::type_info& ti2 = typeid(*test_0);
        std::cout << ti2.name() << std::endl;

        const std::type_info& ti3 = typeid(&test_0);
        std::cout << ti3.name() << std::endl;

        struct f
        {

        };
        const std::type_info& ti4 = typeid(f);
        std::cout << ti4.name() << std::endl;
        std::cout << std::type_index(ti4).name() << std::endl;



        dto_bpi_to_arduino* co = test_0;
        dto_bpi_to_arduino::st* cs = &(test_0->s);
        int* cx = &(test_0->x);
        int* cy = &(test_0->y);
}
#endif
