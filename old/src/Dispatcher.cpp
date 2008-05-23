// This should eventually be defined as a singleton object to prevent instantiation
#include "Dispatcher.h"

Dispatcher::Dispatcher() { }

Dispatcher::~Dispatcher() { }

void Dispatcher::defineChannel(std::string channelName) 
{
  
  //soundNumber_ = std::vector<int> (VOICE_POLYPHONY, -1);
  if(channels.find(channelName) == channels.end()) {
    printf("Defined a channnel called '%s'!\n", channelName.c_str());
 
    channels[channelName] = new sigslot::signal1<Message*>;
    //channels[channelName] = s;
  }
  else {
    // Already defined this channel, so lets not do anything, shall we
  }

}



/*
bool Dispatcher :: isDefined(char channelName) 
{
  if(Channels.find(channelName) == Channels.end()) {
    return false;
  }
  else {
    return true;
  }
}

int Dispatcher :: connect( char channelName, Observer &observer) 
{ 
  // Subscribe (connect) a member function of an observer (has_slots) to the signal
  Channels[channelName].connect(&observer, &Observer::receive);
  // return 0 for success (as Bash program convention)
  return 0;
  //failNil(this.signals[sig], "Signal.connect(): There is no signal named '" + sig + "'");
  //channels[channelName].addObserver(o);                 
}


int Dispatcher :: emit(string channelName, int id, Message message) 
{
  Channels[channelName].emit(id, message);
  return 0;
}
*/
