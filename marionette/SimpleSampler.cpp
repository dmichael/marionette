#include "SimpleSampler.h"

SimpleSampler :: SimpleSampler()
{
  // We don't make the waves here yet, because
  // we don't know what they will be.
  baseFrequency_ = 440.0;
  loopGain_ = 1;
  adsr_.setAllTimes( 0.3, 1.5, 0.6, 0.250 );
}  

SimpleSampler :: ~SimpleSampler()
{
  unsigned int i;
  for ( i=0; i<loops_.size(); i++ ) delete loops_[i];
}

void SimpleSampler :: setGain(StkFloat amplitude)
{
  loopGain_ = amplitude;
}

void SimpleSampler :: keyOn()
{
  // Reset all attack waves.
  for ( unsigned int i=0; i<loops_.size(); i++ )
    loops_[0]->reset();

  // Start the envelope.
  adsr_.keyOn();

}

void SimpleSampler :: keyOff()
{
  adsr_.keyOff();
}

void SimpleSampler :: noteOff(StkFloat amplitude)
{
  this->keyOff();
}

void SimpleSampler :: open( std::string filename, bool raw, bool doNormalize )
{
  if(filename.size() < 1){
    printf("Please give a filename.");
  }
  else{
    loops_.push_back( new WaveLoop( filename.c_str(), false ) );
    setChannels(loops_[0]->getChannels());
  }
}

void SimpleSampler :: close()
{ 
  loops_[0]->closeFile(); 
}

// SINGLE CHANNEL
StkFloat SimpleSampler :: computeSample()
{
  StkFloat temp;

  temp = loopGain_ * loops_[0]->tick();
  temp *= adsr_.tick();

  lastOutput_ = temp;
  return lastOutput_;
}

// MULTICHANNEL
void SimpleSampler :: computeFrame( void )
{
  StkFloat temp;
  StkFloat adsr;
  int j;
  // increment the frame
  loops_[0]->tickFrame(lastOutputs_);
  adsr = adsr_.tick();
  
  for ( j=0; j<channels(); j++ ){
    temp = loopGain_ * lastOutputs_[j];
    temp *= adsr;
    lastOutputs_[j] = temp;    
  }
  
}
