#include "Message.h"

Message :: Message()
{
//
}

Message :: ~Message()
{
}

void Message::setValue(string key, string value)
{
  data[key] = value;
}

string Message::getValue(string key)
{
  
  if(data.find(key) == data.end()) {
    return "null";
  }
  else {
    return data[key];
  }

}
