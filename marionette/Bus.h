/***************************************************/
/*! \class Bus
    \brief STK basic mixer class.

    This class keeps a vector list of inputs and sums them to compute the output frames

    by Perry R. Cook and Gary P. Scavone, 1995 - 2007.
*/
/***************************************************/

#ifndef STK_BUS_H
#define STK_BUS_H

#include "Unit.h"
#include "Effects.h"
#include <list>
#include <vector>

class Bus : public Unit
{
public:
  //! Default constructor.
  Bus( void );
  
  //! Overloaded constructor.
  Bus( int );

  //! Class destructor.
  ~Bus( void );
  
  char * name( void ){ return "Bus"; }
  
  int add(Unit*);
  
  int remove(Unit*);
  
  int addInsert(FX*);
  
  int removeInsert(FX*);
  
  void clearInputs();

  float masterGain();
  
  void setMasterGain(float);

	std::list<FX*> inserts;

  struct Input {
    Unit *unit;
    int sounding;
    int channel;
    double angle;
    double distance;
    // calculated gains
    // these gains are calculated for pans from unit params
    std::vector<StkFloat> gains;
    // Default constructor.
    Input() :sounding(0), channel(0) {}
  };
  
  std::vector<Input> writers;
	  
protected:
  void calculateGains( Bus::Input *input );
  void calculateGains( float, float, int );
  void calculateInserts( void );
  
  int number_of_speakers;
  StkFrames pans; // buffer to hold pan changes
  StkFrames gains;
  float master_gain;
  StkFloat computeSample( void );
  void computeFrame( void );
};

#endif