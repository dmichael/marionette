#$:.unshift "../ext/audio/", "../ext/soundfile/" 
require 'rubygems'
require 'marionette'
#require 'soundfile'
require 'test/unit'


class TestSoundLoop < Test::Unit::TestCase
  
  def test_soundloop
    begin
      Audio.initialize
      Audio.run
      sleep 3#soundfile.duration
      
      soundloop = SoundLoop.new
      soundloop.open "stream.wav"
      Audio::Out.add soundloop
      sleep 3#soundfile.duration
      soundloop.keyon
      sleep 3
      #l.restart
    ensure
      Audio.release
    end
  end  

end