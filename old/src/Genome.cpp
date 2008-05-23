
#include "Genome.h"

#ifndef _STK_DEBUG_
  #define _STK_DEBUG_
#endif

/** Default constructor generates new Genome of randomized values. */
Genome::Genome(int i) 
{
  Rand r;             // random number generating utility
  ostringstream temp; // temp place to house the string version of the rand
  
  // id
  temp << i;
  genes["id"] = temp.str();
  
  // PERIOD
  temp << r.randfloat(1.8, 2.0);
  genes["period"] = temp.str();

  // DESCENDING LENGTH
  temp << r.randfloat(1.8, 2.0);
  genes["decending_length"] = temp.str();

  // EFFECTOR DELAY
  temp << r.randfloat(1.8, 2.0);
  genes["effector_delay"] = temp.str();

  // PRC SLOPE
  temp << r.randfloat(1.8, 2.0);
  genes["prc_slope"] = temp.str();
  
  genes["channel"] = "0";
  genes["sample"]  = "peeper.wav";
 
}

Genome::Genome(string id, string period, string decending_length, string effector_delay, string prc_slope, string channel, string sample, string selective_attention)
{
  genes["id"]               = id;
  genes["period"]           = period;
  genes["decending_length"] = decending_length;
  genes["effector_delay"]   = effector_delay;
  genes["prc_slope"]        = prc_slope;
  genes["channel"]          = channel;
  genes["sample"]           = sample;
  genes["selective_attention"]           = selective_attention;

  //cout << "genome defined " << genes["id"] << "\n";
}

Genome::~Genome() 
{
}

string Genome::getGene(string gene_)
{
  return genes[gene_];
}
  
void Genome::setGene(string gene, string value)
{
  genes[gene] = value;
}

void Genome::save()
{
}
