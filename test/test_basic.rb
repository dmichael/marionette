$:.unshift "../lib"
require 'test/unit'
require 'Marionette'
require 'Anuran'


class TestBasic < Test::Unit::TestCase
  
  def test_00_info
    begin
      Audio.initialize
      Audio.info
    ensure
      Audio.release
    end
  end
  
  def test_01_audio
  begin
    # 1) initializes the audio subsystem,
    Audio.initialize
    # 2) then starts the audio callback via RtAudio to evaluate
    Audio.run
    sleep 1
  ensure
    Audio.release
    # TODO: choose who is to manage the held loop - in C++ or in Ruby...?
  end
  end
  
  def test_02_sine
      sine = Sine.new(:freq => 440)
      sine2 = Sine.new(:freq => 440)
      sine3 = Sine.new(:freq => 440)
      puts sine
      puts sine3
      puts sine
      sine.name
      sine2.name
  end
  
  def test_03_add_ugen_to_out
    begin  
      Audio.initialize
      Audio.run
      sine = Sine.new(:freq => 440)
      #       sine2 = Sine.new(:freq => 840)
      # 
      
      #       
      #p Ugen.public_methods.sort.join(", ")
      sine.play
      sleep 10

      sine.stop
      # 
      #       #Audio::Out.add Sine.new({:freq => 540, :test => 666})
      # 
      #       sleep 1
      #       
      #       5.times do 
      #         sine.freq += 40 
      #         sleep 0.2
      #       end
      #       sines=[]
      #       20.times do |x|
      #         sines[x] = Sine.new(:freq => 440)
      #       end
      #       20.times do |x|
      #         sines[x].play
      #         sleep 1
      #       end
      #       sleep 10  
      #       # cleanup
      #       20.times do |x|
      #         sines[x].stop
      #       end

    ensure
      Audio.release
      # TODO: choose who is to manage the held loop - in C++ or in Ruby...?
    end
    # Audio::Out.add Array.new
  end
  
  def test_04_sound_file
    # begin  
    #       Audio.initialize
    #       Audio.run
    # 
    #       soundfiles=[]
    #       1.times do |x|
    #         soundfiles[x] = SoundFile.new
    #       end
    #       1.times do |x|
    #         Audio::Out.add soundfiles[x]
    #         sleep 1
    #       end
    #       sleep 40
    #       # cleanup
    #       1.times do |x|
    #         Audio::Out.remove soundfiles[x]
    #       end
    #       
    #     ensure
    #       Audio.release
    #       # TODO: choose who is to manage the held loop - in C++ or in Ruby...?
    #     end
    #     # Audio::Out.add Array.new
  end

  
  def test_06_soundfile
    begin
      Audio.initialize
      Audio.run
    
      # soundfile  = SoundFile.new
      # test = SoundFile.new :file => "../soundfiles/test.wav"
      # test.play
      # p test.duration
      # p test.duration_in_samples
      # sleep 2
      # soundfile.open "../soundfiles/peeper.wav"
      # soundfile.play
      # sleep 2
      # p soundfile.duration
      # p soundfile.duration_in_samples

      treefrog = Anuran.new(
        :file => "../soundfiles/test.wav", 
        :coords => [166,10],
        :period => 1, 
        :effector_delay => 0.1, 
        :decending_length => 0.1, 
        :prc => 0.7 )
        
    p treefrog.call_duration
    p treefrog.cpg.host_data
    p treefrog.cpg.host_data = {}
    p treefrog.cpg.host_data = {:y=>666}
    p treefrog.cpg.host_data        
    p treefrog.cpg.host_data = {:x=>777}
    p treefrog.cpg.host_data        
    ensure
      Audio.release
    end
  end
  
  def test_07_sine_rates
    begin
      Audio.initialize
      Audio.run
      #sine = Sine.new :freq => 440
      #mod  = Sine.new :freq => 10
      #sine.play
      #sleep 5
    ensure
      Audio.release
    end
  end
  
end