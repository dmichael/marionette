/*****************************************************************************

File:     RubyAudio.cpp
Date:     06Apr06

Copyright (C) 2007 by David M Michael for Unnature, LLC . All Rights Reserved.

RtAudio wrapper class.
Contains and manages instantiation of the audio output streams
....

*****************************************************************************/
#include <vector>
#include <iostream>
#include <map>
#include <list>
#include <stdexcept>

#include "RtAudio.h"
#include "Bus.h"
#include "Effects.h"


class Marionette
//class RubyAudio_t
{
public:
  
  RtAudio *dac;
  RtAudio::StreamParameters parameters;

  unsigned int bufferSize;
	
	double samplerate;
	double sampleduration;
	double blockduration;	// in seconds
	double now;
	unsigned long clock;	  ///! samples (in the buffer, 0..blocksize)
	unsigned long bufclock;	///! increments once per block
	
	Bus *out;
	Bus *in;

	Marionette();	
	Marionette(int, float);
	virtual ~Marionette();
	
  void execute();
  
  void addUnit(Unit*);
	void removeUnit(Unit*);
  
  void Run();
  void Stop();
  void info( void );
};