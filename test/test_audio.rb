$:.unshift "../ext/audio/"
require 'audio'
require 'test/unit'

class TestBasic < Test::Unit::TestCase
  
  def test_00_info
    begin
      Audio.initialize
      Audio.info
    ensure
      Audio.release
    end
  end
  
  def test_01_audio
  begin
    # 1) initializes the audio subsystem,
    Audio.initialize
    # 2) then starts the audio callback via RtAudio to evaluate
    Audio.run
    sleep 1
  ensure
    Audio.release
    # TODO: choose who is to manage the held loop - in C++ or in Ruby...?
  end
  end
  

end