/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef CONVERTER_H
#define CONVERTER_H

#if 0
#include <typeinfo>
#include <typeindex>
#endif

struct converter
{
    static void object_to_bytes(char* array, short& offset, void* value, int size, char& crc);
    static void bytes_to_object(char* array, short& offset, void* value, int size, char& crc);
    //static void developing();
};

#endif // CONVERTER_H

