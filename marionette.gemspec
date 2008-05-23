require 'rubygems'
spec = Gem::Specification.new do |s|
  s.name = "marionette"
  s.version = "0.0.1"
  s.author = "David M Michael"
  s.email = "david@unnature.net"
  s.rubyforge_project = 'marionette'
  s.homepage = "http://marionette.rubyforge.org"
  s.platform = Gem::Platform::RUBY
  s.summary = "Marionette is audio synthesis in Ruby."
  candidates = Dir.glob("{ext,lib,marionette,stk,test,tmp}/**/*")
  s.files = candidates.delete_if do |item|
              item.include?(".svn")
              item.include?("anuran.rb")
              item.include?("unnature.rb")
            end
  s.require_path = "lib"
  s.test_file    = "test/test_basic.rb"
  s.has_rdoc     = true
  s.extra_rdoc_files = ["README"]
  s.extensions = ["ext/audio/extconf.rb", "ext/oscillators/extconf.rb", "ext/soundfile/extconf.rb", "ext/soundloop/extconf.rb"]
end
if $0 == __FILE__
  Gem::manage_gems
  Gem::Builder.new(spec).build
end