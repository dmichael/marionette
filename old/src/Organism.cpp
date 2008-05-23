#include "Organism.h"  

Organism :: Organism() : location_x(0), location_y(0)
{
}

Organism :: ~Organism(){
}


void Organism :: set_location_x(float x) {
  location_x = x;
}

void Organism :: set_location_y(float y) {
  location_y = y;
}
