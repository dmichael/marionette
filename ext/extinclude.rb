# $Id: extconf.rb 526 2007-09-11 17:43:23Z blackhedd $
#
#----------------------------------------------------------------------------
#
# Copyright (C) 2006-07 by Francis Cianfrocca. All Rights Reserved.
# Gmail: blackhedd
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of either: 1) the GNU General Public License
# as published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version; or 2) Ruby's License.
# 
# See the file COPYING for complete licensing information.
#
#---------------------------------------------------------------------------
#
# We have to munge LDSHARED because this code needs a C++ link.
#

require 'mkmf'
require 'fileutils'


flags = []
libs  = []

case RUBY_PLATFORM.split('-',2)[1]


##########
# Windoze
##########
when 'mswin32', 'mingw32', 'bccwin32'

##########
# MacOS X 
##########
when /darwin/
  flags << '-O3 -Wall -D__MACOSX_CORE__ -D__GXX__'
  libs  << '-lm'
  libs  << '-framework CoreAudio -framework CoreFoundation -framework CoreMidi'
  # on Unix we need a g++ link, not gcc.
  # Ff line contributed by Daniel Harple.
  CONFIG['LDSHARED'] = "$(CXX) " + CONFIG['LDSHARED'].split[1..-1].join(' ')
  
##########
# Linux
##########
when /linux/
  # ALSA
  flags << '-O3 -Wall -D__LINUX_ALSA__ -D__LINUX_ALSASEQ__ -D__LITTLE_ENDIAN__'
  libs  << '-lasound -lpthread'
  
  # JACK
  #flags << '-O3 -Wall -D__UNIX_JACK__ -D__LINUX_ALSASEQ__ -D__LITTLE_ENDIAN__'
  #libs  << "`pkg-config --cflags --libs jack` -lpthread"
  
  # OSS
  #flags << '-O3 -Wall -D__LINUX_OSS__ -D__LITTLE_ENDIAN__'
  #libs  << "-lpthread"
  
  CONFIG['LDSHARED'] = "$(CXX) " + CONFIG['LDSHARED'].split[1..-1].join(' ')
  
end

# now join the flags with the generated file
if $CPPFLAGS
  $CPPFLAGS += ' ' + flags.join(' ')
else
  $CFLAGS += ' ' + flags.join(' ')
end

if $LOCAL_LIBS
  $LOCAL_LIBS += ' ' + libs.join(' ')
else
  $LOCAL_LIBS = libs.join(' ')
end
