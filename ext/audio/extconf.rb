require '../extinclude.rb'

# copy the files you need into a tmp dir that will be deleted
source_files = Dir.glob([
  "audio.cpp",
  "../shared.h",
  "../../marionette/Marionette.*",
  "../../marionette/Bus.*",
  "../../marionette/Observable.*",
  "../../marionette/Unit.*",
  
  
  "../../marionette/Effects.*",
  "../../stk/Effect.*",
  "../../stk/PRCRev.*",
  "../../stk/Delay.*",
  "../../stk/Filter.*",
  
  "../../stk/soundcard.*",
  "../../stk/RtAudio.*",
  "../../stk/RtError.*",
  "../../stk/Stk.*"

  ]
)

tmp_dir = './tmp/.'

FileUtils.mkdir_p tmp_dir
# Copy all the files into one place for compilation
FileUtils.rm_r tmp_dir, :verbose => true, :force => true # prepare the directory that we are going to build in
FileUtils.cp_r source_files, tmp_dir, :verbose => true

create_makefile("audio", "tmp")