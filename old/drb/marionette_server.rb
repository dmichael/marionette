$:.unshift "../lib", "../ext"
require 'rubygems'
require 'audio/audio'
require 'soundfile/soundfile'
require 'marionette'
require 'oscillators/oscillators'
require 'anuran'
require 'activeresource'

require 'drb'

DRb.start_service("druby://:7777", Marionette.new)

DRb.thread.join
