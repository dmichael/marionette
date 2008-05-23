/*****************************************************************************

File:     rubymain.cpp
Date:     06Apr06

Copyright (C) 2007 by David M Michael for Unnature, LLC . All Rights Reserved.

*****************************************************************************/

#include <ruby.h>
#include "cmain.h"
#include "marionette.h"

#include "SineWaveUgen.h"
#include "SoundFileUgen.h"
#include "PannerUgen.h"
#include "Observable.h"





/**************************
rb_ugen_dealloc
**************************/
static void rb_ugen_dealloc(Ugen *ugen){ delete ugen; }


/*****************************************************************************

Sine

*****************************************************************************/

/**************************
rb_sine_alloc
**************************/
static VALUE rb_sine_alloc(VALUE self)
{
  SineWaveUgen *sine = new SineWaveUgen;
  VALUE obj = Data_Wrap_Struct(self, 0, rb_ugen_dealloc, sine);
	return obj;
}

/**************************
rb_sine_initialize
**************************/
static VALUE rb_sine_initialize(VALUE self, VALUE options) 
{
  VALUE freq = rb_hash_aref(options, ID2SYM(rb_intern("freq"))); 
  rb_iv_set(self, "@freq", freq);
  // get the Ugen instance
  SineWaveUgen *sine;
  Data_Get_Struct(self, SineWaveUgen, sine);
  // initialize the parameters
  sine->source->setFrequency(FIX2INT(freq));
	return self;
}

/**************************
rb_sine_set_frequency
**************************/
static VALUE rb_sine_set_frequency(VALUE self, VALUE frequency) 
{
  // get the Ugen instance
  SineWaveUgen *sine;
  Data_Get_Struct(self, SineWaveUgen, sine); 

  // set the frequency  
  if(TYPE(frequency) != T_DATA || RDATA(frequency)->dfree != (RUBY_DATA_FUNC)rb_ugen_dealloc){
    Ugen *mod;
    Data_Get_Struct(frequency, Ugen, mod);
    sine->addFreqMod(mod);
  }
  else{
    sine->source->setFrequency(FIX2INT(frequency));
  }
  
  // store what was set in the instance variable
  rb_iv_set(self, "@freq", frequency);
	return frequency;
}

/**************************
rb_sine_get_frequency
**************************/
static VALUE rb_sine_get_frequency(VALUE self) 
{  
  // This is done in Ruby because the STK SineWave.cpp class has no getter
  return rb_iv_get(self, "@freq");
}

/**************************
rb_name_sine
**************************/
static VALUE rb_sine_name(VALUE self) 
{
  SineWaveUgen *sine;
  Data_Get_Struct(self, SineWaveUgen, sine);
	return rb_str_new2(sine->name());
}



/*****************************************************************************
*****************************************************************************
  PANNER
*****************************************************************************
*****************************************************************************/

/**************************
rb_panner_alloc
**************************/
static VALUE rb_panner_alloc(VALUE self)
{
  PannerUgen *panner = new PannerUgen;
  VALUE obj = Data_Wrap_Struct(self, 0, rb_ugen_dealloc, panner);
	return obj;
}

/**************************
rb_sine_initialize
**************************/
static VALUE rb_panner_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE options;
  VALUE input;
  /* One mandatory, two optional argument */
  rb_scan_args(argc, argv, "02", &input, &options);
  
  // get the Ugen instance
  PannerUgen *panner;
  Data_Get_Struct(self, PannerUgen, panner);
  
  if(input != Qnil){
    // make sure this is a Ugen
    if(TYPE(input) == T_DATA || RDATA(input)->dfree == (RUBY_DATA_FUNC)rb_ugen_dealloc){
      // retrieve the Ugen...
    	Ugen *ugen_ptr;
      Data_Get_Struct(input, Ugen, ugen_ptr);
      printf("%s\n",ugen_ptr->name());
      panner->setInput(ugen_ptr);
    }
    else{
      // wrong type, throw exception
    }
    
  }
  
  if(options != Qnil){
    Check_Type(options, T_HASH);
  	VALUE position = rb_hash_aref(options, ID2SYM(rb_intern("position"))); 
    rb_iv_set(self, "@position", position); 
    // initialize the parameters
    if(TYPE(position) == T_FLOAT || TYPE(position) == T_FIXNUM){
      float position_f = (float) NUM2DBL(position);
      panner->setInputPosition(position_f);
    }
  }    


	return self;
}







