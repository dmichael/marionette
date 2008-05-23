#include "Anuran.h"
//#ifndef _STK_DEBUG_
//#define _STK_DEBUG_
//#endif
int Anuran::last_id = 0;  

Anuran::Anuran()
{  
  initialize();
}

Anuran::Anuran(Dispatcher *dispatcher, Genome *genome_)
{
  initialize();  
  _dispatcher      = dispatcher;
  genome          = genome_;
  // Store the IDs
  _id_str            = genome->genes["id"];
  _id                = atoi(genome->genes["id"].c_str());           
  // Initialize all our variables from the genome
  // period
  setPeriod( atof(genome->genes["period"].c_str()) );
  
  _decending_length  = atof(genome->genes["decending_length"].c_str());
  _effector_delay    = atof(genome->genes["effector_delay"].c_str());
  _prc_slope         = atof(genome->genes["prc_slope"].c_str());


  // Dispatcher initialization
  _signal_name         = genome->genes["channel"];
  // Dispatcher connection
  _dispatcher->defineChannel(_signal_name);
  _dispatcher->channels[_signal_name]->connect(this, &Anuran::receive);

  _voice_names.push_back(genome->genes["sample"]);
  // lets set the call duration
  loadSample(0);
  _call_duration = getSampleDuration(0);
  


  // Setup the Message to be sent on each call
  // sender_id

  _message->setValue("sender_id", genome->genes["id"]);

  // call_duration
  ostringstream string_temp;
  string_temp << _call_duration;
  _message->setValue("call_duration", string_temp.str());

  // Let's report what happened
 // cout << "Anuran born at " << id_str << " period->"<< _PERIOD << " _period_in_frames->"<< _period_in_frames << "\n";
}

Anuran::~Anuran()
{
  delete _message;
}

void Anuran::initialize()
{
  _message = new Message();
  // zero out some initial variables
  elapsed_time = 0; 
  elapsed_time_since_call = 0; 
  e = 0; 
  correction = 0; 
  correction2 = 0; 
  temp = 0; 
  elapsed_time_in_frames = 0; 
  frameCounter_ = 0; 
  nSounding_ = 0;
  // this is a conservative estimate for selective attention
  // some may not want any neighbors at all - so use setter
  _max_neighbors = 2; 
  
  // energy related intialization
  _energy = 100;
  _cost_of_signaling = 10;
  _number_of_calls = 0;
  _ignore_energy = false;
  
  soundOrder_       = std::vector<int> (VOICE_POLYPHONY, -1);
  soundNumber_      = std::vector<int> (VOICE_POLYPHONY, -1);
  float SPEED_OF_SOUND = 344;
}

int Anuran::id( )
{ 
  return _id; 
}

string Anuran::id_str()
{ 
  return _id_str; 
}

void Anuran::setId( int id )
{
  _id = id;
  ostringstream id_temp;
  id_temp << id;
  _id_str = id_temp.str();
};

void Anuran::set_id()
{
  _id = last_id;
  cout<<"ID->"<< last_id<<endl;
  ostringstream id_temp;
  id_temp <<  last_id;
  _id_str = id_temp.str();
  last_id++;
};

float Anuran::spl( int index )
{ 
  return _spl; 
};

void Anuran::setSpl( float spl )
{
  _spl  = spl;
  // TODO: find a better equation for extracting gain from SPL
  _gain = spl / 100;
}

Message* Anuran::message( )
{ 
  return _message; 
}
 
void Anuran::setMessage( )
{
  ostringstream call_duration, spl;
  call_duration << _call_duration;
  spl << _spl;    

  _message->setValue("sender_id", id_str());
  _message->setValue("call_duration", call_duration.str());
  _message->setValue("spl", spl.str());
}
 
int Anuran::maxNeighbors( )
{
  return _max_neighbors;
}

void Anuran::setMaxNeighbors(int max)
{
  _max_neighbors = max;
}

