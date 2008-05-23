require '../extinclude.rb'

# copy the files you need into a tmp dir that will be deleted
candidates = Dir.glob([
  "ruby_soundfile.cpp",
  "../shared.h",
  "../../stk/Stk.*",
  "../../stk/RtAudio.*",
  "../../stk/RtError.*",
  "../../stk/FileWvIn.*",
    "../../stk/FileRead.*",
  "../../stk/WvIn.*",
  "../../stk/soundcard.*",
  
  "../../marionette/SoundFile.*",
  "../../marionette/Observable.*",
  "../../marionette/Unit.*",
  "../../marionette/Rand.*"
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

create_makefile("soundfile", "tmp")