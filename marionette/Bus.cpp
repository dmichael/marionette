
#include <list>

#include "Bus.h"


Bus::Bus()
{
}

Bus::Bus(int num_channels)
{
  setChannels(num_channels);
  this->gains.resize( 1, num_channels );
  for(int i=0; i<num_channels; i++){
    gains[i] = 1;
  }
  number_of_speakers = num_channels;
}

Bus::~Bus()
{
	writers.clear();
}

int Bus::add(Unit *unit)
{
  // initialize a input
  Bus::Input input;
  input.unit = unit;
  // initialize the input gains
  for(int i=0; i < number_of_speakers; i++){
      input.gains.push_back( 0 );
  }
  
  calculateGains(&input);

  // we must first check to see if the this Unit has already been added to the bus
  // otherwise it plays at doubletime! (note to self: figure out why)
  //std::list<Unit*>::iterator i;
  for ( unsigned int i=0; i<writers.size(); i++ ) 
  {
  //for(i = writers.begin(); i != writers.end(); i++){
    if(writers[i].unit == unit){
      return 0;
    }
  }
  writers.push_back(input);	
  //this->pans.resize( 1, writers.size() );
	// TODO: expand number of channels in bus, if necessary:
	// return added process?
	return 1;
}

int Bus::remove(Unit *ugen)
{	
  //std::list<Unit*>::iterator i;
  //for(i = writers.begin(); i != writers.end(); i++){
  for ( unsigned int i=0; i<writers.size(); i++ ) 
  {
    if(writers[i].unit == ugen){
      writers.erase(writers.begin()+i);
      // this->pans.resize( 1, writers.size() );
      return 1;
    }
  }
  
	return 0;
}


int Bus :: addInsert(FX *effect)
{
  // we must first check to see if the this Unit has already been added to the bus
  // otherwise it plays at doubletime! (note to self: figure out why)
  std::list<FX*>::iterator i;
  for(i = inserts.begin(); i != inserts.end(); i++){
    if((*i) == effect){
      return 0;
    }
  }
  inserts.push_back(effect);	
	// TODO: expand number of channels in bus, if necessary:
	// return added process?
	return 1;
}

int Bus :: removeInsert(FX *effect)
{
  std::list<FX*>::iterator i;
  for(i = inserts.begin(); i != inserts.end(); i++){
    if((*i) == effect){
      inserts.erase(i);
      return 1;
    }
  }
	return 0;
}

void Bus :: calculateGains(Bus::Input *input)
{
  float pan  = input->unit->pan();
  float gain = input->unit->gain();
  if(number_of_speakers == 2){
    float p = (pan + 1.f) * 0.5f;
    input->gains[0] = (1.f - p) * gain;
    input->gains[1] = p * gain; 
  }
  else{
    return;
  }
}

void Bus :: calculateGains(float gain, float pan, int writer_num)
{
  // if(pans[writer_num] == pan) return;
  // pans[writer_num] = pan;

  // printf("%f\n",pans[writer_num]);
  // TODO!! RETURN IF THE PAN HAS NOT CHANGED
  // simple stereo gain algorithm from Vessel
  if(channels() == 2){
    float p = (pan + 1.f) * 0.5f;
    gains[0] = (1.f - p) * gain;
    gains[1] = p * gain; 
  }
  else{
    return;
  }
}

void Bus :: calculateInserts()
{
  // INSERTS (MONO)
  std::list<FX*>::iterator i;
  for(i = inserts.begin(); i != inserts.end(); i++){
    lastOutput_ += (*i)->tick(lastOutput_);
  } 
}

void Bus::clearInputs()
{
	writers.clear();
}


StkFloat Bus::computeSample()
{  
  return 0.0;
  // This should not even really be used! You need to compute full multichannel buffers.
}

/*
  For efficiency, it is best to have either 
  1) a one channel StkFrame input, or
  2) a multichannel StkFrame input that matches the number of output channels
  In the case this is insufficient, such as a stereo WAV file to be played through 4 speakers...?
*/
void Bus::computeFrame( void )
{

  // make sure that the samples are cleared out - this is super shitty
  for ( int x=0; x<number_of_speakers; x++ ) lastOutputs_[x] = 0;
  lastOutput_ = 0;

  // loop through all the writers to the bus
  //std::list<Input>::iterator i;
  //int counter = 0;
  for ( unsigned int i=0; i<writers.size(); i++ ) 
  {
  //for(i = writers.begin(); i != writers.end(); i++)
  //{
    // If the writer Unit is only a single channel, apply Panning as necessary
    if( writers[i].unit->channels() == 1 || writers[i].unit->channels() < number_of_speakers ){
      lastOutput_ = writers[i].unit->tick();
      //this->calculateInserts();
      //this->calculateGains((*i)->gain(), (*i)->pan(), counter);
      for ( int j = 0; j < number_of_speakers; j++ ) 
      {
        //lastOutputs_[j] += (lastOutput_ * gains[j] );
        lastOutputs_[j] += (lastOutput_ * writers[i].gains[j] );
      }
    }
    // If the writer is multichannel and has the same number of channels as the outputbuffer 
    else if( writers[i].unit->channels() > 1 && writers[i].unit->channels() == number_of_speakers ){
      // tick the writer to the temp buffer
      writers[i].unit->tickFrame( lastOutputs_temp_ );
      //this->calculateGains((*i)->gain(), (*i)->pan(), counter); 
      // now add them all up!
      for ( int j=0; j<number_of_speakers; j++ ) {
        lastOutputs_[j] += (lastOutputs_temp_[j] * writers[i].gains[j] );
        //lastOutputs_[j] += (lastOutputs_temp_[j] * gains[j] );
      }  
    }
    else{
      // WARNING!!! THE INPUT BUFFER HAS MORE OR LESS CHANNELS THAN THE OUTPUT BUFFER!!
      // THIS RESIZING BEHAVIOR SHOULD BE HANDLED IN THE UGEN in tickFrame( frames )
    }
   // counter++;
  }
  

}


