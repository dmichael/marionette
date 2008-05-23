#include "Oscillators.h"

SineWaveUnit :: SineWaveUnit()
{
  source = new SineWave;
  source->setFrequency(440.0);
}

SineWaveUnit :: ~SineWaveUnit()
{
  delete source;
}

void SineWaveUnit :: setFrequency(float freq)
{
  source->setFrequency(freq);
}

int SineWaveUnit :: addFreqMod(Unit *ugen)
{
  frequency_mods.push_back(ugen);
  frequency_mods_count_++; 
	// expand number of channels in bus, if necessary:
	// return added process
	return 1;
}

int SineWaveUnit :: remove(Unit *ugen)
{	
	// return removed process
	return 1;
}

void SineWaveUnit :: clearInputs()
{
	
}

StkFloat SineWaveUnit :: computeSample()
{  
  lastOutput_ = source->tick();
  return lastOutput_;
}
