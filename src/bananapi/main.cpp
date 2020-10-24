/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#include <iostream>
#include "device.h"

int main()
{
    try
    {
        device().run();
    }
    catch(std::runtime_error e)
    {
        std::cout << "error: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "unknown error" << std::endl;
    }

    return 0;
}
