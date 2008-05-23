
#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <vector>
#include <iostream>

class Observable
{
 public:
  //! Class constructor.
  Observable( void );

  //! Class destructor.
  virtual ~Observable( void );

  ///< register/remove observers
  void addObserver(Observable *);		
  void deleteObserver(void *);
  
  ///< this is what I send to myself to notify my observers;
  /// it's not overridden in general.
  void changed(bool * argument);		
  
  void notifyObservers(void);		///< this is what I send to myself to notify my observers;
  void notifyObservers(void *);	///< this is what I send to myself to notify my observers;
    
  virtual char * name( void ){ return "Observer"; };								
  
  virtual void update( void ){printf("overwrite this!");};		///< this will be implemented in subclasses.
  virtual void update( void *){printf("overwrite this!");};		///< this will be implemented in subclasses.
   
  std::vector<Observable*> mObservers;		///< the private vector of observers
  bool mHasObservers;					///< whether there are any observers (for fast checking)
};

#endif

