$:.unshift "../lib", "../ext"
require 'rubygems'
#require 'unnature'
require 'audio/audio'
require 'soundfile/soundfile'
require 'greenfield/greenfield'
require 'anuran'
require 'activeresource'

class God
  def self.make(species)
    case species
      when "peeper" 
        Anuran.new(
          :file      => "../soundfiles/peeper.wav", 
          :coords    => [rand_range(-100.0, 100.00), rand_range(-100.0, 100.00)],
          :period    => rand_range(0.6, 0.8), 
          :effector_delay => rand_range(0.05, 0.07), 
          :decending_length => rand_range(0.090, 0.110), 
          :spl       => rand_range(1,5), #db
          :prc_slope => rand_range(0.1, 0.3), # [0 - 5]
          :rebound   => rand_range(0.05, 0.07),
          :threshold => rand_range(0.2, 0.5),
          :position  => {:position => rand_range(-1, 1) },
          :acceleration => rand_range(0.2, 0.5)

        )
      when "copes"
        Anuran.new(
          :file      => "../soundfiles/copes_gray_treefrog.wav", 
          :coords    => [rand_range(-100.0, 100.00), rand_range(-100.0, 100.00)],
          :period    => rand_range(6.5, 7.6), 
          :effector_delay => rand_range(0.05, 0.07), 
          :decending_length => rand_range(0.090, 0.110), 
          :spl       => rand_range(5,30), #db
          :prc_slope => rand_range(0.1, 0.3), # [0 - 5]
          :rebound   => 0.0,
          :threshold => 0.2,
          :position  => {:position => rand_range(-1, 1)}
        )
     when "cricket"
        Anuran.new(
          :file      => "../soundfiles/allards_ground_cricket_chirp.wav", 
          :coords    => [rand_range(-100.0, 100.00), rand_range(-100.0, 100.00)],
          :period    => rand_range(0.05, 0.05), 
          :effector_delay => rand_range(0.05, 0.07), 
          :decending_length => rand_range(0.090, 0.110), 
          :spl       => rand_range(1,2), #db
          :prc_slope => rand_range(0.1, 0.3), # [0 - 5]
          :rebound   => 0.0,
          :threshold => 0.2,
          :position  => {:position => rand_range(-1, 1)},
          :acceleration => rand_range(0.02, 0.05)
        )
      when "katydid"
        Anuran.new(
          :file      => "../soundfiles/common_meadow_katydid3.wav", 
          :coords    => [rand_range(-100.0, 100.00), rand_range(-100.0, 100.00)],
          :period    => rand_range(8.5, 10.05), 
          :effector_delay => rand_range(0.05, 0.07), 
          :decending_length => rand_range(0.090, 0.110), 
          :spl       => rand_range(4,10), #db
          :prc_slope => rand_range(0.1, 0.3), # [0 - 5]
          :rebound   => 0.0,
          :threshold => 0.2,
          :position  => {:position => rand_range(-1, 1)}
          # :acceleration => rand_range(0.2, 0.5)
        )

    end
  end
  
  def self.place_frogs(frogs)
    frogs.each_with_index do |frog, i|
      frogs.each_with_index do |f, j|
        if frog != frogs[j]
          frog.listen_to frogs[j] 
        end
      end
    end
    
  end
end


  
begin
  Audio.initialize
  Audio.run
  
  crickets = []
  3.times do 
    crickets.push God.make "cricket"
  end
  God.place_frogs crickets
  crickets.each do |cricket|
    cricket.play
    sleep (rand + 1)
  end


  katydids = []
  2.times do 
    katydids.push God.make "katydid"
  end
  God.place_frogs katydids

  katydids.each do |frog|
    frog.play
    sleep (rand + 1)
  end

  
  
  copes = []
  3.times do 
    copes.push God.make "copes"
  end
  God.place_frogs copes

  copes.each do |frog|
    frog.play
    sleep (rand + 1)
  end

  
  peepers = []
  10.times do 
    peepers.push God.make "peeper"
  end
  God.place_frogs peepers

  peepers.each do |frog|
  #  frog.play
    sleep (rand + 1)
  end
  

  

  while true do
    # holding pattern
    sleep 10
    
    peepers.each{|peeper| 
      peeper.cpg.threshold = 1.0; sleep (rand + 0.5)
    }
    
    sleep 10
    p "setting new threshold"
    peepers.each{|peeper| 
      peeper.cpg.threshold =+ 0.6
      sleep (rand + 2)
    }
  end

ensure
  Audio.release
end



