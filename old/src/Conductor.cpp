#include "Conductor.h"
#define _DEBUG_

  
//const double TWO_PI = M_PI * 2;

Conductor::Conductor(int channels)
{
  space_scaler = 10; // multiplier of default locations from [0,1]
  number_of_speakers = number_of_channels = channels;
  lastOutputs_.resize( 1, channels );
  initializeSpeakers();
}

Conductor :: ~Conductor() { }


void Conductor::initializeSpeakers()
{
  speakers = new Speaker *[number_of_speakers];
  // make sure there are at least 2 output channels. mono sucks.
  if(number_of_speakers < 2) {
    errorString_ << "Conductor::initializeSpeakers(): I need at least 2 speakers to run this program!";
    handleError( StkError::WARNING );
  }
  // there are enough output channels so handle them
  else {
    // for 2 speakers, fix the angles
    if(number_of_speakers == 2) {
      speakers[0] = new Speaker( 0, 0, 1);
      speakers[1] = new Speaker( 1, (M_PI/2), 1);
    }
    // if we have more than 2 speakers, lets assume that they are equidistant
    // b/c chances are that the hardware is 
    // TODO: make this function able to accept arbitrary speaker angles or sets - useful for 5.1 configurations
    else {
      double angle = 0;
      double speaker_spread = (2 * M_PI) / number_of_speakers; // in radians
      // now lets initialize the speakers with the values we have decided on
      for( int i = 0; i < number_of_speakers; i++ ) {
        cout << atan2(sin(angle), cos(angle)) << "\n"; // this convert to the interval [-PI,PI]
        speakers[i] = new Speaker( i, angle, 1);
        angle += speaker_spread;
      }
    }
    
  }
}

void Conductor::addInstrument( Organism *instrument)
{
  // initialize a voice
  Conductor::Voice voice;
  voice.instrument = instrument;
  voice.noteNumber = -1;
  // initialize the voice gains
  for(int i=0; i < number_of_speakers; i++){
      voice.gains.push_back( 0 );
  }
  setVoicePosition(&voice);
  
  voices_.push_back( voice );
#if defined(_DEBUG_)
  cout << voices_.size() << ": angle: " << (voice.angle * 180/M_PI) << ", distance: " << voice.distance << "\n";
#endif
}

void Conductor::setVoicePosition(Conductor::Voice *voice)
{
  Rand r;  
  // this scaler expands the width of the field (in meters)
  // The default distances and x y coords are set randomized in the range [0,1]
  // the scaler can exand this space
  
  if(number_of_speakers == 2){
    voice->angle    = r.randdouble(0, PI_OVER_TWO);
    voice->distance = r.randdouble(-1, 1) * space_scaler;
  }
  else{
    voice->angle    = r.randdouble(0, TWO_PI);
    voice->distance = r.randdouble(-1, 1) * space_scaler;
  }

  // set location in the message sent to one another
  ostringstream angle_temp, distance_temp, location_x, location_y;
  angle_temp    << voice->angle;
  distance_temp << voice->distance;
  
  // convert polar to cartesian coordinates for location
  float location_x_ = ( voice->distance * cos(voice->angle) );
  float location_y_ = ( voice->distance * sin(voice->angle) );
  // create the strings for the message
  location_x    << location_x_;
  location_y    << location_y_;
  // set the voice's location
  voice->instrument->set_location_x( location_x_ );
  voice->instrument->set_location_y( location_y_ );
  // set the voice's location in the message too
  voice->instrument->_message->setValue( "location_x", location_x.str() );
  voice->instrument->_message->setValue( "location_y", location_y.str() );
  voice->instrument->_message->setValue( "angle", angle_temp.str() );
  voice->instrument->_message->setValue( "distance", distance_temp.str() );
  
  // now set the gains of the voice
  setVoiceGains(voice);
}

// calculate the gains (effectivly a cache) based on the angles between the speakers
// here is where you use the fancy equation

