/***************************************************/
/*! \class Generator
    \brief STK abstract unit generator parent class.

    This class provides common functionality for
    STK unit generator sample-source subclasses.

    by Perry R. Cook and Gary P. Scavone, 1995 - 2007.
*/
/***************************************************/

#ifndef STK_UGEN_H
#define STK_UGEN_H

#include "Stk.h"
#include "Observable.h"
#include <iostream>


class Unit : public Stk, public Observable
{
 public:
  //! Class constructor.
  Unit( void );

  //! Class destructor.
  virtual ~Unit( void );
  
  void setChannels( int );
  int channels( void );
  
  // get pan
  float pan( void );
  
  // get gain
  float gain( void );
  
  // Set the pan
  void setPan(float);
  
  // Set the pan
  void setGain(float);
  
  //! Return the last output value.
  virtual StkFloat lastOut( void ) const { return lastOutput_; };

  //! Compute one sample and output.
  virtual StkFloat tick( void );

  //! Fill a channel of the StkFrames object with computed outputs.
  /*!
    The \c channel argument should be zero or greater (the first
    channel is specified by 0).  An StkError will be thrown if the \c
    channel argument is equal to or greater than the number of
    channels in the StkFrames object.
  */
  virtual StkFrames& tick( StkFrames& frames, unsigned int channel = 0 );
  
  // multichannel adapter
  virtual StkFrames& tickFrame( StkFrames& frames );
  
  void update( void *, void * ){printf("overwrite this!");};		///< this will be implemented in subclasses.
  //virtual void update(void *, void *){ printf("alas, this is not what i want to update.\n");/* do nothing - please override in subclasses as needed */ }

 protected:
   
   // Time information
 	 //unsigned long	bufclock;
   unsigned int	nextclock, clock;
   
   // Channel information
   float pan_;
   float gain_;
   
  // This abstract function must be implemented in all subclasses.
  // It is used to get around a C++ problem with overloaded virtual
  // functions.
  virtual StkFloat computeSample( void ) = 0;
  virtual void computeFrame( void ) {/* */};
  StkFloat lastOutput_;
  StkFrames lastOutputs_;
  StkFrames lastOutputs_temp_; // temporary buffer for summation functions
};

#endif

