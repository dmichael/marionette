#$:.unshift "../ext/audio/", "../ext/soundfile/" 
require 'rubygems'
require 'marionette'
#require 'soundfile'
require 'test/unit'


class TestSoundfile < Test::Unit::TestCase
  
  def test_soundfile
    begin
      Audio.initialize
      Audio.run
      soundfile = SoundFile.new :file => "dreemz_edit.wav"
      soundfile.play
      sleep soundfile.duration
    ensure
      Audio.release
    end
  end  

end