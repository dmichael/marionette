/***************************************************
\class Rand
***************************************************/
#ifndef Rand_H
#define Rand_H

#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>

class Rand 
{
public:
  //! Class constructor.
  Rand();
  //! Class destructor.
  ~Rand();
  void initrand(void);
  int randint();
  int randint(int);
  int randint(int, int);
  float randfloat();
  float randfloat(float);
  float randfloat(float, float);
  double randdouble();
  double randdouble(double);
  double randdouble(double, double);

};
#endif
