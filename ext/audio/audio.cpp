#include <ruby.h>
#include "shared.h"
#include "Marionette.h"

/* Audio provides the core object for the management of the realtime audio subsystem.
 * It is meant to be a singleton with in the processes context because it uses a single common callback for calculating buffers.
 *
 * Copyright (C) 2008 by David M Michael <david@unnature.org>.
 * You may distribute under the terms of the MIT license
 *
 * = Introduction
 *
 */
 
// This is the main object that keeps record of all realtime audio related object instances.
Marionette *_marionette = NULL;

// This function is used by all Ruby Unit classes - see shared.h for reference.
void rb_ugen_dealloc(Unit *ugen){ delete ugen; }



/*
 * Convenience method for adding Unit graphs to the main out bus, Audio::Out
 * The following two statements are equivalent.
 *   
 *   sine = Sine.new
 *
 *   Audio::Out.add(sine) -> true
 *   sine.play            -> You are already playing this Unit.
 * 
*/
static VALUE Unit_play(VALUE self) 
{
	// retrieve the Unit...
	Unit *ugen;
  Data_Get_Struct(self, Unit, ugen);
  
  // make sure the type being passing in is the one that we want
	if(TYPE(self) != T_DATA){
    printf("This is not T_DATA\n");
	}
	if(RDATA(self)->dfree != (RUBY_DATA_FUNC)rb_ugen_dealloc){
    printf("This does not have the right deallocation function\n");
	}
	if(TYPE(self) != T_DATA || RDATA(self)->dfree != (RUBY_DATA_FUNC)rb_ugen_dealloc){
    rb_raise(rb_eTypeError, "Wrong argument type");
	}

  // ... and add it to "out"
  _marionette->addUnit(ugen);
  return Qtrue;
}

/**************************
Unit_stop
**************************/
static VALUE Unit_stop(VALUE self) 
{
  // make sure the type being passing in is the one that we want
	if(TYPE(self) != T_DATA || RDATA(self)->dfree != (RUBY_DATA_FUNC)rb_ugen_dealloc){
    rb_raise(rb_eTypeError, "Wrong argument type");
	}
	// retrieve the Unit...
	Unit *ugen;
  Data_Get_Struct(self, Unit, ugen);
  _marionette->removeUnit(ugen);
  return Qtrue;
}

/**************************
Unit_set_pan
**************************/
static VALUE Unit_set_pan(VALUE self, VALUE pan) 
{
  if(TYPE(pan) == T_FLOAT || TYPE(pan) == T_FIXNUM){
  	// retrieve the Unit...
  	Unit *ugen;
    Data_Get_Struct(self, Unit, ugen);
    float pan_ = (float) NUM2DBL(pan);  
    ugen->setPan(pan_);
    rb_iv_set(self, "@pan", pan);
  }
  return Qtrue;
}

/**************************
Unit_get_pan
**************************/
static VALUE Unit_get_pan(VALUE self) 
{
  // retrieve the Unit...
  return rb_iv_get(self, "@pan");
}

/**************************
Unit_add_observer
**************************/
static VALUE Unit_add_observer(VALUE self, VALUE observer) 
{
  // make sure the type being passing in is the one that we want
	if(TYPE(observer) != T_DATA || RDATA(observer)->dfree != (RUBY_DATA_FUNC)rb_ugen_dealloc){
    rb_raise(rb_eTypeError, "Wrong argument type");
	}
	// retrieve the Unit...
	Unit *ugen;
  Data_Get_Struct(self, Unit, ugen);
  Unit *ugen2;
  Data_Get_Struct(observer, Unit, ugen2);
  ugen->addObserver( ugen2 );
  return Qtrue;
}

/**************************
Unit_remove_observer
**************************/
static VALUE Unit_remove_observer(VALUE self, VALUE observer) 
{
  // make sure the type being passing in is the one that we want
	if(TYPE(observer) != T_DATA || RDATA(observer)->dfree != (RUBY_DATA_FUNC)rb_ugen_dealloc){
    rb_raise(rb_eTypeError, "Wrong argument type");
	}
	// retrieve the Unit...
	Unit *ugen;
  Data_Get_Struct(observer, Unit, ugen);
  ugen->deleteObserver( ugen );
  return Qtrue;
}


// AUDIO INITIALIZED
static VALUE Audio_initialize(int argc, VALUE *argv, VALUE self)
{
  printf("Initializing audio subsystem...");
  VALUE options;
  /* Zero mandatory, one optional argument */
  /* Here, this optional arg is meant to be a Hash */
  rb_scan_args(argc, argv, "01", &options);
  int device_id = 0;
  float sr = 44100.0; // sample rate
  if(options != Qnil){
    // lets make sure its a hash
    Check_Type(options, T_HASH);
    VALUE device = rb_hash_aref(options, ID2SYM(rb_intern("device")));
    VALUE samplerate = rb_hash_aref(options, ID2SYM(rb_intern("samplerate")));
    if(samplerate != Qnil) sr = (float) NUM2DBL(samplerate);
    if(device != Qnil) device_id = FIX2INT(device);
  }
  else{
    device_id = -1;
  }

	if (_marionette){
    printf("Audio is already initialized.\n");
	}
	else{
	  _marionette = (device_id == -1)? new Marionette() : new Marionette(device_id, sr);  
	}
  
  printf("done.\n");
	return Qtrue;
}

