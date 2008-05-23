
#include "Marionette.h"
// some variables that are to be used scriptwide
unsigned int inputs = 2;
unsigned int outputs = 2;
StkFrames frames;

// callback funtion used by the audio engine
inline int audioCallback( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
  Marionette *marionette = (Marionette *) dataPointer;
  // not sure why this pointer needs to be essentially renamed
  register StkFloat *samples = (StkFloat *) outputBuffer;

	// run Marionette - calculate all the dependencies and what not
	marionette->execute();
	
	for ( unsigned int i=0; i<frames.size(); i++ )
    *samples++ = frames[i];
  
  return 0;
}

/******************************
Marionette::Marionette
******************************/

Marionette::Marionette()
{
  //std::cout << "initializing Marionette C++ object" << std::endl;
  dac = new RtAudio;
    
  int output_device_id = dac->getDefaultOutputDevice();
  int input_device_id  = dac->getDefaultInputDevice();
  
  int num_out_channels = dac->getDeviceInfo(output_device_id).outputChannels;
  int num_in_channels  = dac->getDeviceInfo(input_device_id).inputChannels;
  parameters.deviceId  = output_device_id;
  parameters.nChannels = num_out_channels;
  printf("Device id: %d, channels %d\n", output_device_id, num_out_channels);
  // Resize the StkFrames object appropriately.
  frames.resize( RT_BUFFER_SIZE, num_out_channels );
  // default sample rate
  Stk::setSampleRate( 44100.0 );
  // create the global outs
  out = new Bus(num_out_channels);
  // create the global ins
  in  = new Bus(num_in_channels);
  
}


/******************************
Marionette::Marionette
******************************/

Marionette::Marionette(int outdevice, float samplerate)
{
  //std::cout << "initializing Marionette C++ object" << std::endl;
  dac = new RtAudio;
    
  int output_device_id    = outdevice;  
  int num_out_channels = dac->getDeviceInfo(output_device_id).outputChannels;

  parameters.deviceId = output_device_id;
  parameters.nChannels = num_out_channels;
  printf("Device id: %d , channels %d\n", output_device_id, num_out_channels);
  // Resize the StkFrames object appropriately.
  frames.resize( RT_BUFFER_SIZE, num_out_channels );
  // default sample rate
  Stk::setSampleRate( samplerate );
  // create the global outs
  out = new Bus(num_out_channels);
  // create the global ins
  in = new Bus(0);
}

/******************************
Marionette::~Marionette
******************************/

Marionette::~Marionette()
{
  std::cout << "Deleteing Marionette C++ object" << std::endl;
  Stop();
  delete dac;
  delete out;
  delete in;
}


/******************************
Marionette::execute
******************************/

void Marionette::execute()
{
  //double startnow = bufclock * blockduration;
  //bufclock++;
  //double endnow = bufclock * blockduration; 
  
  // calculate all the ouputs
  this->out->tickFrame( frames );
  //	current = this;
  //	
  //	DEBUG("next callback %f %i\n", now, bufclock);

  // garbage collect
  
  // update clock
	//now = endnow;
}

/******************************
Marionette::addUnit
******************************/

void Marionette::addUnit(Unit *ugen)
{
  if(!out->add(ugen)){
    std::cout << "You are already playing this Unit.\n";      
  }
}

/******************************
Marionette::removeUnit
******************************/

void Marionette::removeUnit(Unit *ugen)
{
  if(!out->remove(ugen)){
    std::cout << "This Unit is not playing.\n";      
  }
}


/*******************
Marionette::Run
*******************/

void Marionette::Run()
{
  if(dac->isStreamOpen()){
    std::cout << "A stream is already running!";  
  }
  else
  {
    std::cout << "Starting audio stream...";
    RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
    bufferSize = RT_BUFFER_SIZE;

    
    

    try {
      dac->openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferSize, &audioCallback, (void *)this );
      dac->startStream();
    }
    catch ( RtError &error ) {
      error.printMessage();
    }
    
  }
  std::cout << "audio subsystem is running." << std::endl;
  std::cout << "Ornament is the decoration of mortality.\n" << std::endl;
  // Office for Soft Architecture.
}


/*******************
Marionette::Stop
*******************/

void Marionette::Stop()
{
  if(dac->isStreamOpen()){
  std::cout << "stopping audio stream...\n" << std::endl;
    try {
      dac->closeStream();
    }
    catch ( RtError &error ) {
      error.printMessage();
    }
  }
}

void Marionette::info()
{
  // Create an api map.
  std::map<int, std::string> apiMap;
  apiMap[RtAudio::MACOSX_CORE] = "OS-X Core Audio";
  apiMap[RtAudio::WINDOWS_ASIO] = "Windows ASIO";
  apiMap[RtAudio::WINDOWS_DS] = "Windows Direct Sound";
  apiMap[RtAudio::UNIX_JACK] = "Jack Client";
  apiMap[RtAudio::LINUX_ALSA] = "Linux ALSA";
  apiMap[RtAudio::LINUX_OSS] = "Linux OSS";
  apiMap[RtAudio::RTAUDIO_DUMMY] = "RtAudio Dummy";

  std::vector< RtAudio::Api > apis;
  RtAudio :: getCompiledApi( apis );

  std::cout << "\nCompiled APIs:\n";
  for ( unsigned int i=0; i<apis.size(); i++ )
    std::cout << "  " << apiMap[ apis[i] ] << std::endl;

  RtAudio audio;
  RtAudio::DeviceInfo info;

  std::cout << "\nCurrent API: " << apiMap[ audio.getCurrentApi() ] << std::endl;

  unsigned int devices = audio.getDeviceCount();
  std::cout << "\nFound " << devices << " device(s) ...\n";

  for (unsigned int i=0; i<devices; i++) {
    info = audio.getDeviceInfo(i);

    std::cout << "\nDevice Name = " << info.name << '\n';
    if ( info.probed == false )
      std::cout << "Probe Status = UNsuccessful\n";
    else {
      std::cout << "Probe Status = Successful\n";
      std::cout << "Output Channels = " << info.outputChannels << '\n';
      std::cout << "Input Channels = " << info.inputChannels << '\n';
      std::cout << "Duplex Channels = " << info.duplexChannels << '\n';
      if ( info.isDefaultOutput ) std::cout << "This is the default output device.\n";
      else std::cout << "This is NOT the default output device.\n";
      if ( info.isDefaultInput ) std::cout << "This is the default input device.\n";
      else std::cout << "This is NOT the default input device.\n";
      if ( info.nativeFormats == 0 )
        std::cout << "No natively supported data formats(?)!";
      else {
        std::cout << "Natively supported data formats:\n";
        if ( info.nativeFormats & RTAUDIO_SINT8 )
          std::cout << "  8-bit int\n";
        if ( info.nativeFormats & RTAUDIO_SINT16 )
          std::cout << "  16-bit int\n";
        if ( info.nativeFormats & RTAUDIO_SINT24 )
          std::cout << "  24-bit int\n";
        if ( info.nativeFormats & RTAUDIO_SINT32 )
          std::cout << "  32-bit int\n";
        if ( info.nativeFormats & RTAUDIO_FLOAT32 )
          std::cout << "  32-bit float\n";
        if ( info.nativeFormats & RTAUDIO_FLOAT64 )
          std::cout << "  64-bit float\n";
      }
      if ( info.sampleRates.size() < 1 )
        std::cout << "No supported sample rates found!";
      else {
        std::cout << "Supported sample rates = ";
        for (unsigned int j=0; j<info.sampleRates.size(); j++)
          std::cout << info.sampleRates[j] << " ";
      }
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
}