void Anuran::connectDispatcher()
{
  if ( _signal_name.empty() ) {
    errorString_ << "Anuran::connectDispatcher: define a _signal_name before connecting it to the Dispatcher";
    handleError( StkError::WARNING );
  }
  // Dispatcher connection
  _dispatcher->defineChannel(_signal_name);
  _dispatcher->channels[_signal_name]->connect(this, &Anuran::receive);
}

void Anuran::loadSample(int i)
{
  cout << "Anuran::loadSample " << i << "\n";
  Rand r;
  try {
    waves_[i] = new FileWvIn( (string("../audio/") + _voice_names[i]).c_str() );
  }
  catch (StkError &) {
    exit(0);
  }

  //waves_[i].openFile( (string("rawwaves/") + voiceNames[i]).c_str(), false, true );
  
  double rate = 1.0;
  rate = waves_[i]->getFileRate() / Stk::sampleRate();
  rate *= r.randfloat(0.93, 1.05);
  cout << "sample rate: " << rate << "\n";
  waves_[i]->setRate( rate );
}

unsigned long Anuran::getSampleDuration(int i)
{
  return waves_[i]->getSize();
}

StkFloat Anuran::getElapsedTime(void) const
{
  return (StkFloat) frameCounter_ / Stk::sampleRate();
}

unsigned long Anuran::getFrameCount( void ) const
{
  return frameCounter_;
}

StkFloat Anuran::getTime( void ) const
{
  return (StkFloat) frameCounter_ / Stk::sampleRate();
}

void Anuran::connectToSignal( string signal_name, Dispatcher *dispatcher )
{
  _signal_name = signal_name;
  if ( _signal_name.empty() ) {
    errorString_ << "Anuran::setSignal: define a _signal_name before connecting it to the Dispatcher";
    handleError( StkError::WARNING );
  }
  // Dispatcher connection
  _dispatcher = dispatcher;
  _dispatcher->defineChannel(_signal_name);
  _dispatcher->channels[_signal_name]->connect(this, &Anuran::receive);
  // Set the message
  setMessage();
}


float Anuran::costOfSignaling(){
  return _cost_of_signaling;
};

void Anuran::setCostOfSignaling( float cost ){
  _cost_of_signaling = cost;  
};

void Anuran::setIgnoreEnergy(){
  _ignore_energy = true;
}

