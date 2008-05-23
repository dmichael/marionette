/***************************************************/
/*! \class Bus
    \brief STK basic mixer class.

    This class keeps a vector list of inputs and sums them to compute the output frames

    by Perry R. Cook and Gary P. Scavone, 1995 - 2007.
*/
/***************************************************/

#ifndef STK_SOUNDFILEUGEN_H
#define STK_SOUNDFILEUGEN_H

#include "Unit.h"
#include "FileWvIn.h"
#include <list>

class SoundFile : public Unit
{
public:
  //! Default constructor.
  SoundFile( void );

  //! Class destructor.
  virtual ~SoundFile( void );
  
  char * name( void ){ return "SoundFile"; }
  
  void update( void *arg );

  //! Open the specified file and load its data.
  /*!
    Data from a previously opened file will be overwritten by this
    function.  An StkError will be thrown if the file is not found,
    its format is unknown, or a read error occurs.  If the file data
    is to be loaded incrementally from disk and normalization is
    specified, a scaling will be applied with respect to fixed-point
    limits.  If the data format is floating-point, no scaling is
    performed.
  */
  void open( std::string fileName, bool raw = false, bool doNormalize = true );

  //! Close a file if one is open.
  void close( void );

  //! Clear outputs and reset time (file) pointer to zero.
  void reset( void );

  //! Normalize data to a maximum of +-1.0.
  /*!
    This function has no effect when data is incrementally loaded
    from disk.
  */
  void normalize( void );
  
  //! Set the data read rate in samples.  The rate can be negative.
  /*!
    If the rate value is negative, the data is read in reverse order.
  */
  void setRate( float rate );

  unsigned long readFile( void );

  unsigned long durationInSamples( void );
  
  StkFloat durationInSeconds( void );
    
  int add(Unit*);
  
  int remove(Unit*);
  
  void clearInputs();

	std::list<Unit*> frequency_mods;
	std::list<Unit*> phase_mods;
	std::list<Unit*> writers;
		  
  FileWvIn *source;
  
  bool is_finished( void );
  
protected:
  std::string filename_;
  
  StkFloat computeSample( void );
  void computeFrame( void );
};

#endif