/*****************************************************************************
*****************************************************************************

SoundFile

*****************************************************************************
*****************************************************************************/

/**************************
rb_sound_file_alloc
**************************/
static VALUE rb_sound_file_alloc(VALUE self)
{
  SoundFileUgen *ugen = new SoundFileUgen;
  VALUE obj = Data_Wrap_Struct(self, 0, rb_ugen_dealloc, ugen);
	return obj;
}

/**************************
rb_sound_file_initialize
**************************/
static VALUE rb_sound_file_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE options;
  /* One mandatory, one optional argument */
  rb_scan_args(argc, argv, "01", &options);
  
  SoundFileUgen *soundfileugen;
  Data_Get_Struct(self, SoundFileUgen, soundfileugen);
  
  if(options != Qnil){
    Check_Type(options, T_HASH);
    VALUE filename = rb_hash_aref(options, ID2SYM(rb_intern("file")));     
    
    soundfileugen->open(StringValuePtr(filename));
  }    
	return self;
}

/**************************
rb_sound_file_name
**************************/
static VALUE rb_sound_file_name(VALUE self) 
{
  SoundFileUgen *ugen;
  Data_Get_Struct(self, SoundFileUgen, ugen);
	return rb_str_new2(ugen->name());
}

/**************************
rb_sound_file_set_rate
**************************/
static VALUE rb_sound_file_set_rate(VALUE self, VALUE rate) 
{
  // get the Ugen instance
  SoundFileUgen *ugen;
  Data_Get_Struct(self, SoundFileUgen, ugen); 
  
  if(TYPE(rate) == T_FLOAT || TYPE(rate) == T_FIXNUM){
    float rate_f = (float) NUM2DBL(rate);
    // set the frequency
    ugen->setRate(rate_f);
    // store what was set in the instance variable
    rb_iv_set(self, "@rate", rate);
  }
	return rate;
}

/**************************
rb_sound_file_get_rate
**************************/
static VALUE rb_sound_file_get_rate(VALUE self) 
{  
  // This is done in Ruby because the STK SineWave.cpp class has no getter
  return rb_iv_get(self, "@rate");
}

/**************************
rb_sound_file_open
**************************/
static VALUE rb_sound_file_open(VALUE self, VALUE filename) 
{
  // We want a string here...
  Check_Type(filename, T_STRING);
  // get the Ugen instance
  SoundFileUgen *ugen;
  Data_Get_Struct(self, SoundFileUgen, ugen);
  
  // open the file
  ugen->open(StringValuePtr(filename));
	return filename;
}

/**************************
rb_sound_file_restart
**************************/
static VALUE rb_sound_file_restart(VALUE self) 
{
  SoundFileUgen *ugen;
  Data_Get_Struct(self, SoundFileUgen, ugen);
  ugen->reset();
}

/**************************
rb_sound_file_get_duration
**************************/
static VALUE rb_sound_file_get_duration(VALUE self) 
{
  SoundFileUgen *ugen;
  Data_Get_Struct(self, SoundFileUgen, ugen);
  
  StkFloat duration = ugen->durationInSeconds();
	return rb_float_new(duration);
}

/**************************
rb_sound_file_get_duration_samples
**************************/
static VALUE rb_sound_file_get_duration_samples(VALUE self) 
{
  SoundFileUgen *ugen;
  Data_Get_Struct(self, SoundFileUgen, ugen);
  
  unsigned long duration = ugen->durationInSamples();
	return LONG2NUM(duration);
}


/*****************************************************************************
*****************************************************************************

Ugen Module

*****************************************************************************
*****************************************************************************/

/**************************
rb_ugen_play
**************************/
static VALUE rb_ugen_play(VALUE self) 
{
  // make sure the type being passing in is the one that we want
	if(TYPE(self) != T_DATA || RDATA(self)->dfree != (RUBY_DATA_FUNC)rb_ugen_dealloc){
    rb_raise(rb_eTypeError, "Wrong argument type");
	}
	// retrieve the Ugen...
	Ugen *ugen;
  Data_Get_Struct(self, Ugen, ugen);
  // ... and add it to "out"
  c_audio_out_add(ugen);
  return Qtrue;
}