void Anuran::noteOn( StkFloat instrument, StkFloat amplitude )
{
#if defined(_STK_DEBUG_)
  errorString_ << "Anuran::noteOn: instrument = " << instrument << ", amplitude = " << amplitude << '.';
  handleError( StkError::DEBUG_WARNING );
#endif

  StkFloat gain = amplitude;
  if ( amplitude > 1.0 ) {
    errorString_ << "Anuran::noteOn: amplitude parameter is greater than 1.0 ... setting to 1.0.";
    handleError( StkError::WARNING );
    gain = 1.0;
  }
  else if ( amplitude < 0.0 ) {
    errorString_ << "Anuran::noteOn: amplitude parameter is less than 0.0 ... doing nothing.";
    handleError( StkError::WARNING );
    return;
  }

  int noteNumber = ( (int) instrument ) % 11;

  // If we already have a wave of this note number loaded, just reset
  // it.  Otherwise, look first for an unused wave or preempt the
  // oldest if already at maximum polyphony.
  int iWave;
  for ( iWave=0; iWave<VOICE_POLYPHONY; iWave++ ) {
    if ( soundNumber_[iWave] == noteNumber ) {
      if ( waves_[iWave]->isFinished() ) {
        soundOrder_[iWave] = nSounding_;
        nSounding_++;
      }
      waves_[iWave]->reset();
      filters_[iWave].setPole( 0.999 - (gain * 0.6) );
      filters_[iWave].setGain( gain );
      break;
    }
  }

  if ( iWave == VOICE_POLYPHONY ) { // This note number is not currently loaded.
    if ( nSounding_ < VOICE_POLYPHONY ) {
      for ( iWave=0; iWave<VOICE_POLYPHONY; iWave++ )
        if ( soundOrder_[iWave] < 0 ) break;
      nSounding_ += 1;
    }
    else {
      for ( iWave=0; iWave<VOICE_POLYPHONY; iWave++ )
        if ( soundOrder_[iWave] == 0 ) break;
      // Re-order the list.
      for ( int j=0; j<VOICE_POLYPHONY; j++ ) {
        if ( soundOrder_[j] > soundOrder_[iWave] )
          soundOrder_[j] -= 1;
      }
    }
    soundOrder_[iWave] = nSounding_ - 1;
    soundNumber_[iWave] = noteNumber;

    // Concatenate the rawwave path to the rawwave file
//    waves_[iWave]->openFile( (std::string("rawwaves/") + voiceNames[ noteNumber ]).c_str(), false );
    
    if ( Stk::sampleRate() != 44100.0 )
      waves_[iWave]->setRate( 44100.0 / Stk::sampleRate() );
    filters_[iWave].setPole( 0.999 - (gain * 0.6) );
    filters_[iWave].setGain( gain );
  }

#if defined(_STK_DEBUG_)
  errorString_ << "Anuran::noteOn: number sounding = " << nSounding_ << '\n';
  for (int i=0; i<nSounding_; i++) errorString_ << soundNumber_[i] << "  ";
  errorString_ << '\n';
  handleError( StkError::DEBUG_WARNING );
#endif
}

void Anuran::noteOff(StkFloat amplitude)
{
  // Set all sounding wave filter gains low.
  int i = 0;
  while ( i < nSounding_ ) filters_[i++].setGain( amplitude * 0.01 );
}

/* 
  here is where we calculate the SPL at the receiver. 
  it is assumed that the SPL that comes in from the message is measured at 1 meter..
  so the equation used here:
  D (difference) = 20 log (d1/d2), where d1 =1, d2 = distance of this agent from the source  
  Thanks to Dimitri Ranos for helping me figure this one out.
*/
float Anuran::calculateAttenuation(float spl, float distance)
{
  return spl + ( 20 * log10(1/distance) );
}

float Anuran::calculateDistance(float x1, float y1, float x2, float y2)
{
  return sqrt( pow((x2 - x1), 2) + pow((y2 - y1), 2) );
}

bool Anuran::sortNeighbors(float i, float j) { return (i > j); }

/*
  This function adds a neighbor's SPL to a list of all neighbors and sorts it in decending order.
  TODO: have neighbors expire after some time so that selective attention can shift to another signaller
*/
bool Anuran::attendToNeighbor(float spl)
{
  bool attending = false;
  // are we already attending to this guy?
  for(unsigned int i=0; i< _neighbors.size(); i++ ){
    if( _neighbors[i] == spl){
      attending = true;
    } 
  }
  // If the neighbor is not found in the list, add it if we have room.
  if( !attending ) {
    // is there room to just push it into the vector?
    if( _neighbors.size() < _max_neighbors ) {
      _neighbors.push_back( spl );
      attending = true;
    }
    else{
      // if we have reached our limits, replace the quietest, 
      // but only if it is louder than the queitest (!) ...
      if(spl > _neighbors[_max_neighbors - 1]) {
        _neighbors[_max_neighbors - 1] = spl;
        attending = true;  // affirm that you are attending to this one
      }
      else {
        attending = false; // deny that you are attending to this one
      }
    }    
    // sort them in descending order
    sort(_neighbors.begin(), _neighbors.end(), Anuran::sortNeighbors);
    
    //cout << _id << " Attending to: ";
    //for(unsigned int i=0; i< _neighbors.size(); i++ ){
    //  cout << _neighbors[i] << " ";
    //}
    //cout << endl;
    
  }
  return attending;
}

