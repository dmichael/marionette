#include "Speaker.h"

Speaker::Speaker( const int channel, const double angle, const double distance)
{
  cout << "Created a speaker (channel:"<< channel<< ", rad-angle:"<< angle <<", distance:"<< distance <<" )\n";
  setChannel(channel);
  setAngle(angle);
  setDistance(distance);
}

Speaker::Speaker( const int channel, const double x, const double y, const double distance)
{
  // convert cartiesian coordinates to radians
  //  double atan2(y/x);
}

Speaker::~Speaker()
{
  
}

int Speaker::channel(){  return _channel; }
void Speaker::setChannel(const int channel){ _channel = channel; }
int Speaker::gain(){  return _channel; }
void Speaker::setGain(const int gain){ _gain = gain; }
double Speaker::angle(){ return _angle; }
void Speaker::setAngle(double angle){ _angle = angle; }
double Speaker::distance(){ return _distance; }
void Speaker::setDistance(double distance){ _distance = distance; }
