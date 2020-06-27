/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef DTO_BPI_H
#define DTO_BPI_H

#include "serializable.h"

class dto_bpi : public serializable
{
public:
    int x, y;
    virtual ~dto_bpi();
    dto_bpi();
    virtual void serialize(char* array, short& offset);
    virtual bool deserialize(char* array, short& offset);
};

#endif // DTO_BPI_H
