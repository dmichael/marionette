$:.unshift "../ext/"
require 'rubygems'
require 'audio/audio'
require 'greenfield/greenfield'
require 'test/unit'


class TestSoundfile < Test::Unit::TestCase
  
  def test_greenfield
    begin
      Audio.initialize
      Audio.run
      greenfield = Greenfield.new
    ensure
      Audio.release
    end
  end  

end