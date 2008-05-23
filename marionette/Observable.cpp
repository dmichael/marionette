
#include "Observable.h"
#include "Unit.h"
Observable::Observable()
{
}

Observable::~Observable()
{
}

void Observable::addObserver(Observable *o)
{
  mObservers.push_back(o);
	mHasObservers = true;
}

void Observable::deleteObserver(void *o)
{
  Observable *observer = (Observable *) o;
  int count = mObservers.size();
	int i;
	for (i = 0; i < count; i++){
	  if(mObservers[i] == observer){
	    break;
	  }
	}
	if (i < count) mObservers.erase(mObservers.begin() + i);
	if (mObservers.size() == 0) mHasObservers = false;
}

void Observable::changed(bool * argument)
{
  
}

void Observable::notifyObservers()
{ 
  // speed hack
  if( ! mHasObservers) return;
  // iterate over my observers
	std::vector<Observable*>::iterator pos;
	for (pos = mObservers.begin(); pos != mObservers.end(); ++pos){
    (*pos)->update();
	}
}

/*
  Overloaded method to send a message and the sender
*/
void Observable::notifyObservers( void *o )
{ 
  // speed hack
  if( ! mHasObservers) return;
  // iterate over my observers
	std::vector<Observable*>::iterator pos;
	for (pos = mObservers.begin(); pos != mObservers.end(); ++pos){
    (*pos)->update( o );
	}
}
										
// void Observable::update(void * arg, void * observable)
// {
//   std::cout << "I see, this is not getting overwritten\n";
// }