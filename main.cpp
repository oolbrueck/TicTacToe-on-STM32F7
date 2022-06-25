/*
* @author Oliver Olbrück
*/

#include "state.h"


int main(void) {
  StateContext *sc = StateContext::getInstance();

  while(1)
	{
			sc->getToNextState();
  }		
}