// The equation for the gain between any 2 speakers was taken from 
// Curtis Roads "The Computer Music Tutorial" section on spacialization
void Conductor::setVoiceGains(Conductor::Voice *voice)
{
  double multiplier = sqrt(2)/2;
  float distance_percentage = fabs(voice->distance) / space_scaler;
  cout << "VOICE distance_percentage " <<  distance_percentage << endl;
  // since the tests in this loop look for the current speaker angle and the next, 
  // loop to the the number of speakers - 1 so as not to overshoot angles
  
  // TODO: fix this!! this is surely ineffecient, but for the moment its necessary
  // TODO: calculate lookup tables for sin and cosine functions, and possibly even for the gain curves vetween speakers
  // .. for situations where positions are being calculated a number of times for varying positions, this could make a huge difference
  for(int i=0; i < number_of_speakers; i++){
    voice->gains[i] = 0; // clear out the gains
  }
  for(int i=0; i < number_of_speakers; i++)
  {
    double source_angle       = voice->angle;
    double speaker_angle      = speakers[i]->angle();
    double speaker_next_angle = (i == number_of_speakers - 1)? TWO_PI : speakers[i + 1]->angle();

    // scale the angle for the equation (Computer Music Tutorial p.xxx)
    double this_angle = (source_angle - speaker_angle) - (speaker_next_angle - speaker_angle)/2;
    
    if(source_angle >= speaker_angle && source_angle < speaker_next_angle) {
      double sin_angle = sin(this_angle);
      double cos_angle = cos(this_angle);
      // gain speaker 1
      voice->gains[i]  = distance_percentage * (multiplier * (cos_angle - sin_angle));
      // gain speaker 2
      if(i == number_of_speakers - 1){
        voice->gains[0] = distance_percentage * (multiplier * (cos_angle + sin_angle));
      }
      else{
        voice->gains[i+1] = distance_percentage * (multiplier * (cos_angle + sin_angle));
      }
    }
    // if the source angle is greater the speaker's angle and its next neighbor,
    // it gets no volume at all (this setting may not be necessary since default = 0)   
  }
#if defined(_DEBUG_)  
  for(int i=0; i < number_of_speakers; i++)
  {
    cout << "voice gain " << i << ": "<< voice->gains[i] << "\n";
  }
#endif
}



void Conductor::removeInstrument( Organism *instrument )
{
  bool found = false;
  std::vector< Conductor::Voice >::iterator i;
  for ( i=voices_.begin(); i!=voices_.end(); ++i ) {
    if ( (*i).instrument != instrument ) continue;
    voices_.erase( i );
    found = true;
    break;
  }

  if ( !found ) {
    errorString_ << "Conductor::removeInstrument: instrument pointer not found in current voices!";
    handleError( StkError::WARNING );
  }
}
 

long Conductor :: noteOn(StkFloat noteNumber, StkFloat amplitude, int channel )
{
  unsigned int i;
  StkFloat frequency = (StkFloat) 220.0 * pow( 2.0, (noteNumber - 57.0) / 12.0 );
  for ( i=0; i<voices_.size(); i++ ) {
    if (voices_[i].noteNumber < 0 && voices_[i].channel == channel) {
	    voices_[i].tag = tags_++;
      voices_[i].channel = channel;
      voices_[i].noteNumber = noteNumber;
      voices_[i].frequency = frequency;
	    voices_[i].instrument->noteOn( frequency, amplitude * ONE_OVER_128 );
      voices_[i].sounding = 1;
      return voices_[i].tag;
    }
  }

  // All voices are sounding, so interrupt the oldest voice.
  int voice = -1;
  for ( i=0; i<voices_.size(); i++ ) {
    if ( voices_[i].channel == channel ) {
      if ( voice == -1 ) voice = i;
      else if ( voices_[i].tag < voices_[voice].tag ) voice = (int) i;
    }
  }

  if ( voice >= 0 ) {
    voices_[voice].tag = tags_++;
    voices_[voice].channel = channel;
    voices_[voice].noteNumber = noteNumber;
    voices_[voice].frequency = frequency;
    voices_[voice].instrument->noteOn( frequency, amplitude * ONE_OVER_128 );
    voices_[voice].sounding = 1;
    return voices_[voice].tag;
  }

  return -1;
}

