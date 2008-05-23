#include "unistd.h"
#include "sigslot.h"
#include "Cricket.h" 

#include "RtAudio.h"
#include "Breeder.h"
#include "Dispatcher.h"
#include "Conductor.h"
#include "Anuran.h" 

#include "Speaker.h" 
#ifndef _DEBUG_
#define _DEBUG_
#endif

// The TickData structure holds all the class instances and data that
// are shared by the various processing functions.
// If any variable within this needs to be initialize, do so in the constructor TickData().
struct TickData 
{
  int channels;
  StkFrames frames;  

  // The conductor holds channel information and directs output traffic, creating the illusion of space.
  Conductor *conductor;
  // default constructor
  TickData()
  : channels(0){}
};
  
// This tick() function handles sample computation only.  It will be
// called automatically when the system needs a new buffer of audio
// samples.

// bufferSize is set in RtAudio::openStream() and passed in by RtAudio::setStreamCallback() and is set in Stk.h (512)
// I am assuming that *buffer is an array of size (bufferSize * number_of_channels) - the data written to it is interleaved.
// The number_of_channels is also set in RtAudio::openStream().

//int tick(char *buffer, int bufferSize, void *dataPointer)
int tick( void *buffer, void *inputBuffer, unsigned int bufferSize,
         double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
  // cast the dataPointer type
  TickData *data = (TickData *) dataPointer;
  //register StkFloat *samples = (StkFloat *) buffer; // cast the buffer type
  register StkFloat *samples = (StkFloat *) buffer; // cast the buffer type
 
  data->conductor->tickFrame( data->frames );
      
  // The buffer (samples) is interleaved. 
  for ( unsigned int i=0; i<data->frames.size(); i++ )
    *samples++ = data->frames[i];

  return 0;
}


