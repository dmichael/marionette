#ifndef SPEAKER_H
#define SPEAKER_H
#include <iostream>
using namespace std;
class Speaker
{
public:

  //! Class constructor for polar coordinates.
  Speaker( const int channel, const double angle, const double distance);

  //! Class constructor for cartesian coordinates.  
  Speaker( const int channel, const double x, const double y, const double distance);

  //! Class destructor.
  ~Speaker();
  
  int channel();
  void setChannel(const int);
  int gain();
  void setGain(const int gain);
  double angle();
  void setAngle(double);
  double distance();
  void setDistance(double);

private:
  int    _channel;
  double _angle;
  double _distance;
  double _gain;
  
};
#endif
