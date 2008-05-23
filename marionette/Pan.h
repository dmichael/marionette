/***************************************************/
/*! \class PannerUgen
    \brief STK wrapper class for multichannel panning
    \author David M. Michael
    \date Feb, 9 2008
    
    Input to the Panner should be [-1,1]
*/
/***************************************************/

#ifndef STK_PANNERUGEN_H
#define STK_PANNERUGEN_H

#include "Unit.h"
#include "Speaker.h"
#include <math.h>
#include <vector>

const double PI_OVER_TWO = M_PI/2;

class PannerUgen : public Ugen
{
public:
  //! Default constructor.
  PannerUgen( void );

  PannerUgen(Ugen *);

  //! Class destructor.
  virtual ~PannerUgen( void );
  
  char * name( void ){ return "Panner"; }
  
  void init();
  
  void initializeSpeakers();
  
  int setInput(Ugen *);

  int setInput(Ugen *, float);

  void setInputPosition(float);
  
  Ugen* input();
    
protected:
  float input_position_;
  unsigned long TABLE_SIZE;
  vector<StkFloat> gains_;
  Speaker **speakers_;
  Ugen* input_;
  float input_angle_in_radians_;
  static StkFrames sin_table_;
  static StkFrames cos_table_;
  float multiplier_; // used for power calculations
  StkFloat computeSample( void );
  void computeFrame( void );
};

#endif