 /***************************************************/
/*! \class Unit
    \brief STK abstract unit generator parent class.

    This class provides common functionality for
    STK unit generator sample-source subclasses.

    by Perry R. Cook and Gary P. Scavone, 1995 - 2007.
*/
/***************************************************/

#include "Unit.h"

Unit :: Unit() : Stk()
{
  lastOutput_ = 0.0;
  pan_ = 0.0;
  gain_ = 1.0;
  clock = 0;
  // default frame size is 1 -- that is, one channel
  // So Bus objects and other implicitly multichannels objects should take care to handle this
  setChannels(1);
}

Unit :: ~Unit()
{
}

int Unit :: channels()
{
  return lastOutputs_.channels();
}

void Unit :: setChannels(int num_channels)
{
  this->lastOutputs_.resize( 1, num_channels );
  this->lastOutputs_temp_.resize( 1, num_channels );
}


float Unit :: pan()
{
  return pan_;
}

float Unit :: gain()
{
  return gain_;
}

void Unit :: setGain( float gain )
{
  if(gain == gain_) return;
  gain_ = gain;
}


void Unit :: setPan( float pan )
{
  if(pan == pan_) return;
  pan_ = pan;
}

// Calls computeSample();
StkFloat Unit :: tick( void )
{
  return computeSample();
}

// Ticks onto a single channel of a multichannel buffer (frames)
StkFrames& Unit :: tick( StkFrames& frames, unsigned int channel )
{
  if ( channel >= frames.channels() ) {
    errorString_ << "Unit::tick(): channel and StkFrames arguments are incompatible!";
    handleError( StkError::FUNCTION_ARGUMENT );
  }
  // One channel tick
  if ( frames.channels() == 1 ) {
    for ( unsigned int i=0; i<frames.frames(); i++ )
      frames[i] = this->computeSample();
      // dont set pan for a single channel
  }
  // Interleaved multichannel tick
  else if ( frames.interleaved() ) {
    unsigned int hop = frames.channels();
    unsigned int index = channel;
    for ( unsigned int i=0; i<frames.frames(); i++ ) {
      frames[index] = this->computeSample();
      index += hop;
    }
  }
  // Non-interleaved multichannel tick
  else {
    unsigned int iStart = channel * frames.frames();
    for ( unsigned int i=0; i<frames.frames(); i++, iStart++ )
      frames[iStart] = this->computeSample();
  }

  return frames;
}

// Calls computeFrame()
StkFrames& Unit :: tickFrame( StkFrames& frames )
{ 
  unsigned int nChannels = lastOutputs_.channels();
  if ( nChannels == 0 ) {
    errorString_ << "Unit::tickFrame(): no data has been loaded!";
    handleError( StkError::WARNING );
    return frames;
  }

  if ( nChannels != frames.channels() ) {
    errorString_ << "Unit::tickFrame(): incompatible channel value in StkFrames argument!";
    handleError( StkError::FUNCTION_ARGUMENT );
  }

  unsigned int j;
  unsigned int i;
  if ( nChannels == 1 || frames.interleaved() ) {
    unsigned int counter = 0;

    for ( i=0; i<frames.frames(); i++ ) {
      this->computeFrame();
      for ( j=0; j<nChannels; j++ )
        frames[counter++] = lastOutputs_[j];
    }
  }
  else { // non-interleaved data
    unsigned int hop = frames.frames();
    unsigned int index;
    for ( i=0; i<frames.frames(); i++ ) {
      this->computeFrame();
      index = i;
      for ( j=0; j<nChannels; j++ ) {
        frames[index] = lastOutputs_[j];
        index += hop;
      }
    }
  }

  return frames;
}
