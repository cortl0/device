/*
 *   device
 *   created by Ilya Shishkin
 *   cortl@8iter.ru
 *   https://github.com/cortl0/device
 *   licensed by GPL v3.0
 */

#ifndef SIMPLE_MATH_H
#define SIMPLE_MATH_H

struct simple_math
{
  inline static int Abs(int x)
  {
    if (0b1 & (x >> 15))
      return ~x;
    else
      return x;
  }

  inline static int sign(int x)
  {
    return 1 - 2 * (0b1 & (x >> 15));
  }

  inline static int sign0(int x)
  {
    if (x == 0)
      return 0;
    else
      return 1 - 2 * (0b1 & (x >> 15));
  }
};

#endif //SIMPLE_MATH_H

