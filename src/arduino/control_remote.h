/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef CONTROL_REMOTE_H
#define CONTROL_REMOTE_H

#include "Arduino.h"
#include "control.h"

class control_remote : 
public control
{
public:
  virtual ~control_remote();
  control_remote();
  void get_state(int& left, int& right);
};

#endif //CONTROL_REMOTE_H

