/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef CONTROL_H
#define CONTROL_H

#include "Arduino.h"

class control
{
public:
  virtual ~control()
  {

  }
  
  control()
  {

  }

  virtual void get_state(int& left, int& right) = 0;
};

#endif //CONTROL_H

