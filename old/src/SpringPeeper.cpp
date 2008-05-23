#include "SpringPeeper.h"

SpringPeeper::SpringPeeper(Dispatcher *d) : Anuran()
{ 
  Rand r;
  ostringstream sample;

  // Set the ID
  set_id( );   
  
  // Period (T) in seconds                   
  setPeriod( r.randfloat(0.8, 1.0) );
  
  // Effector delay (t) in seconds
  //(always supposedly short-ish, but paper gives as related to decending length)
  setEffectorDelay( r.randfloat(0.15, 0.15) );
  
  // Decending length (r) 
  // slightly longer than effector delay [20 - 120]ms  or [0.02 - 0.120]s      
  setDecendingLength( effector_delay() + 0.16 ); 
   
  setPrcSlope( r.randfloat(0.01, 0.2) );       // PRC slope
  setSpl( r.randfloat(25, 35) );                // sound pressure level (SPL)
  
  // Maximum neighbors to attend to
  setMaxNeighbors( 2 );
  
  // set the sample voice
  sample << "peeper.wav";
  setSample(sample.str());

  // Dispatcher connection (relies on a channel definition)  
  connectToSignal("spring_peeper", d);

}

SpringPeeper::~SpringPeeper() { }
