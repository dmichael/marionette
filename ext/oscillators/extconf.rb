require '../extinclude.rb'

# copy the files you need into a tmp dir that will be deleted
candidates = Dir.glob([
  "ruby_oscillators.cpp",
  "../shared.h",
  "../../marionette/Oscillators.*",
  "../../marionette/Observable.*",
  "../../marionette/Unit.*",


  "../../stk/SineWave.*",
  "../../stk/Generator.*",

  "../../stk/soundcard.*",
  "../../stk/RtAudio.*",
  "../../stk/RtError.*",
  "../../stk/Stk.*"
  ]
)

source_files = candidates.delete_if do |item|
              item.include?(".svn")
end

tmp_dir = './tmp/.'

FileUtils.mkdir_p tmp_dir
# Copy all the files into one place for compilation
FileUtils.rm_r tmp_dir, :verbose => true, :force => true # prepare the directory that we are going to build in
FileUtils.cp_r source_files, tmp_dir, :verbose => true

create_makefile("oscillators", "tmp")