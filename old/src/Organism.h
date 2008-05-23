#ifndef ORGANISM_H
#define ORGANISM_H

#include "Organism.h"
#include "Instrmnt.h"
#include "Message.h"
#include "sigslot.h"
  
class Organism : public Instrmnt, public sigslot::has_slots<> {
public:
  Organism();

  ~Organism();

  void set_location_x(float);  
  
  void set_location_y(float);

  float location_x;
  float location_y;
  
  Message *_message;
};
#endif
