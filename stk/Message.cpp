#include "Message.h"

Message :: Message(){}

Message :: ~Message(){}

void Message::setValue(string key, float value)
{
  data[key] = value;
}

float Message::getValue(string key)
{
  if(data.find(key) == data.end()) {
    return 0.0;
  }
  else {
    return data[key];
  }

}