void Anuran::forgetNeighbors( )
{
  _neighbors.clear();
}


void Anuran::expendEnergy( )
{
  Rand r;
  _energy -= r.randfloat(0.1, 0.3) * _cost_of_signaling;
  
  if( _energy <= 0) {
    has_energy_to_signal = false;
    forgetNeighbors();
    _number_of_calls = 0;
    _energy = 0;
  }
}

void Anuran::regainEnergy( )
{
  Rand r;
  _energy += r.randfloat(0.15, 0.25) * _cost_of_signaling;
  if( _energy >= 95) has_energy_to_signal = true;
}
/*!
  When a message is received from a neighbor, the call period is lengthened or shortened by the timing algorithm.
  The call timing information is specified in the genome in seconds so that its easier to read.
  !!!
  NB: These calculations are done in *SECONDS*, 
      thus there needs to be certain conversions to frames for these variables:
      elapsed_time, 
*/
void Anuran::receive( Message *message )
{
  Rand r;
  double senders_x, senders_y, stimulus_distance, stimulus_duration, trigger_time;
  float src_spl, spl;

  // Ignore messages sent by yourself
  // TODO: you shouldn't even hear yourself anyhoo
  if( _id_str != message->getValue("sender_id") )
  {
    
    // Get values from the message sent
    senders_x         = atof( message->getValue("location_x").c_str() );
    senders_y         = atof( message->getValue("location_y").c_str() );
    src_spl           = atof( message->getValue("spl").c_str() );
    stimulus_duration = atof( message->getValue("call_duration").c_str() );
    // Calculate the distance and SPL with attenuation of the stimulus.
    stimulus_distance = Anuran::calculateDistance( senders_x, senders_y, location_x, location_y );    
    spl               = Anuran::calculateAttenuation( src_spl, stimulus_distance );
    
    if( attendToNeighbor(spl) && has_energy_to_signal)
    {    
      // Make for damn sure these are cleared out
      elapsed_time_since_call = 0;
      temp = 0;
      // Calculate the trigger time = time that this individual would have sent the trigger to the actuator
      trigger_time      = _period - _effector_delay; 
      elapsed_time      = elapsed_time_in_frames / Stk::sampleRate();
    
      // Has the time elapsed just passed the next triggering event (its a very small window)?  
      // If so, set the elapsed_time_since_call negative! - very important, get it?
      // - also if it HAS passed, this period is added onto the next period - NOT the current one
      if ( elapsed_time > trigger_time) { 
        elapsed_time_since_call = trigger_time - elapsed_time;
      }
      else {
        elapsed_time_since_call = elapsed_time;
      }   
    
      // Add a little noise to the period adjustment
      e = r.randfloat(-0.06, 0.06) * _period;

  #if defined(_STK_DEBUG_)    
      cout << "RECEIVE: " << _id_str << "\n";
      cout << "_period:" << _period <<"\n";
      cout << "_prc_slope:" << _prc_slope <<"\n";
      cout << "stimulus_distance:"<< stimulus_distance <<"\n"; 
  #endif

  #if defined(_STK_DEBUG_)

      cout << "src_spl:"<< src_spl <<"\n";    
      cout << "spl at this agent:"<< spl <<"\n";    
      cout << "_period:"<< _period <<"\n";    
      cout << "stimulus_duration:"<< stimulus_duration <<"\n";
      cout << "_call_duration:"<< _call_duration <<"\n";    
  #endif

      // Calculate the new call period!
      //( 0.4 * ( (d+(l/v)) - (r-t) ) ) + (T + E) + (s.y-x) => temp;
      //cout << "elapsed_time_since_call:"<< elapsed_time_since_call <<"\n";    
      //cout <<"((elapsed_time_since_call + (stimulus_distance/SPEED_OF_SOUND))" << (stimulus_distance/SPEED_OF_SOUND) + elapsed_time_since_call << endl;
      //cout << "(_decending_length - _effector_delay)" << (_decending_length - _effector_delay) << endl;
      double adjustment = ( _prc_slope * ((elapsed_time_since_call + (stimulus_distance/344)) - (_decending_length - _effector_delay)) );
      //cout << "adjustment:"<< adjustment <<"\n";    
      temp = adjustment + (_period + e) + (stimulus_duration - _call_duration);


      // If the "elapsed_time_since_call" is positive, set the current "period_" to "temp", thus lengthening it,
      // otherwise, put the value in the overflow to be used for the next period
      if( elapsed_time_since_call > 0.0 ){
        _period = temp;
        _period_in_frames = (unsigned long) (temp * Stk::sampleRate());
  #if defined(_STK_DEBUG_)
      cout << "new _period:"<< _period <<"\n";    
  #endif
      }
      else {
  #if defined(_STK_DEBUG_)
        cout << "---------- PERIOD NEXT SET ---------";
  #endif
        _period_overflow = temp;
      }
    }
    else{
      // cout << "xxxxxxxxxx NOT attending to my neighbor at spl: " << spl << "\n\n";
    }
  } // end if - dont pay attention to yourself
}

