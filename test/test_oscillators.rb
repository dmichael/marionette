$:.unshift "../ext/audio/", "../ext/oscillators/" 
require 'audio'
require 'oscillators'
require 'test/unit'


class TestOscillators < Test::Unit::TestCase
  
  def test_sine
    begin
      Audio.initialize
      Audio.run
      s = Sine.new :freq => 440
      s.play
      sleep 10
    ensure
      Audio.release
    end
  end  

end