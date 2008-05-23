/*
This is the header file for inclusion into the bindings files. 
It contains definitions of the functions that need to be accessed when building the library
*/
#include "SineWaveUgen.h"
#ifndef __A_Audio__H_
#define __A_Audio__H_

#if __cplusplus
extern "C" {
#endif
  void c_initialize_library();
  void c_release_library();
  void c_run_audio();
  void c_stop_audio();
  void c_class_hello();
  void c_instance_hello();
  void c_audio_out_add(Ugen*);
  void c_audio_out_remove(Ugen*);  
  void c_init_sine();
  void c_audio_info();
#if __cplusplus
}
#endif


#endif // __Audio__H_

static VALUE AudioModule;
static VALUE AudioOut;
static VALUE UgenModule;
static VALUE SineWaveClass;
static VALUE SoundFileClass;
static VALUE PannerClass;