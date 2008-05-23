#include "Pan.h"

StkFrames PannerUgen::sin_table_;
StkFrames PannerUgen::cos_table_;

PannerUgen::PannerUgen()
{
  init();
}

PannerUgen::PannerUgen(Ugen *input)
{
  init();
  setInput(input);
}

PannerUgen::~PannerUgen()
{
}

void PannerUgen::init()
{
  TABLE_SIZE = 2048;
  // We need at the very least, 2 channels for output
  setChannels(2);
  // initialize the gains
  for(int i=0; i < 2; i++){
    gains_.push_back( 0.0 );
  }
  // Initialize the sine table
  if ( sin_table_.empty() ) {
    sin_table_.resize( TABLE_SIZE + 1, 1 );
    StkFloat temp = 1.0 / TABLE_SIZE;
    for ( unsigned long i=0; i<=TABLE_SIZE; i++ )
      sin_table_[i] = sin( TWO_PI * i * temp );
  }
  // Initialize the cosine table
  if ( cos_table_.empty() ) {
    cos_table_.resize( TABLE_SIZE + 1, 1 );
    StkFloat temp = 1.0 / TABLE_SIZE;
    for ( unsigned long i=0; i<=TABLE_SIZE; i++ )
      cos_table_[i] = cos( TWO_PI * i * temp );
  }
  // Initialize the multiplier too
  multiplier_ = sqrt(2)/2;  

  // Now initialize the speakers
  initializeSpeakers();
}

void PannerUgen::initializeSpeakers()
{
  speakers_ = new Speaker *[channels()];
  // for 2 speakers, fix the angles
  if(channels() == 2) {
    speakers_[0] = new Speaker( 0, 0, 1);
    speakers_[1] = new Speaker( 1, (M_PI/2), 1);
  }
  // if we have more than 2 speakers, lets assume that they are equidistant
  // b/c chances are that the hardware is 
  // TODO: make this function able to accept arbitrary speaker angles or sets - useful for 5.1 configurations
  else {
    double angle = 0;
    double speaker_spread = (2 * M_PI) / channels(); // in radians
    // now lets initialize the speakers with the values we have decided on
    for( int i = 0; i < channels(); i++ ) {
      //cout << atan2(sin_table_(angle), cos_table_(angle)) << "\n"; // this convert to the interval [-PI,PI]
      speakers_[i] = new Speaker( i, angle, 1);
      angle += speaker_spread;
    }
  }
    
  
}

int PannerUgen::setInput(Ugen *input)
{
  input_ = input;
  setInputPosition(0.0); // set the default position right in the middle
  return 1;
}

int PannerUgen::setInput(Ugen *input, float input_value)
{
  setInput(input);
  setInputPosition(input_value);
  return 1;
}

Ugen* PannerUgen::input(){
  return input_;
}

void PannerUgen::setInputPosition(float value)
{
  input_position_ = value;
  // position is expected in the range [-1,1] (spread of 2)
  if(value > 1 || value < -1) printf("Warning: The Panners input value is out of bounds");
  // convert to positive scale range [0,2]
  value = value + 1;
  // calculate the input angle in radians
  if(channels() == 2){
    input_angle_in_radians_ = PI_OVER_TWO * (value/2); // input_value/2 = x/PI_OVER_TWO
  }
  else{
    input_angle_in_radians_ = TWO_PI * (value/2);      // input_value/2 = x/TWO_PI
  }

  // convert polar to cartesian coordinates for location
  //float location_x_ = ( voice->distance * cos(voice->angle) );
  //float location_y_ = ( voice->distance * sin(voice->angle) );
}

StkFloat PannerUgen::computeSample()
{  
  // The Panner should operate on multichannel buffers
  return lastOutput_;
}


void PannerUgen::computeFrame( void )
{
  //unsigned int nChannels = lastOutputs_.channels();

  // calculate a single multichannel sample of the input
  lastOutput_ = input_->tick();
  
  // since the tests in this loop look for the current speaker angle and the next, 
  // loop to the the number of speakers - 1 so as not to overshoot angles
  
  // TODO: fix this!! this is surely ineffecient, but for the moment its necessary
  // TODO: calculate lookup tables for sin and cosine functions, and possibly even for the gain curves vetween speakers
  // .. for situations where positions are being calculated a number of times for varying positions, this could make a huge difference
  // for(int i=0; i < channels(); i++){
  //   lastOutputs_[i] = 0; // clear out the gains
  // }
  // 
  for(int i=0; i < channels(); i++)
  {
    
    
    float p = (input_position_ + 1.f) * 0.5f;
		
		lastOutputs_[0] = lastOutput_ * (1.f - p);
    lastOutputs_[1] = lastOutput_ * p;
    
  //   double source_angle       = input_angle_in_radians_;
  //   double speaker_angle      = speakers_[i]->angle();
  //   double speaker_next_angle = (i == channels() - 1)? TWO_PI : speakers_[i + 1]->angle();
  // 
  //   // scale the angle for the equation (Computer Music Tutorial p.xxx)
  //   double this_angle = (source_angle - speaker_angle) - (speaker_next_angle - speaker_angle)/2;
  //   
  //   if(source_angle >= speaker_angle && source_angle < speaker_next_angle) {
  //     // a little reusable variable action
  //     StkFloat sin_angle = sin(this_angle);
  //     StkFloat cos_angle = cos(this_angle);
  //     // gain speaker 1
  //     gains_[i]  = (multiplier_ * (cos_angle - sin_angle));
  //     // gain speaker 2
  //     if(i == channels() - 1){
  //       gains_[0] = (multiplier_ * (cos_angle + sin_angle));
  //     }
  //     else{
  //       gains_[i+1] = (multiplier_ * (cos_angle + sin_angle));
  //     }
  //   }
    // if the source angle is greater the speaker's angle and its next neighbor,
    // it gets no volume at all (this setting may not be necessary since default = 0) 
  }
}

