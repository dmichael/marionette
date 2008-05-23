#$:.unshift "../lib", "../ext"
require 'audio'
require 'oscillators'
require 'soundfile'
require 'soundloop'

module Marionette
  def self.initialize
    Audio.initialize
  end
  
  def self.start  
    Audio.run
  end
  
  def self.stop
    Audio.release
  end
  
  def self.probe
    Audio.info
  end
  
  def run_eval(a)
    eval(a)
    if block_given?
      ss = "there is a block"
      yield ss
    end
  end

  def run_proc(proc)
    proc.call
  end
  
end
# def run
#   Audio.initialize
#   Audio.run
# end
# 
# def stop
#   Audio.release
# end
# 
# def probe
#   Audio.info
# end
