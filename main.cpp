/*
* @author Oliver Olbrück
*/

#include "state.h"
#include "PlayControl.h"
//#include <stdio.h>

#define PLAYER_ONE 0
#define PLAYER_TWO 1
#define EMPTYFIELD -1

static void test() 
{
	PlayControl pc;
	
	  int caseOne[3][3] = {{PLAYER_ONE, PLAYER_TWO, EMPTYFIELD},
                           {EMPTYFIELD, PLAYER_TWO, EMPTYFIELD},
                           {PLAYER_TWO, PLAYER_ONE, PLAYER_ONE}};

    int caseTwo[3][3] = {{PLAYER_ONE, PLAYER_TWO, PLAYER_ONE},
                           {PLAYER_ONE, PLAYER_TWO, PLAYER_TWO},
                           {PLAYER_ONE, PLAYER_ONE, PLAYER_ONE}};

    int caseThree[3][3] = {{PLAYER_ONE, PLAYER_ONE, PLAYER_TWO},
                           {PLAYER_TWO, PLAYER_ONE, EMPTYFIELD},
                           {PLAYER_ONE, PLAYER_TWO, PLAYER_ONE}};
		
		int caseFour[3][3] = {{PLAYER_ONE, PLAYER_ONE, PLAYER_TWO},
                           {PLAYER_TWO, PLAYER_ONE, PLAYER_ONE},
                           {PLAYER_ONE, PLAYER_TWO, PLAYER_TWO}};
		
		if(pc.isAMatch(caseOne) == false) {
			//printf("test 1 completed successfully");
		} else {
			//printf("test 1 failed!");
		}
		if(pc.isAMatch(caseTwo) == true) {
			//printf("test 1 completed successfully");
		} else {
			//printf("test 1 failed!");
		}
		if(pc.isAMatch(caseThree) == true) {
			//printf("test 1 completed successfully");
		} else {
			//printf("test 1 failed!");
		}
		if(pc.isADeadlock(caseFour) == true) {
			//printf("test 1 completed successfully");
		} else {
			//printf("test 1 failed!");
		}
		//printing on console (Serial Print) need to be enabled in IDE Settings
		//https://community.silabs.com/s/article/how-to-display-printf-output-over-swo-in-keil-vision?language=en_US
		
		//#include <stdio.h> and its content is uncommented due to code-size-limitations of IDE license Version
		//.\Bin\Keil\STM32F7xx\main.axf: error: L6047U: The size of this image (35076 bytes) exceeds 
		// the maximum allowed for this version of the linker
		// -> https://developer.arm.com/documentation/ka002709/latest (Answer 1)
}

int main(void) {
	test();
  StateContext *sc = StateContext::getInstance();

  while(1)
	{
			sc->getToNextState();
  }		
}

