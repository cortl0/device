/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#include "control_remote.h"

control_remote::~control_remote()
{

}

control_remote::control_remote()
{
  DDRC = DDRC & B11110000;
}

void control_remote::get_state(int& left, int& right)
{
  left = PINC & B00000011;
  right = (PINC & B00001100) >> 2;
}

