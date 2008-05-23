//use this first function to seed the random number generator,
//call this before any of the other functions
#include "Rand.h"

Rand::Rand() {  
 // initrand();
}

Rand::~Rand() {  
  
}

void Rand::initrand() {
    //srand(time(0) * getpid());
}

//generates a psuedo-random integer between 0 and 32767
int Rand::randint() {
    return rand();
}

//generates a psuedo-random integer between 0 and max
int Rand::randint(int max) {
    return int(max*rand()/(RAND_MAX+1.0));
}

//generates a psuedo-random integer between min and max
int Rand::randint(int min, int max) {
    if (min>max) {
        return max+int((min-max+1)*rand()/(RAND_MAX+1.0));
    }
    else {
        return min+int((max-min+1)*rand()/(RAND_MAX+1.0));
    }
}

//generates a psuedo-random float between 0.0 and 0.999...
float Rand::randfloat() {
  return (float) rand() / (float) RAND_MAX;
}

//generates a psuedo-random float between 0.0 and max
float Rand::randfloat(float max) {
    return randfloat()*max;
}

//generates a psuedo-random float between min and max
float Rand::randfloat(float min, float max) {
    if (min>max)
    {
        return randfloat()*(min-max)+max;    
    }
    else
    {
        return randfloat()*(max-min)+min;
    }    
}

//generates a psuedo-random double between 0.0 and 0.999...
double Rand::randdouble() {
    return rand()/(double(RAND_MAX)+1);
}

//generates a psuedo-random double between 0.0 and max
double Rand::randdouble(double max) {
    return randdouble()*max;
}

//generates a psuedo-random double between min and max
double Rand::randdouble(double min, double max) {
    if (min>max) {
        return randdouble()*(min-max)+max;    
    }
    else {
        return randdouble()*(max-min)+min;
    }
}

  