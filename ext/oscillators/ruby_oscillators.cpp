#include <ruby.h>
#include "shared.h"
#include "Oscillators.h"

static VALUE rb_sine_alloc(VALUE self)
{
  SineWaveUnit *sine = new SineWaveUnit;
  VALUE obj = Data_Wrap_Struct(self, 0, rb_ugen_dealloc, sine);
	return obj;
}

static VALUE Sine_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE options;
  /* One mandatory, one optional argument */
  rb_scan_args(argc, argv, "01", &options);
  

  // get the Unit instance
  SineWaveUnit *sine;
  Data_Get_Struct(self, SineWaveUnit, sine);
  
  if(options != Qnil){
     Check_Type(options, T_HASH);
     VALUE freq = rb_hash_aref(options, ID2SYM(rb_intern("freq")));
     rb_iv_set(self, "@freq", freq); // this may be unnecessary
     sine->setFrequency(FIX2INT(freq));
   }
  // initialize the parameters
  //sine->source->setFrequency(FIX2INT(freq));
	return self;
}

static VALUE rb_sine_set_frequency(VALUE self, VALUE frequency) 
{
  // get the Unit instance
  SineWaveUnit *sine;
  Data_Get_Struct(self, SineWaveUnit, sine); 

  // set the frequency  
  if(TYPE(frequency) != T_DATA || RDATA(frequency)->dfree != (RUBY_DATA_FUNC)rb_ugen_dealloc){
    Unit *mod;
    Data_Get_Struct(frequency, Unit, mod);
    sine->addFreqMod(mod);
  }
  else{
    sine->source->setFrequency(FIX2INT(frequency));
  }
  
  // store what was set in the instance variable
  rb_iv_set(self, "@freq", frequency);
	return frequency;
}

static VALUE rb_sine_get_frequency(VALUE self) 
{  
  // This is done in Ruby because the STK SineWave.cpp class has no getter
  return rb_iv_get(self, "@freq");
}

static VALUE rb_sine_name(VALUE self) 
{
  SineWaveUnit *sine;
  Data_Get_Struct(self, SineWaveUnit, sine);
	return rb_str_new2(sine->name());
}

VALUE SineWaveClass;

extern "C" void Init_oscillators() 
{
  // Sine
  SineWaveClass = rb_define_class("Sine", rb_cObject);
  rb_define_alloc_func(SineWaveClass, rb_sine_alloc);
  rb_define_method(SineWaveClass, "initialize", (VALUE(*)(...)) Sine_initialize, -1);
  rb_define_method(SineWaveClass, "name", (VALUE(*)(...)) rb_sine_name, 0);
  rb_define_method(SineWaveClass, "freq=", (VALUE(*)(...)) rb_sine_set_frequency, 1);
  rb_define_method(SineWaveClass, "freq", (VALUE(*)(...)) rb_sine_get_frequency, 0);  
  rb_include_module(SineWaveClass, UnitModule);
}