#include <ruby.h>
#include "shared.h"
#include "SimpleSampler.h"

VALUE SoundLoopClass;

static VALUE rb_sound_loop_alloc(VALUE self)
{
  SimpleSampler *ugen = new SimpleSampler;
  VALUE obj = Data_Wrap_Struct(self, 0, rb_ugen_dealloc, ugen);
	return obj;
}

static VALUE SoundLoop_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE options;
  /* One mandatory, one optional argument */
  rb_scan_args(argc, argv, "01", &options);
  
  SimpleSampler *soundfileugen;
  Data_Get_Struct(self, SimpleSampler, soundfileugen);
  
  if(options != Qnil){
    Check_Type(options, T_HASH);
    VALUE filename = rb_hash_aref(options, ID2SYM(rb_intern("file")));     
    
    soundfileugen->open(StringValuePtr(filename));
    // make sure its not playing
    soundfileugen->keyOff();
  }    
	return self;
}

static VALUE SoundLoop_name(VALUE self) 
{
  SimpleSampler *ugen;
  Data_Get_Struct(self, SimpleSampler, ugen);
	return rb_str_new2(ugen->name());
}

static VALUE SoundLoop_set_rate(VALUE self, VALUE rate) 
{
  //   // get the Unit instance
  //   SimpleSampler *ugen;
  //   Data_Get_Struct(self, SimpleSampler, ugen); 
  //   
  //   if(TYPE(rate) == T_FLOAT || TYPE(rate) == T_FIXNUM){
  //     float rate_f = (float) NUM2DBL(rate);
  //     // set the frequency
  //     ugen->setRate(rate_f);
  //     // store what was set in the instance variable
  //     rb_iv_set(self, "@rate", rate);
  //   }
  // return rate;
  return Qnil;
}

static VALUE SoundLoop_get_rate(VALUE self) 
{  
  // This is done in Ruby because the STK SineWave.cpp class has no getter
  return rb_iv_get(self, "@rate");
}

static VALUE SoundLoop_open(VALUE self, VALUE filename) 
{
  // We want a string here...
  Check_Type(filename, T_STRING);
  // get the Unit instance
  SimpleSampler *ugen;
  Data_Get_Struct(self, SimpleSampler, ugen);

  // Open the file, rethrowing errors as necessary  
  try {
    ugen->open(StringValuePtr(filename));  
    ugen->keyOff();
  }
  catch(StkError &error) {
    return self;
  }
	return self;
}

static VALUE SoundLoop_restart(VALUE self) 
{
  SimpleSampler *sampler;
  Data_Get_Struct(self, SimpleSampler, sampler);
  sampler->keyOn();
  return Qtrue;
}

static VALUE SoundLoop_duration(VALUE self) 
{
  //   SimpleSampler *ugen;
  //   Data_Get_Struct(self, SimpleSampler, ugen);
  //   
  //   StkFloat duration = ugen->durationInSeconds();
  // return rb_float_new(duration);
  return Qnil;
}

static VALUE SoundLoop_duration_samples(VALUE self) 
{
  //   SimpleSampler *ugen;
  //   Data_Get_Struct(self, SimpleSampler, ugen);
  //   
  //   unsigned long duration = ugen->durationInSamples();
  // return LONG2NUM(duration);
  return Qnil;
}

static VALUE SoundLoop_keyon(VALUE self) 
{
  SimpleSampler *ugen;
  Data_Get_Struct(self, SimpleSampler, ugen);
  ugen->keyOn();
  return Qtrue;
}

static VALUE SoundLoop_keyoff(VALUE self) 
{
  SimpleSampler *ugen;
  Data_Get_Struct(self, SimpleSampler, ugen);
  ugen->keyOff();
  return Qtrue;
}

static VALUE SoundLoop_adsr(VALUE self, VALUE a, VALUE d, VALUE s, VALUE r) 
{
  SimpleSampler *ugen;
  Data_Get_Struct(self, SimpleSampler, ugen);
  
  if(TYPE(a) == T_FLOAT || TYPE(a) == T_FIXNUM){
    ugen->adsr_.setAttackTime(a);
  }
  if(TYPE(d) == T_FLOAT || TYPE(d) == T_FIXNUM){
    ugen->adsr_.setDecayTime(d);
  }
  if(TYPE(s) == T_FLOAT || TYPE(s) == T_FIXNUM){
    ugen->adsr_.setSustainLevel(1.0);
  }
  if(TYPE(r) == T_FLOAT || TYPE(r) == T_FIXNUM){
    ugen->adsr_.setReleaseTime(r);
  }

  return Qtrue;
}

static VALUE SoundLoop_set_gain(VALUE self, VALUE amplitude) 
{
  // get the Unit instance
  SimpleSampler *ugen;
  Data_Get_Struct(self, SimpleSampler, ugen); 
  
  if(TYPE(amplitude) == T_FLOAT || TYPE(amplitude) == T_FIXNUM){
    float amplitude_f = (float) NUM2DBL(amplitude);
    // set the frequency
    ugen->setGain(amplitude_f);
  }
  return amplitude;
}

extern "C" void Init_soundloop() 
{
  // SoundFile
  SoundLoopClass = rb_define_class("SoundLoop", rb_cObject);
  rb_define_alloc_func(SoundLoopClass, rb_sound_loop_alloc);
  rb_define_method(SoundLoopClass, "initialize", (VALUE(*)(...)) SoundLoop_initialize, -1);
  rb_define_method(SoundLoopClass, "name", (VALUE(*)(...)) SoundLoop_name, 0);
  rb_define_method(SoundLoopClass, "rate=", (VALUE(*)(...)) SoundLoop_set_rate, 1);
  rb_define_method(SoundLoopClass, "rate", (VALUE(*)(...)) SoundLoop_get_rate, 0);
  rb_define_method(SoundLoopClass, "open", (VALUE(*)(...)) SoundLoop_open, 1);
  rb_define_method(SoundLoopClass, "restart", (VALUE(*)(...)) SoundLoop_restart, 0);
  rb_define_method(SoundLoopClass, "duration", (VALUE(*)(...)) SoundLoop_duration, 0);
  rb_define_method(SoundLoopClass, "duration_in_samples", (VALUE(*)(...)) SoundLoop_duration_samples, 0);
  rb_define_method(SoundLoopClass, "keyon", (VALUE(*)(...)) SoundLoop_keyon, 0);
  rb_define_method(SoundLoopClass, "keyoff", (VALUE(*)(...)) SoundLoop_keyoff, 0);
  rb_define_method(SoundLoopClass, "adsr", (VALUE(*)(...)) SoundLoop_adsr, 4);
  rb_define_method(SoundLoopClass, "gain=", (VALUE(*)(...)) SoundLoop_set_gain, 1);
  rb_include_module(SoundLoopClass, UnitModule);
}