/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include "../converter.h"

#define NAME_LENGTH 16

class serializable
{
protected:
    char name[NAME_LENGTH] { 'm', 'e', 's', 's', 'a', 'g', 'e', '0', '0', '0', '0', '0', '0', '0', '0', '\0' };
    char crc; // CRC of name & members
public:
    virtual ~serializable();
    serializable();
    virtual void serialize(char* array, short& offset) = 0;
    virtual bool deserialize(char* array, short& offset) = 0;
};

#endif // SERIALIZABLE_H
