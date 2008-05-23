#include "Breeder.h"
#ifndef _STK_DEBUG_
  #define _STK_DEBUG_
#endif

Breeder::Breeder(Dispatcher *d, Conductor *c)
{ 
  /* The Breeder needs to know about a Dispatcher so that it can be sent on to the instances of the Anurans and Insects
  */
  dispatcher = d; // register a reference to the main Dispatcher (sends messages)
  conductor  = c; // register a reference to the main Conductors (sets gains, directs audio output to speaker channels)
}

Breeder::~Breeder() { }

void Breeder::create_frogs(int size, string species) 
{
  int anuran_count = anurans.size();

  for( int i=anuran_count; i < (size + anuran_count); i++ ) 
  {
    // SPRING PEEPER
    if ( !strcmp("spring_peeper", species.c_str()) ) {
      anurans[i] = new SpringPeeper(dispatcher);
    }
    // GRAY TREE FROG
    else if ( !strcmp("graytreefrog", species.c_str()) ) {
      anurans[i] = new GrayTreeFrog(dispatcher);  
    }
    // BIRD-VOICED TREE FROG
    else if ( !strcmp("bird_voiced_tree_frog", species.c_str()) ) {
      anurans[i] = new BirdVoicedTreeFrog(dispatcher);  
    }
    // GREEN FROG
    else if ( !strcmp("greenfrog", species.c_str()) ) {
      anurans[i] = new GreenFrog(dispatcher);  
    }
    // GREEN TREE FROG
    else if ( !strcmp("green_tree_frog", species.c_str()) ) {
      anurans[i] = new GreenTreeFrog(dispatcher);
    }
    // AMERICAN TOAD
    else if ( !strcmp("americantoad", species.c_str()) ) {
      anurans[i] = new AmericanToad(dispatcher);
    }
    // BULL FROG 
    else if ( !strcmp("bullfrog", species.c_str()) ) {
      anurans[i] = new BullFrog(dispatcher);
    }
    // BRIMLEYS CHORUS FROG
    else if ( !strcmp("brimleys_chorus_frog", species.c_str()) ) {
      anurans[i] = new BrimleysChorusFrog(dispatcher);
    }
    // EASTERN NARROWMOUTH TOAD
    else if ( !strcmp("eastern_narrowmouth_toad", species.c_str()) ) {
      anurans[i] = new EasternNarrowmouthToad(dispatcher);
    }
    // PICKEREL FROG
    else if ( !strcmp("pickerel_frog", species.c_str()) ) {
      anurans[i] = new PickerelFrog(dispatcher);
    }
    // BULL FROG 
    else if ( !strcmp("allards_ground_cricket", species.c_str()) ) {
      anurans[i] = new AllardsGroundCricket(dispatcher);
    }
    // SNOWY TREE CRICKET
    else if ( !strcmp("snowy_tree_cricket", species.c_str()) ) {
      anurans[i] = new SnowyTreeCricket(dispatcher);
    }
    // COMMON MEADOW KATYDID
    else if ( !strcmp("common_meadow_katydid", species.c_str()) ) {
      anurans[i] = new CommonMeadowKatydid(dispatcher);
    }
    // COMMON MEADOW KATYDID
    else if ( !strcmp("spring_ground_cricket", species.c_str()) ) {
      anurans[i] = new SpringGroundCricket(dispatcher);
    }
    // GREATER ANGLEWING
    else if ( !strcmp("greater_anglewing", species.c_str()) ) {
      anurans[i] = new GreaterAnglewing(dispatcher);
    }
    else{
      cout << "You have requested a species for which there is no template yet (" << species << "). \nPlease contact Unnature's creation division.\n";
    }
    
    conductor->addInstrument(anurans[i]);
  }
  
}

void Breeder::create_insects(int _size, string species) 
{
  int insect_count = insects.size();
  for( int i=insect_count; i < (_size + insect_count); i++ ) 
  {
    // ALLARDS GROUND CRICKET
    if ( !strcmp("allards_ground_cricket", species.c_str()) ) {
    
     // insects[i] = new AllardsGroundCricket(dispatcher);
    }
    // GREATER ANGLEWING
    if ( !strcmp("greater_anglewing", species.c_str()) ) {
    
      //insects[i] = new GreaterAnglewing(dispatcher);
    }
    else{
      cout << "You have requested an insect species for which there is no template yet (" << species << "). Please contact Unnature's creation division.\n";
    }
    conductor->addInstrument(insects[i]);
  }

}
