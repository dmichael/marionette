/***************************************************
\class Breeder

This Breeder class is a factory pattern to generate Organisms from Genomes.
At the time of authorship, it is only built to handle the contruction of Anurans.

***************************************************/

#ifndef BREEDER_H
#define BREEDER_H

#include <cmath>
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include "Rand.h"
#include "Genome.h"
// ANURAN INCLUDES
#include "Anuran.h"
#include "Conductor.h"
#include "SpringPeeper.h"
#include "GrayTreeFrog.h"
#include "BirdVoicedTreeFrog.h"
#include "GreenTreeFrog.h"
#include "GreenFrog.h"
#include "AmericanToad.h"
#include "BullFrog.h"
#include "BrimleysChorusFrog.h"
#include "EasternNarrowmouthToad.h"
#include "PickerelFrog.h"
// INSECT INCLUDES
#include "Cricket.h"
#include "AllardsGroundCricket.h"
#include "GreaterAnglewing.h"
#include "SnowyTreeCricket.h"
#include "CommonMeadowKatydid.h"
#include "SpringGroundCricket.h"
using namespace std;

// limit the population size to avoid potential population explosions (and subsequent resource exhaustion)
const int POPULATION_SIZE = 30;

class Breeder {
public:

  /** Default constructor. There should only be one Breeder initialized. */
  Breeder(Dispatcher*, Conductor*);
  /** Default deconstructor */
  ~Breeder();
  
  /** 
    Initializes the population of Anurans.
    This method should create Anurans from existing Genomes, 
    else it creates one from scratch
  */
  void create_frogs(int, string);
  
  void create_insects(int, string);
  
  /*
    These are fields that are references to the main program objects, 
    Dispatcher and Conductor (who really might need to be Singleton classes).
    In most cases they are here in order that they can give the reference to Bred objects
  */
  Dispatcher *dispatcher; 
  Conductor  *conductor;
  
  /** Map of genomes, keyed by genome ID*/
  map<int,Genome*> genomes;
  /** Map of anurans, keyed by anuran ID*/
  map<int,Anuran*>  anurans;
  map<int,Cricket*> insects;
  
};
#endif