void Conductor :: noteOff( StkFloat noteNumber, StkFloat amplitude, int channel )
{
  for ( unsigned int i=0; i<voices_.size(); i++ ) {
    if ( voices_[i].noteNumber == noteNumber && voices_[i].channel == channel ) {
      voices_[i].instrument->noteOff( amplitude * ONE_OVER_128 );
      voices_[i].sounding = -muteTime_;
    }
  }
}

void Conductor :: noteOff( long tag, StkFloat amplitude )
{
  for ( unsigned int i=0; i<voices_.size(); i++ ) {
    if ( voices_[i].tag == tag ) {
      voices_[i].instrument->noteOff( amplitude * ONE_OVER_128 );
      voices_[i].sounding = -muteTime_;
      break;
    }
  }
}

void Conductor :: setFrequency( StkFloat noteNumber, int channel )
{
  StkFloat frequency = (StkFloat) 220.0 * pow( 2.0, (noteNumber - 57.0) / 12.0 );
  for ( unsigned int i=0; i<voices_.size(); i++ ) {
    if ( voices_[i].channel == channel ) {
      voices_[i].noteNumber = noteNumber;
      voices_[i].frequency = frequency;
      voices_[i].instrument->setFrequency( frequency );
    }
  }
}

void Conductor :: setFrequency( long tag, StkFloat noteNumber )
{
  StkFloat frequency = (StkFloat) 220.0 * pow( 2.0, (noteNumber - 57.0) / 12.0 );
  for ( unsigned int i=0; i<voices_.size(); i++ ) {
    if ( voices_[i].tag == tag ) {
      voices_[i].noteNumber = noteNumber;
      voices_[i].frequency = frequency;
      voices_[i].instrument->setFrequency( frequency );
      break;
    }
  }
}

void Conductor :: pitchBend( StkFloat value, int channel )
{
  StkFloat pitchScaler;
  if ( value < 64.0 )
    pitchScaler = pow(0.5, (64.0-value)/64.0);
  else
    pitchScaler = pow(2.0, (value-64.0)/64.0);
  for ( unsigned int i=0; i<voices_.size(); i++ ) {
    if ( voices_[i].channel == channel )
      voices_[i].instrument->setFrequency( (StkFloat) (voices_[i].frequency * pitchScaler) );
  }
}

void Conductor :: pitchBend( long tag, StkFloat value )
{
  StkFloat pitchScaler;
  if ( value < 64.0 )
    pitchScaler = pow(0.5, (64.0-value)/64.0);
  else
    pitchScaler = pow(2.0, (value-64.0)/64.0);
  for ( unsigned int i=0; i<voices_.size(); i++ ) {
    if ( voices_[i].tag == tag ) {
      voices_[i].instrument->setFrequency( (StkFloat) (voices_[i].frequency * pitchScaler) );
      break;
    }
  }
}

void Conductor :: controlChange( int number, StkFloat value, int channel )
{
  for ( unsigned int i=0; i<voices_.size(); i++ ) {
    if ( voices_[i].channel == channel )
      voices_[i].instrument->controlChange( number, value );
  }
}

void Conductor :: controlChange( long tag, int number, StkFloat value )
{
  for ( unsigned int i=0; i<voices_.size(); i++ ) {
    if ( voices_[i].tag == tag ) {
      voices_[i].instrument->controlChange( number, value );
      break;
    }
  }
}

void Conductor :: silence( void )
{
  for ( unsigned int i=0; i<voices_.size(); i++ ) {
    if ( voices_[i].sounding > 0 )
      voices_[i].instrument->noteOff( 0.5 );
  }
}

// This tick() is also called by the frames version of the tick.
// .. it is in this function that 
// 1) all the instruments are looped through
// 2) in this loop, the instrument is tick()ed
// 3) then, the intrument's gains are looped through, multiplying the ticked value by the gain
// .. assigning the resulting value to the something like _lastOutputs[num_of_channels]
// 4) this value is then assigned to the correct frame.. whatever that maybe :(

