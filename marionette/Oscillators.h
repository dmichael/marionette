#include <list>
#include "Unit.h"

/***************************************************/
/*! \class Sine
    \brief STK basic mixer class.

    This class keeps a vector list of inputs and sums them to compute the output frames

    by Perry R. Cook and Gary P. Scavone, 1995 - 2007.
*/
/***************************************************/

#include "SineWave.h"
    
#ifndef STK_SINEWAVEUGEN_H
#define STK_SINEWAVEUGEN_H

class SineWaveUnit : public Unit
{
public:
  //! Default constructor.
  SineWaveUnit( void );

  //! Class destructor.
  ~SineWaveUnit( void );
  
  void setFrequency(float);
  
  char * name( void ){ return "SineWave"; }
  
  int addFreqMod(Unit*);
  
  int remove(Unit*);
  
  void clearInputs();

	std::vector<Unit*> frequency_mods;
	std::list<Unit*> phase_mods;
	std::list<Unit*> writers;
		  
  SineWave *source;
  
  
protected:
  int frequency_mods_count_;
  StkFloat computeSample( void );

};

#endif