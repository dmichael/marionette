// crtsine.cpp STK tutorial program
#include <iostream>
#include "SineWave.h"
#include "RtAudio.h"

using namespace std;
// This tick() function handles sample computation only.  It will be
// called automatically when the system needs a new buffer of audio
// samples.
int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
  SineWave *sine = (SineWave *) dataPointer;
  register StkFloat *samples = (StkFloat *) outputBuffer;

  for ( unsigned int i=0; i<nBufferFrames; i++ )
    *samples++ = sine->tick();

  return 0;
}

int main()
{
  // Set the global sample rate before creating class instances.
  Stk::setSampleRate( 44100.0 );

  SineWave sine;
  RtAudio dac;
 

  // Figure out how many bytes in an StkFloat and setup the RtAudio stream.
  RtAudio::StreamParameters parameters;
  //parameters.deviceId = 2;
  parameters.nChannels = 1;
  RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
  unsigned int bufferFrames = RT_BUFFER_SIZE;
  try {

    cout << "Default output device " << dac.getDefaultOutputDevice() << endl;
    cout << "Compiled APIs" << dac.getDeviceInfo(0).outputChannels << endl;

dac.openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)&sine );
  }
  catch ( RtError &error ) {
    error.printMessage();
    goto cleanup;
  }

  sine.setFrequency(440.0);

  try {
    dac.startStream();
  }
  catch ( RtError &error ) {
    error.printMessage();
    goto cleanup;
  }

  // Block waiting here.
  char keyhit;
  std::cout << "\nPlaying ... press <enter> to quit.\n";
  std::cin.get( keyhit );

  // Shut down the output stream.
  try {
    dac.closeStream();
  }
  catch ( RtError &error ) {
    error.printMessage();
  }

 cleanup:

  return 0;
}