void Conductor :: computeFrame( void )
{
  // make sure that the samples are cleared out
  for ( int i=0; i<number_of_speakers; i++ ) {
    lastOutputs_[i] = 0;
  }
  //lastOutput_ = lastOutputLeft_ = lastOutputRight_ = 0.0;
  // loop through all the voices and tick() them, producing a single sample
  for ( int i=0; i<voices_.size(); i++ ) 
  {
    // calculate the output of the instrument
    lastOutput_ = voices_[i].instrument->tick();
    for ( int j=0; j<number_of_speakers; j++ ) 
    {
      lastOutputs_[j] += (lastOutput_ * voices_[i].gains[j]);
    }
  }
  // now lower the calculated volumes to something reasonable;
  // double divisor = (voices_.size());
  double divisor = 1; // no volume cut
  
  for ( int i=0; i<number_of_speakers; i++ ) {
    lastOutputs_[i] = lastOutputs_[i] / divisor;
  }

}

StkFrames& Conductor :: tickFrame( StkFrames& frames )
{
  unsigned int nChannels = lastOutputs_.channels();
  if ( nChannels == 0 ) {
    errorString_ << "Conductor::tickFrame(): no data has been loaded!";
    handleError( StkError::WARNING );
    return frames;
  }

  if ( nChannels != frames.channels() ) {
    errorString_ << "Conductor::tickFrame(): incompatible channel value in StkFrames argument!";
    handleError( StkError::FUNCTION_ARGUMENT );
  }

  unsigned int j;
  if ( nChannels == 1 || frames.interleaved() ) {
    unsigned int counter = 0;
    for ( unsigned int i=0; i<frames.frames(); i++ ) {
      this->computeFrame();
      for ( j=0; j<nChannels; j++ )
        frames[counter++] = lastOutputs_[j];
    }
  }
  else { // non-interleaved data
    unsigned int hop = frames.frames();
    unsigned int index;
    for ( unsigned int i=0; i<frames.frames(); i++ ) {
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

StkFloat Conductor :: tick()
{
  lastOutput_ = lastOutputLeft_ = lastOutputRight_ = 0.0;
  for ( unsigned int i=0; i<voices_.size(); i++ ) {
    if ( voices_[i].sounding != 0 ) {
      lastOutput_ += voices_[i].instrument->tick();
      lastOutputLeft_ += voices_[i].instrument->lastOutLeft();
      lastOutputRight_ += voices_[i].instrument->lastOutRight();
    }
    if ( voices_[i].sounding < 0 ) {
      voices_[i].sounding++;
      if ( voices_[i].sounding == 0 )
        voices_[i].noteNumber = -1;
    }
  }
  return lastOutput_ / voices_.size();
}

StkFloat *Conductor :: tick(StkFloat *vector, unsigned int vectorSize)
{
  for (unsigned int i=0; i<vectorSize; i++)
    vector[i] = tick();

  return vector;
}

StkFrames& Conductor :: tick( StkFrames& frames, unsigned int channel )
{
  if ( channel == 0 || frames.channels() < channel ) {
    errorString_ << "Conductor::tick(): channel argument (" << channel << ") is zero or > channels in StkFrames argument!";
    handleError( StkError::FUNCTION_ARGUMENT );
  }

  if ( frames.channels() == 1 ) {
    for ( unsigned int i=0; i<frames.frames(); i++ )
      frames[i] = tick();
  }
  else if ( frames.interleaved() ) {
    unsigned int hop = frames.channels();
    unsigned int index = channel - 1;
    for ( unsigned int i=0; i<frames.frames(); i++ ) {
      frames[index] = tick();
      index += hop;
    }
  }
  else {
    unsigned int iStart = (channel - 1) * frames.frames();
    for ( unsigned int i=0; i<frames.frames(); i++ )
      frames[iStart + i] = tick();
  }

  return frames;
}

StkFloat Conductor :: lastOut() const
{
  return lastOutput_;
}

StkFloat Conductor :: lastOutLeft() const
{
  return lastOutputLeft_;
}

StkFloat Conductor :: lastOutRight() const
{
  return lastOutputRight_;
}

