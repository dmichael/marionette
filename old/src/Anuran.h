/*!
    \class Anuran
    \brief STK sample-based generic Anuran class implementing the Greenfield call-timing algorithm
    
    T' = s[(d + l/v) − (r − t)] + (T + e) + (y − x)
    
    where:
    T  = free running call period (seconds)
    T' = modified call period after receipt of stimulus (seconds) 
    d  = time elapsed since onset of last call (seconds)
    l  = distance of the stimulus (meters)
    v  = the speed of sound (344 m/s)
    r  = length of the decending slope
    t  = effector delay
    s  = phase response curve
    e  = stochastic element
    y  = length of the stimulus call
    x  = length of the call

    Usage ::
    You can specify the maximum polyphony (maximum
    number of simultaneous voices) in Anuran.h.
    
    by David M. Michael 7.4.2007 for Unnature
    Adapted from VoicDrum by Perry R. Cook and Gary P. Scavone, 1995 - 2005.
    
*/


#ifndef STK_ANURAN_H
#define STK_ANURAN_H

#include <cmath>
#include <iostream>
#include <sstream>
#include "Rand.h"
#include "Genome.h"
#include "Organism.h"
#include "FileWvIn.h"
#include "OnePole.h"
#include "Message.h"
#include "Dispatcher.h"
#include "sigslot.h"

using namespace std;
  
class Anuran : public Organism 
{
public:
  // static variables to be shared with all Anurans
  static const int    VOICE_NUMWAVES  = 11;
  static const int    VOICE_POLYPHONY = 15;


  //! Class constructor.
  /*
    This constructor is used mostly in extention in children classes who need to set values without a Genome
  */
  Anuran();
  
  //! Class constructor.
  Anuran(Dispatcher *dispatcher, Genome *genome);
  
  void initialize(void);
  
  //! Class destructor.
  ~Anuran();
  
  
  static float calculateAttenuation(float spl, float distance);

  static float calculateDistance(float x1, float y1, float x2, float y2);
  
  
  /* 
    GETTERS AND SETTERS 
    ---------------------------------------------------- 
  */
  //! Get ID
  int id( );
  string id_str( );
  
  //! Set ID
  void setId( int);
  
  void set_id( );
  
  //! get period
  double period(){ return _PERIOD; };
  
  //! set period and related params
  void setPeriod(float period) {
    _PERIOD =  _period  = period;
    _period_in_frames    = (unsigned long) (_period * Stk::sampleRate());
    _period_overflow     = 0;
  };
  
  //! Get decending length
  float decending_length() { return _decending_length; };
  
  //! Set decending length
  void setDecendingLength( float length ) {
    _decending_length = length;
  };
  
  //! Get effector delay
  float effector_delay() { return _effector_delay; };
  
  //! Set effector delay
  void setEffectorDelay( float delay ) {
    _effector_delay = delay;
  };
  
  //! Get PRC slope
  float prc_slope() { return _prc_slope; };
  
  //! Set PRC slope
  void setPrcSlope( float prc_slope ) {
    _prc_slope = prc_slope;
  };
  
  //! Get signal name
  string signal_name() { return _signal_name; };
  
  //! Set signal
  void connectToSignal( string, Dispatcher*);
  
  //! Get sample
  string sample( int index ) { return _voice_names[index]; };

  //! Set sample
  void setSample( string sample ) {
      _voice_names.push_back( sample );
      // lets set the call duration
      loadSample(0);
      _call_duration = getSampleDuration(0)/ Stk::sampleRate();
  };
  
  float spl(int);

  void setSpl(float);
   
  Message* message();
  
  void setMessage();
  
  int maxNeighbors();
  
  void setMaxNeighbors(int);
  
  float costOfSignaling();
  
  void setCostOfSignaling( float );
  
  void setIgnoreEnergy();
  
  /* ---------------------------------------------------- */
  
  void connectDispatcher();
  
  //! Class constructor.
  void loadSample(int);
  
  static bool sortNeighbors(float, float);
  
  
  //! Start a note with the given drum type and amplitude.
  void noteOn(StkFloat instrument, StkFloat amplitude);
  
  //! Stop a note with the given amplitude (speed of decay).
  void noteOff(StkFloat amplitude);
  
  bool attendToNeighbor( float );

  void forgetNeighbors( void );

  void expendEnergy( void );
  
  void regainEnergy( void );
  
  bool hasEnergyToSignal();
   
  //! Receive method to handle messages from the dispatcher
  void receive(Message*);
  
  void triggerSignal();
  
  //! 
  void incrementFrame(void);
  
  //! The computation method - very important
  StkFloat computeSample( void );
  
  //! Class constructor.
  StkFloat getTime( void ) const;

  //! Class constructor.
  StkFloat getElapsedTime( void ) const;

  //! Return the duration of the call (sample) in frames
  unsigned long getSampleDuration(int);
  
  //! Class constructor.
  unsigned long getFrameCount( void ) const;





//private:
  //! reference to the Genome
  Genome *genome;

  StkFloat elapsed_time;          //! Elapsed time since object creation (seconds)
  double elapsed_time_since_call;  //! (d) Time elapsed since onset of last call (seconds) 

  double pulse_duration;
  float e;                      //! Stochastic element
  float correction;
  float correction2;
  float temp;
  unsigned long elapsed_time_in_frames;
  
  // These variables could be managed outside the object (environment? dispatcher?)
  float stimulus_distance;   //! (l) Distance of the stimulus (meters)

  // Fields for dealing with the samples
  FileWvIn *waves_[VOICE_POLYPHONY];
  OnePole  filters_[VOICE_POLYPHONY];
  vector<int> soundOrder_;
  vector<int> soundNumber_;
  unsigned long frameCounter_;
  int nSounding_;
  
    
private:

  static int last_id;  //! ids for various identification purposes.
  int    _id;
  string _id_str;
  
  //! reference to the Dispatcher
  Dispatcher    *_dispatcher;
  // Message       *_message; // defined in parent class
  string         _signal_name;
  vector<string> _voice_names;
  
  // important internal fields
  //! Fields for use in the Greenfield call-timing algorithm
  double _PERIOD;               //! (T) Free running call period (seconds)
  double _period;               //! (T') Modified call period after receipt of stimulus (seconds)       
  double _period_overflow;      //! Storage variable for edge case (seconds)
  unsigned long _period_in_frames;

  float  _decending_length;     //! (r) Length of the decending slope        
  float  _effector_delay;       //! (t) Effector delay  
  float  _prc_slope;            //! (s) Phase Response Curve  (PRC)
  double _call_duration;        //! (x) Length of the call  

  // sound pressure level (SPL) of the call at the source (dB)
  float  _spl;            
  float  _gain;
  
  // Energy for calling... this should aid in provoking unison bout singing
  float _energy;
  float _cost_of_signaling;
  int   _number_of_calls;
  bool  _ignore_energy;
  
  vector<float> _neighbors;
  int   _max_neighbors;
  
  bool has_energy_to_signal;
};

#endif
