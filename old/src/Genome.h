/***************************************************
\class Genome
***************************************************/

#ifndef GENOME_H
#define GENOME_H

#include <cmath>
#include <cstdio>
#include <stdlib.h>
#include <map>
#include <string>
#include "Rand.h"
#include <iostream>
#include <sstream>

using namespace std;

class Genome {
public:
  Genome(int i);
  Genome(string, string, string, string, string, string, string, string);
  ~Genome();
  
  /** */
  string getGene(string gene);

  /** */  
  void setGene(string gene, string value);  

  /** */  
  void save(void);

//private:
  // SHARED FIELDS
  map<string,string> genes;
  int id;

};
#endif