int main(int argc, char *argv[]) 
{
  char *sample_rate = NULL;
  int numchannels = 0;
  int devicenum = 0;
  int index;
  int c;
  
  opterr = 0;
  
  // parse the command line options
  while ((c = getopt(argc, argv, "d:c:s:")) != -1) {
    switch (c)
    {
      case 'd':
        devicenum = atoi(optarg);
        break;
      case 'c':
        numchannels = atoi(optarg);
        break;
      case 's':
        sample_rate = optarg;
        break;
      case '?':
        if (optopt == 's' || optopt == 'c' || optopt == 'd')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%c'.\n", optopt);
        return 1;
      default:
        abort();
    }
  }
  
  for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);
  
  if(sample_rate == NULL){
    cout << "Default sample rate (44100) is being used" << endl;
    sample_rate = "44100";
  }
  else{
    cout << "The sample rate ("<< sample_rate <<") is being used" << endl;
  }
  // Set the global sample rate before creating class instances.
  Stk::setSampleRate( atoi(sample_rate) );
  // seed the random number generator
  srand(time(0) * getpid()); 
  
  // Initialize our core objects
  TickData data;
  RtAudio::DeviceInfo device;
  RtAudio *dac = 0;
  Dispatcher *dispatcher;
  Conductor *conductor;
  Breeder *breeder;
  int channels;
  RtAudio::StreamParameters parameters;
  // Here is where all the real work begins for streaming the realtime audio.
  // Figure out how many bytes in an StkFloat and setup the RtAudio object.
  // The number of channels is set dynamically based on the device available, this way 
  // RtAudio and Conductor can determine the number of channels available at compile time and configure itself.

  //-------------------------------------------------------------------- RtAudio setup
  RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
  unsigned int bufferSize = RT_BUFFER_SIZE;
  try 
  {
    dac    = new RtAudio();
    int device_count = dac->getDeviceCount();
    // device numbers start at 1... booo
    if(devicenum < 0 || devicenum > device_count){
      devicenum = 0;
      cout << "Default output device (0) being used." << endl;
    }
    device = dac->getDeviceInfo(devicenum);
    
    parameters.deviceId = devicenum;
    
    
    for(unsigned int i=0; i < device.sampleRates.size(); i++){
      cout << "Device supports these sample rates (" << device.sampleRates[i] << ")." << endl;
    }

    int device_channels = device.outputChannels;
    if(numchannels == 0 || numchannels > device_channels) {
      channels = device_channels;
      cout << "Default device channels (" << channels << ") being used." << endl;
    }
    else {
      channels = numchannels;
    }
    parameters.nChannels = channels;
#if defined(_DEBUG_)      
    cout << "This system has " << device_count << " devices\n";
    cout << "The chosen device (" << devicenum << ") has " << device.outputChannels << " channels\n";
#endif



    //-------------------------------------------------------------------- Main setup
    // Set up the rest of the objects
    dispatcher = new Dispatcher();
    conductor  = new Conductor(channels);
    breeder    = new Breeder(dispatcher, conductor);

    data.conductor = conductor;

    // 2) Create the populations
    //breeder->create_frogs( 0, "green_tree_frog" );
    //breeder->create_frogs( 0, "bird_voiced_tree_frog" );
    //breeder->create_frogs( 2, "bullfrog" );
    //breeder->create_frogs( 3, "greenfrog" );
      
    breeder->create_frogs( 4, "spring_peeper" );
    //breeder->create_frogs( 3, "graytreefrog" );

    //breeder->create_frogs( 0, "eastern_narrowmouth_toad" );    
    //breeder->create_frogs( 4, "brimleys_chorus_frog" );    
    //breeder->create_frogs( 2, "pickerel_frog" );  
    //breeder->create_frogs( 2, "americantoad" );    
  
    // TODO: these should be merged
   // breeder->create_frogs( 1, "allards_ground_cricket" );
    //breeder->create_frogs( 1, "greater_anglewing");  
    //breeder->create_frogs( 0, "snowy_tree_cricket" );  
    //breeder->create_frogs( 2, "common_meadow_katydid" );  
    //breeder->create_frogs( 2, "spring_ground_cricket" );  
    /*
    // 3) Tell the conductor who is playing
    // TODO: this should be moved to Breeder class
    std::map<int,Anuran*>::iterator p;
    for(p = breeder->anurans.begin(); p != breeder->anurans.end(); p++){
      data.conductor->addInstrument(p->second);
    }
  
    std::map<int,Cricket*>::iterator cricket_iterator;
    for(cricket_iterator = breeder->insects.begin(); cricket_iterator != breeder->insects.end(); cricket_iterator++){
      data.conductor->addInstrument(cricket_iterator->second);
    }
    */

    // 4) Reconfigure the TickData object to the appropriate values
    data.channels = channels;
    // Resize the StkFrames object appropriately.
    data.frames.resize( bufferSize, channels );




    // OPEN UP THE STREAM, SCOTTY
    // dac->openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)&sine );
    dac->openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferSize, &tick, (void *)&data );
    //dac->openStream(devicenum, channels, 0, 0, format, (int)Stk::sampleRate(), &bufferSize, 4);
    dac->startStream();
  }
  catch (RtError& error) 
  {
    error.printMessage();
    goto cleanup; // exit(0);
  }

  
 

  
  //-------------------------------------------------------------------- RtAudio stream start
 // try {
    //dac->setStreamCallback(&tick, (void *)&data); // only in RtAudio versions previous to 4.0
  //  dac->startStream();
  //}
  //catch (RtError &error) {
   // error.printMessage();
    //goto cleanup;
  //}

  // Block waiting until callback signals done.
    while ( true )
      Stk::sleep( 1000 );
  //char keyhit;
  //std::cout << "\nPlaying ... press <enter> to quit.\n";
  //std::cin.get(keyhit);

  // Shut down the callback and output stream.
  //try {
  //  dac->closeStream();
  //}
  //catch (RtError &error) {
  //  error.printMessage();
  //}

 cleanup:

  delete dac;
  delete breeder;
  delete conductor;
  delete dispatcher;
  return 0;
}
