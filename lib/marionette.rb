require 'audio'
require 'oscillators'
require 'soundfile'
require 'soundloop'

module Marionette
  
  def self.initialize
    Audio.initialize
  end
  
  def self.start(opts = {})  
    device = opts[:device]
    samplerate = opts[:samplerate]
    Audio.initialize device, samplerate
    Audio.run
  end
  
  def self.stop
    Audio.release
  end
  
  def self.probe
    Audio.info
  end    

end