void Anuran::triggerSignal()
{
  #if defined(_STK_DEBUG_)
    cout << "\n\n(( CHIRP " << _id << " " << _period << " ))\n";
  #endif
  // Trigger the audio sample
  noteOn( 0, _gain );
  // Emit the signal
  _dispatcher->channels[_signal_name]->emit(_message);
  // expend a little energy for signaling  
  expendEnergy();
  // reassess neighbors
  _number_of_calls += _number_of_calls;
  if (_number_of_calls >= 4) {
    forgetNeighbors();
  }
}

/*!
  These calculations *should* be done in frames, as that is the currency of realtime audio.
  This function also determines whether or not to chirp, based on the period
*/
void Anuran::incrementFrame( )
{

  // If the elapsed time (Telapsed) is greater than or equal to the call period (Tprime)
  // send a trigger to the chirp  
  if( elapsed_time_in_frames >= _period_in_frames ) 
  {  
   
    if( has_energy_to_signal || _ignore_energy )
    {
      triggerSignal();
    
      // If there is something "stored" for the next period, 
      // it becomes the modified period (?..double check this)
      if( _period_overflow > 0 ) {
        _period_in_frames = (unsigned long) (_period_overflow * Stk::sampleRate());
        _period = _period_overflow;
      }
      else {
        _period_in_frames = (unsigned long) (_PERIOD * Stk::sampleRate());
        _period = _PERIOD;  // reset the period to its free-running value
      }
      _period_overflow        = 0; // reset the next round modifications
    }
    else
    {
      regainEnergy();
    }

  // reset the elapsed time
    elapsed_time_in_frames = 0;
    elapsed_time           = 0;
  
  }
  
  elapsed_time_in_frames++;
}

/* 
  This member function is called by tick() method inherited from Instrmnt.
   - tick(), in turn, is called by the callback stream (<-- correct) in main();
*/
StkFloat Anuran::computeSample()
{
  lastOutput_ = 0.0;
  incrementFrame();
  
  if ( nSounding_ == 0 ) return lastOutput_;

  for ( int i=0; i<VOICE_POLYPHONY; i++ ) {
    if ( soundOrder_[i] >= 0 ) {
      if ( waves_[i]->isFinished() ) {
        // Re-order the list.
        for ( int j=0; j<VOICE_POLYPHONY; j++ ) {
          if ( soundOrder_[j] > soundOrder_[i] )
            soundOrder_[j] -= 1;
        }
        soundOrder_[i] = -1;
        nSounding_--;
      }
      else
        lastOutput_ += filters_[i].tick( waves_[i]->tick() );
    }
  }

  return lastOutput_;
}
