$:.unshift "../ext/audio/", "../ext/oscillators/" 
require 'audio'
require 'oscillators'

require 'drb'
DRb.start_service
marionette = DRbObject.new(nil, 'druby://:7777')
# marionette.probe
marionette.start
marionette.run_proc( 
  Proc.new{ s = Sine.new(:freq => 440); p "huh"; sleep 5}
)

sleep 5