// AUDIO RELEASE
static VALUE Audio_release(VALUE self) 
{
	if(_marionette){
	  delete _marionette;    
    _marionette = NULL;
	}
	else{
	  printf("Audio is not initialized.");
	}
	return Qtrue;
}

// AUDIO RUN
static VALUE Audio_run(VALUE self) 
{
	if(_marionette) {
	  _marionette->Run();
	}
	else{
    printf("Audio is not initialized.");
	}
	return Qtrue;
}

// AUDIO STOP
static VALUE Audio_stop(VALUE self) 
{
	if(_marionette){
	  _marionette->Stop();
	}
  else{
	  printf("Audio is not initialized.");
  }
	return Qtrue;
}

// AUDIO INFO
static VALUE Audio_info(VALUE self) 
{
  if(_marionette) {
    _marionette->info();    
  } 
  else {
    printf("Audio is not intialized.\n");
  }
	return Qnil;
}

// AUDIO OUT ADD UGEN
static VALUE Audio_Out_add(VALUE self, VALUE ugen) 
{
	// make sure the type being passing in is the one that we want
	if(TYPE(ugen) != T_DATA || RDATA(ugen)->dfree != (RUBY_DATA_FUNC)rb_ugen_dealloc){
    rb_raise(rb_eTypeError, "Wrong argument type");
	}
	// retrieve the Unit...
	Unit *ugen_ptr;
  Data_Get_Struct(ugen, Unit, ugen_ptr);
  
  // ... and add it to "out"
  // Make sure that Marionette is instantiated
  if (!_marionette){
    printf("Audio is not initialized.");
    return Qfalse;
  } 
  else{
    _marionette->addUnit(ugen_ptr);
  	return Qtrue;
  } 
  // If the default Unit channel count (2) is not that of the outputs, resize it to be so
  // int output_channels = _marionette->out->channels();
  // if(ugen->channels() != output_channels) ugen.setChannels(output_channels);
}

// AUDIO OUT REMOVE UGEN
static VALUE Audio_Out_remove(VALUE self, VALUE ugen) 
{
	// make sure the type being passed in is the one that we want
	if(TYPE(ugen) != T_DATA || RDATA(ugen)->dfree != (RUBY_DATA_FUNC)rb_ugen_dealloc){
    rb_raise(rb_eTypeError, "Wrong argument type");
	}

  if(!_marionette) {
    printf("Audio is not initialized.");
    return Qfalse;
  }
  else {
  	Unit *ugen_ptr;
    Data_Get_Struct(ugen, Unit, ugen_ptr);
    _marionette->removeUnit(ugen_ptr);    
    return Qtrue;
  }
}


/*********************
Init_audio
*********************/
VALUE AudioModule;
VALUE AudioOut;
VALUE UnitModule;

extern "C" void Init_audio() 
{
  // Audio subsystem
  AudioModule = rb_define_module("Audio");
  rb_define_module_function(AudioModule, "initialize", (VALUE(*)(...)) Audio_initialize, -1);
  rb_define_module_function(AudioModule, "release", (VALUE(*)(...)) Audio_release, 0);
  rb_define_module_function(AudioModule, "run", (VALUE(*)(...)) Audio_run, 0);
  rb_define_module_function(AudioModule, "stop", (VALUE(*)(...)) Audio_stop, 0);
  rb_define_module_function(AudioModule, "info", (VALUE(*)(...)) Audio_info, 0);  

  // Audio Outputs
  AudioOut    = rb_define_class_under(AudioModule, "Out", rb_cObject);
  rb_define_singleton_method(AudioOut, "add", (VALUE(*)(...)) Audio_Out_add, 1);
  rb_define_singleton_method(AudioOut, "remove", (VALUE(*)(...)) Audio_Out_remove, 1);

  // Unit
  UnitModule = rb_define_module("Unit");
  rb_define_method(UnitModule, "play", (VALUE(*)(...)) Unit_play, 0);
  rb_define_method(UnitModule, "stop", (VALUE(*)(...)) Unit_stop, 0);
  rb_define_method(UnitModule, "pan=", (VALUE(*)(...)) Unit_set_pan, 1);
  rb_define_method(UnitModule, "pan", (VALUE(*)(...)) Unit_get_pan, 0);
  rb_define_method(UnitModule, "add_observer", (VALUE(*)(...)) Unit_add_observer, 1);
  rb_define_method(UnitModule, "remove_observer", (VALUE(*)(...)) Unit_remove_observer, 1);
}