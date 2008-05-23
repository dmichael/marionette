/***************************************************/
/*! \class Bus
    \brief STK basic mixer class.

    This class keeps a vector list of inputs and sums them to compute the output frames

    by Perry R. Cook and Gary P. Scavone, 1995 - 2007.
*/
/***************************************************/

#ifndef SIMPLESAMPLER_H
#define SIMPLESAMPLER_H

#include "Unit.h"
#include "WaveLoop.h"
#include "ADSR.h"
#include <list>
#include <cstdio>
#include <iostream>

class SimpleSampler : public Unit
{
public:
  //! Default constructor.
  SimpleSampler( void );

  //! Class destructor.
  virtual ~SimpleSampler( void );
  
  char * name( void ){ return "SimpleSampler"; }
  
  void setGain(StkFloat);
  
  //! Initiate the envelopes with a key-on event and reset the attack waves.
  void keyOn();

  //! Signal a key-off event to the envelopes.
  void keyOff();

  //! Stop a note with the given amplitude (speed of decay).
  virtual void noteOff(StkFloat amplitude);

  //! Open the specified file and load its data.
  /*!
    Data from a previously opened file will be overwritten by this
    function.  An StkError will be thrown if the file is not found,
    its format is unknown, or a read error occurs.  If the file data
    is to be loaded incrementally from disk and normalization is
    specified, a scaling will be applied with respect to fixed-point
    limits.  If the data format is floating-point, no scaling is
    performed.
  */
  void open( std::string fileName, bool raw = false, bool doNormalize = true );

  //! Close a file if one is open.
  void close( void );

  ADSR adsr_;
  
    
protected:
  
  std::vector<WaveLoop *> loops_;

  StkFloat baseFrequency_;

  std::vector<StkFloat> loopRatios_;

  StkFloat loopGain_;
  
  StkFloat computeSample( void );

  void computeFrame( void );
};

#endif