/**************************
rb_ugen_stop
**************************/
static VALUE rb_ugen_stop(VALUE self) 
{
  // make sure the type being passing in is the one that we want
	if(TYPE(self) != T_DATA || RDATA(self)->dfree != (RUBY_DATA_FUNC)rb_ugen_dealloc){
    rb_raise(rb_eTypeError, "Wrong argument type");
	}
	// retrieve the Ugen...
	Ugen *ugen;
  Data_Get_Struct(self, Ugen, ugen);
  c_audio_out_remove(ugen);
  return Qtrue;
}

/**************************
rb_ugen_set_pan
**************************/
static VALUE rb_ugen_set_pan(VALUE self, VALUE pan) 
{
  if(TYPE(pan) == T_FLOAT || TYPE(pan) == T_FIXNUM){
  	// retrieve the Ugen...
  	Ugen *ugen;
    Data_Get_Struct(self, Ugen, ugen);
    float pan_ = (float) NUM2DBL(pan);  
    ugen->setPan(pan_);
    rb_iv_set(self, "@pan", pan);
  }
  return Qtrue;
}

/**************************
rb_ugen_get_pan
**************************/
static VALUE rb_ugen_get_pan(VALUE self) 
{
  // retrieve the Ugen...
  return rb_iv_get(self, "@pan");
}

/**************************
rb_ugen_add_observer
**************************/
static VALUE rb_ugen_add_observer(VALUE self, VALUE observer) 
{
  // make sure the type being passing in is the one that we want
	if(TYPE(observer) != T_DATA || RDATA(observer)->dfree != (RUBY_DATA_FUNC)rb_ugen_dealloc){
    rb_raise(rb_eTypeError, "Wrong argument type");
	}
	// retrieve the Ugen...
	Ugen *ugen;
  Data_Get_Struct(self, Ugen, ugen);
  Ugen *ugen2;
  Data_Get_Struct(observer, Ugen, ugen2);
  ugen->addObserver( ugen2 );
  return Qtrue;
}

/**************************
rb_ugen_remove_observer
**************************/
static VALUE rb_ugen_remove_observer(VALUE self, VALUE observer) 
{
  // make sure the type being passing in is the one that we want
	if(TYPE(observer) != T_DATA || RDATA(observer)->dfree != (RUBY_DATA_FUNC)rb_ugen_dealloc){
    rb_raise(rb_eTypeError, "Wrong argument type");
	}
	// retrieve the Ugen...
	Ugen *ugen;
  Data_Get_Struct(observer, Ugen, ugen);
  ugen->deleteObserver( ugen );
  return Qtrue;
}



/*****************************************************************************

Audio subsystem functions

*****************************************************************************/
  
/**************************
rb_audio_initialize
**************************/
static VALUE rb_audio_initialize(VALUE self) 
{
  c_initialize_library();
	return Qtrue;
}

/**************************
rb_audio_release
**************************/
static VALUE rb_audio_release(VALUE self) 
{
  c_release_library();
	return Qtrue;
}

/**************************
rb_audio_run
**************************/
static VALUE rb_audio_run(VALUE self) 
{
  c_run_audio();
	return Qtrue;
}

/**************************
rb_audio_stop
**************************/
static VALUE rb_audio_stop(VALUE self) 
{
  c_stop_audio();
	return Qtrue;
}

/**************************
rb_audio_info
**************************/
static VALUE rb_audio_info(VALUE self) 
{
  c_audio_info();
	return Qnil;
}




/**************************
rb_audio_out_add
**************************/
static VALUE rb_audio_out_add(VALUE self, VALUE ugen) 
{
	// make sure the type being passing in is the one that we want
	if(TYPE(ugen) != T_DATA || RDATA(ugen)->dfree != (RUBY_DATA_FUNC)rb_ugen_dealloc){
    rb_raise(rb_eTypeError, "Wrong argument type");
	}
	// retrieve the Ugen...
	Ugen *ugen_ptr;
  Data_Get_Struct(ugen, Ugen, ugen_ptr);
  // ... and add it to "out"
  c_audio_out_add(ugen_ptr);
	return Qnil; // this should return true or something
}

/**************************
rb_audio_out_remove
**************************/
static VALUE rb_audio_out_remove(VALUE self, VALUE ugen) 
{
	// make sure the type being passing in is the one that we want
	if(TYPE(ugen) != T_DATA || RDATA(ugen)->dfree != (RUBY_DATA_FUNC)rb_ugen_dealloc){
    rb_raise(rb_eTypeError, "Wrong argument type");
	}
	// retrieve the Ugen...
	Ugen *ugen_ptr;
  Data_Get_Struct(ugen, Ugen, ugen_ptr);
  // ... and add it to "out"
  c_audio_out_remove(ugen_ptr);
	return Qnil; // this should return true or something
}


