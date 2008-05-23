/*****************************************************************************

File:     cmain.cpp
Date:     06Apr06

Copyright (C) 2007/8 by David M Michael for Unnature, LLC . All Rights Reserved.

*****************************************************************************/

#include <iostream>
#include <stdexcept>
#include "Marionette.h"


Marionette *marionette = NULL;


/***********************
c_initialize_library
***********************/

extern "C" void c_initialize_library()
{
  std::cout << "Initializing audio...";
	if (marionette){
    throw std::runtime_error ("already initialized");
	}
  marionette = new Marionette();
}

/********************
c_release_library
********************/

extern "C" void c_release_library()
{
	if (!marionette){
    throw std::runtime_error ("not initialized");
	}
	delete marionette;
	marionette = NULL;
}


/****************
c_run_audio
****************/

extern "C" void c_run_audio()
{
	if (!marionette){
    throw std::runtime_error ("not initialized");
	}
	marionette->Run();
}

/****************
c_run_audio
****************/

extern "C" void c_stop_audio()
{
	if (!marionette)
		throw std::runtime_error ("not initialized");
	marionette->Stop();
}

extern "C" void c_init_sine()
{
}


extern "C" void c_audio_out_add(Ugen* ugen)
{
  // Make sure that Marionette is instantiated
  if (!marionette) throw std::runtime_error ("Marionette is not initialized!");
  // If the default Ugen channel count (2) is not that of the outputs, resize it to be so
  // int output_channels = marionette->out->channels();
  // if(ugen->channels() != output_channels) ugen.setChannels(output_channels);
  marionette->addUgen(ugen);
}

extern "C" void c_audio_out_remove(Ugen* ugen)
{
  if (!marionette) throw std::runtime_error ("Marionette is not initialized!");
  marionette->removeUgen(ugen);
}

extern "C" void c_audio_info()
{
  if (!marionette) throw std::runtime_error ("Marionette is not initialized!");
  marionette->info();
}

