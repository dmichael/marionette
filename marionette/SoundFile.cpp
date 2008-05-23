#include <list>
#include "SoundFile.h"
#include "Rand.h"

SoundFile :: SoundFile()
{
  source = new FileWvIn;
  setChannels(source->getChannels());
}

SoundFile :: ~SoundFile()
{
  delete source;
}

void SoundFile :: update( void *arg )
{
  // set the soundfile to start from the beginning
  source->reset();
}

void SoundFile :: open( std::string filename, bool raw, bool doNormalize )
{
  filename_ = filename; // store the filename for informational purposes
  if(filename_.size() < 1){
    printf("Please give a filename.");
  }
  else{
    source->openFile( filename, raw, doNormalize );
    setChannels(source->getChannels());
    source->addTime(100000000000000.0); // this is a sloppy way of making sure that the file does not start by itself
  }
}

void SoundFile :: close()
{ 
  source->closeFile(); 
}

void SoundFile :: reset()
{ 
  source->reset(); 
}

void SoundFile :: normalize()
{ 
  source->normalize(); 
}

void SoundFile :: setRate(float new_rate)
{
  StkFloat rate = source->getFileRate() / Stk::sampleRate();
  rate *= new_rate;
  source->setRate(rate);
}

/*This funciton is necessary because the FileWvIn source only returns the chunksize if the file is read as chunks*/
unsigned long SoundFile :: readFile()
{
  FileRead file;
  file.open(filename_);
  return file.fileSize();
}

unsigned long SoundFile :: durationInSamples()
{ 
  return readFile(); 
}

StkFloat SoundFile :: durationInSeconds()
{ 
  return (readFile()/Stk::sampleRate()); 
}

int SoundFile :: add(Unit *ugen)
{	
  writers.push_back(ugen);	
	// expand number of channels in bus, if necessary:
	// return added process
	return 1;
}

int SoundFile :: remove(Unit *ugen)
{	
	// return removed process
	return 1;
}

void SoundFile :: clearInputs(){}

StkFloat SoundFile :: computeSample()
{  
 lastOutput_ = source->tick();
 return lastOutput_;
}

bool SoundFile :: is_finished()
{
  return source->isFinished();
}

void SoundFile :: computeFrame( void )
{
  source->tickFrame( lastOutputs_ );
  //source->computeFrame();
}