/*********************
Init_audio
*********************/



extern "C" void Init_rubymarionette() 
{
  // Audio subsystem
  AudioModule = rb_define_module("Audio");
  rb_define_module_function(AudioModule, "initialize", (VALUE(*)(...)) rb_audio_initialize, 0);
  rb_define_module_function(AudioModule, "release", (VALUE(*)(...)) rb_audio_release, 0);
  rb_define_module_function(AudioModule, "run", (VALUE(*)(...)) rb_audio_run, 0);
  rb_define_module_function(AudioModule, "stop", (VALUE(*)(...)) rb_audio_stop, 0);
  rb_define_module_function(AudioModule, "info", (VALUE(*)(...)) rb_audio_info, 0);  

  // Audio Outputs
  AudioOut    = rb_define_class_under(AudioModule, "Out", rb_cObject);
  rb_define_singleton_method(AudioOut, "add", (VALUE(*)(...)) rb_audio_out_add, 1);
  rb_define_singleton_method(AudioOut, "remove", (VALUE(*)(...)) rb_audio_out_remove, 1);  
  
  // Ugen
  UgenModule = rb_define_module("Ugen");
  rb_define_method(UgenModule, "play", (VALUE(*)(...)) rb_ugen_play, 0);
  rb_define_method(UgenModule, "stop", (VALUE(*)(...)) rb_ugen_stop, 0);
  rb_define_method(UgenModule, "pan=", (VALUE(*)(...)) rb_ugen_set_pan, 1);
  rb_define_method(UgenModule, "pan", (VALUE(*)(...)) rb_ugen_get_pan, 0);
  rb_define_method(UgenModule, "add_observer", (VALUE(*)(...)) rb_ugen_add_observer, 1);
  rb_define_method(UgenModule, "remove_observer", (VALUE(*)(...)) rb_ugen_remove_observer, 1);
  
  // Sine
  SineWaveClass = rb_define_class("Sine", rb_cObject);
  rb_define_alloc_func(SineWaveClass, rb_sine_alloc);
  rb_define_method(SineWaveClass, "initialize", (VALUE(*)(...)) rb_sine_initialize, 1);
  rb_define_method(SineWaveClass, "name", (VALUE(*)(...)) rb_sine_name, 0);
  rb_define_method(SineWaveClass, "freq=", (VALUE(*)(...)) rb_sine_set_frequency, 1);
  rb_define_method(SineWaveClass, "freq", (VALUE(*)(...)) rb_sine_get_frequency, 0);  
  rb_include_module(SineWaveClass, UgenModule);
  
  // Panner
  PannerClass = rb_define_class("Panner", rb_cObject);
  rb_define_alloc_func(PannerClass, rb_panner_alloc);
  rb_define_method(PannerClass, "initialize", (VALUE(*)(...)) rb_panner_initialize, -1);
  rb_include_module(PannerClass, UgenModule);
  
  // SoundFile
  SoundFileClass = rb_define_class("SoundFile", rb_cObject);
  rb_define_alloc_func(SoundFileClass, rb_sound_file_alloc);
  rb_define_method(SoundFileClass, "initialize", (VALUE(*)(...)) rb_sound_file_initialize, -1);
  rb_define_method(SoundFileClass, "name", (VALUE(*)(...)) rb_sound_file_name, 0);
  rb_define_method(SoundFileClass, "rate=", (VALUE(*)(...)) rb_sound_file_set_rate, 1);
  rb_define_method(SoundFileClass, "rate", (VALUE(*)(...)) rb_sound_file_get_rate, 0);
  rb_define_method(SoundFileClass, "open", (VALUE(*)(...)) rb_sound_file_open, 1);
  rb_define_method(SoundFileClass, "restart", (VALUE(*)(...)) rb_sound_file_restart, 0);
  rb_define_method(SoundFileClass, "duration", (VALUE(*)(...)) rb_sound_file_get_duration, 0);
  rb_define_method(SoundFileClass, "duration_in_samples", (VALUE(*)(...)) rb_sound_file_get_duration_samples, 0);
  rb_include_module(SoundFileClass, UgenModule);
}