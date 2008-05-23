/***************************************************/
/*! \class Message
    \brief class defining audio signals in the sig/slot signal/dispatch or observer patterns
    
    by David M. Michael 7.4.2007 for Unnature
*/
/***************************************************/

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <map>

using namespace std;

class Message
{
public:
  //! Class constructor.
  Message();
  
  //! Class destructor.
  ~Message();
  
  void setValue(string, float);
  
  float getValue(string);
  
  map<string,float> data;
  
};

#endif
