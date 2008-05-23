#include "ruby.h"
#include "Unit.h"

RUBY_EXTERN VALUE SineWaveClass;
RUBY_EXTERN VALUE AudioModule;
RUBY_EXTERN VALUE AudioOut;
RUBY_EXTERN VALUE UnitModule;
RUBY_EXTERN VALUE SoundFileClass;
RUBY_EXTERN VALUE SoundLoopClass;
RUBY_EXTERN VALUE GreenfieldClass;

RUBY_EXTERN void rb_ugen_dealloc(Unit *);