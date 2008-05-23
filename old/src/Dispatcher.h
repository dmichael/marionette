#ifndef STK_DISPATCHER_H
#define STK_DISPATCHER_H

#include <string>
#include <vector>
#include <map>
#include "sigslot.h"
#include "Message.h"
// this saves us the headache of specifying std:: for everything.
using namespace std;
  
class Dispatcher
{
public:
  //! Class constructor.
  Dispatcher();
  //! Class destructor.
  ~Dispatcher();

  void defineChannel(string);
  
  map<string,sigslot::signal1<Message*>* > channels;


};
#endif
