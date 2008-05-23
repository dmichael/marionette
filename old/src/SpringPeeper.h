#ifndef STK_SPRING_PEEPER_H
#define STK_SPRING_PEEPER_H
#include "Anuran.h"

class SpringPeeper : public Anuran 
{
public:
  //! The constructor.
  SpringPeeper(Dispatcher*);
  
  //! The destructor.
  ~SpringPeeper(void);
};

#endif
