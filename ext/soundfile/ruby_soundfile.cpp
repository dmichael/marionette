#include <ruby.h>
#include "shared.h"
#include "SoundFile.h"

VALUE SoundFileClass;

static VALUE rb_sound_file_alloc(VALUE self)
{
  SoundFile *ugen = new SoundFile;
  VALUE obj = Data_Wrap_Struct(self, 0, rb_ugen_dealloc, ugen);
	return obj;
}

static VALUE SoundFile_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE options;
  /* One mandatory, one optional argument */
  rb_scan_args(argc, argv, "01", &options);
  
  SoundFile *soundfileugen;
  Data_Get_Struct(self, SoundFile, soundfileugen);
  
  if(options != Qnil){
    Check_Type(options, T_HASH);
    VALUE filename = rb_hash_aref(options, ID2SYM(rb_intern("file")));     
    
    soundfileugen->open(StringValuePtr(filename));
  }    
	return self;
}

static VALUE SoundFile_name(VALUE self) 
{
  SoundFile *ugen;
  Data_Get_Struct(self, SoundFile, ugen);
	return rb_str_new2(ugen->name());
}

static VALUE SoundFile_set_rate(VALUE self, VALUE rate) 
{
  // get the Unit instance
  SoundFile *ugen;
  Data_Get_Struct(self, SoundFile, ugen); 
  
  if(TYPE(rate) == T_FLOAT || TYPE(rate) == T_FIXNUM){
    float rate_f = (float) NUM2DBL(rate);
    // set the frequency
    ugen->setRate(rate_f);
    // store what was set in the instance variable
    rb_iv_set(self, "@rate", rate);
  }
	return rate;
}

static VALUE SoundFile_get_rate(VALUE self) 
{  
  // This is done in Ruby because the STK SineWave.cpp class has no getter
  return rb_iv_get(self, "@rate");
}

static VALUE SoundFile_open(VALUE self, VALUE filename) 
{
  // We want a string here...
  Check_Type(filename, T_STRING);
  // get the Unit instance
  SoundFile *ugen;
  Data_Get_Struct(self, SoundFile, ugen);

  // Open the file, rethrowing errors as necessary  
  try {
    ugen->open(StringValuePtr(filename));  
  }
  catch(StkError &error) {
    return self;
  }
	return self;
}

static VALUE SoundFile_restart(VALUE self) 
{
  SoundFile *ugen;
  Data_Get_Struct(self, SoundFile, ugen);
  ugen->reset();
  return Qtrue;
}

static VALUE SoundFile_duration(VALUE self) 
{
  SoundFile *ugen;
  Data_Get_Struct(self, SoundFile, ugen);
  
  StkFloat duration = ugen->durationInSeconds();
	return rb_float_new(duration);
}

static VALUE SoundFile_duration_samples(VALUE self) 
{
  SoundFile *ugen;
  Data_Get_Struct(self, SoundFile, ugen);
  
  unsigned long duration = ugen->durationInSamples();
	return LONG2NUM(duration);
}

static VALUE SoundFile_is_finished(VALUE self) 
{
  SoundFile *soundfile;
  Data_Get_Struct(self, SoundFile, soundfile);
  
  return (soundfile->is_finished())? Qtrue : Qfalse;
}


extern "C" void Init_soundfile() 
{
  // SoundFile
  SoundFileClass = rb_define_class("SoundFile", rb_cObject);
  rb_define_alloc_func(SoundFileClass, rb_sound_file_alloc);
  rb_define_method(SoundFileClass, "initialize", (VALUE(*)(...)) SoundFile_initialize, -1);
  rb_define_method(SoundFileClass, "name", (VALUE(*)(...)) SoundFile_name, 0);
  rb_define_method(SoundFileClass, "rate=", (VALUE(*)(...)) SoundFile_set_rate, 1);
  rb_define_method(SoundFileClass, "rate", (VALUE(*)(...)) SoundFile_get_rate, 0);
  rb_define_method(SoundFileClass, "open", (VALUE(*)(...)) SoundFile_open, 1);
  rb_define_method(SoundFileClass, "restart", (VALUE(*)(...)) SoundFile_restart, 0);
  rb_define_method(SoundFileClass, "duration", (VALUE(*)(...)) SoundFile_duration, 0);
  rb_define_method(SoundFileClass, "duration_in_samples", (VALUE(*)(...)) SoundFile_duration_samples, 0);
  rb_define_method(SoundFileClass, "is_finished?", (VALUE(*)(...)) SoundFile_is_finished, 0);
  rb_include_module(SoundFileClass, UnitModule);
}