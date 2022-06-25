/*
* @author Oliver Olbrück
*/

#include "state.h"
#include "display_image.h"
#include "lib.h"
#include "GameConf.h"
#include "PlayControl.h"

//*******************************************************************
//includes for touch {
#include "Src/I2Cmaster_MCU.h"
#include "Peripheral/Touch_FT6206.h"
#include "Com/Device/Control/devControlPointer.h"
//                   }

// definitions for Feedback which area was touched {
#define NOTHING 0x00
#define ONE 0x01
#define TWO 0x02
#define THREE 0x03
#define FOUR 0x04
#define FIVE 0x05
#define SIX 0x06
#define SEVEN 0x07
#define EIGHT 0x08
#define NINE 0x09
#define START 0x10
#define MAN 0x11
#define PLVSCO 0x12
#define PLVSPL 0x13
#define GOBACK 0x14
#define RETTOMENUE 0x15
#define PLAGAIN 0x16
//                                                 }


  StateContext* StateContext::stateContext = new StateContext(new StartState());
	State* StateContext::nextState = new StartMenue(); //TODO global entfernen

	//- Touch -----------------------------------------------------------
  cHwI2Cmaster_N     i2c  ( cHwI2Cmaster_N::I2C_4, cHwI2Cmaster::CR_400kHz );
  cHwTouch_FT6206    touch( i2c ); //, pinINT,800, 480); 
  cDevControlPointer pointer( touch );
  //	

static void delayAfterDeadLockOrWin() 
{
	for(int i = 0; i < 1000000; ++i) { 
				int a = i * 3.14 + 134.47676; //random calculation
	}
}


static void delayAfterTouchInput() 
{
	for(int i = 0; i < 100000; ++i) { 
				int a = i * 3.14 + 134.47676; //random calculation
	}
}


int waitForTouchinput(Image& image) 
{
	int touchedAreaVar;
	delayAfterTouchInput();
	while(1) {
		cDevControlPointer::cData event = pointer.get();
		if(event.flags == 0x01) { //PRESSED  = 0x01,         //!< Pointer is pressed
			int x = event.posX;
		  int y = event.posY;
		  touchedAreaVar = image.identifyTouchedContext(x, y);
	    if(touchedAreaVar != NOTHING) { break; }
		}	
	}
	return touchedAreaVar;
}


StateContext* StateContext::getInstance()
{
	return StateContext::stateContext;
}


StateContext::StateContext(State *state)
{
	this->currentState = state;
}


StateContext::~StateContext()
{
}


State::~State() {}
	

void StateContext::getToNextState()
{
	this->currentState->changeState(this);
}


void StateContext::setState(State *state)
{
	delete this->currentState;
	this->currentState = state;
	this->currentState->init();
}


void State::changeState(StateContext* context)
{
}


void State::init()
{
}


void StartState::changeState(StateContext* context)
{
	context->setState(StateContext::nextState);
}


StartState::~StartState() {}
	

void StartState::init()
{
}


void StartMenue::changeState(StateContext* context)
{
	context->setState(StateContext::nextState);
}


StartMenue::~StartMenue() {}
	
	
void StartMenue::init()
{
	StartMenueImage startMenueImage;
	startMenueImage.print();
	
	int touchedArea = waitForTouchinput(startMenueImage);
	
	if(touchedArea == START) {
		StateContext::nextState = new SubMenue();
	} else if(touchedArea == MAN) {
		StateContext::nextState = new Manual();
	}
}


SubMenue::~SubMenue() {}
	
	
void SubMenue::init()
{
	SubMenueImage subMenueImage;
	subMenueImage.print();
	
	int touchedArea = waitForTouchinput(subMenueImage);
	
	if(touchedArea == PLVSPL) {
		GameConf::playerVsPlayerAndNotPlayerVsComputer = true;
		StateContext::nextState = new InTheGame();
	} else if(touchedArea == PLVSCO) {
		GameConf::playerVsPlayerAndNotPlayerVsComputer = false;
		StateContext::nextState = new InTheGame();
	}
}


void SubMenue::changeState(StateContext* context)
{
  context->setState(StateContext::nextState);
}


Manual::~Manual() {}
	
	
void Manual::init()
{
	ManualImage manualImage;
	manualImage.print();
	
	int touchedArea = waitForTouchinput(manualImage);
	
	if(touchedArea == GOBACK) {
		StateContext::nextState = new StartMenue();
	}
}


void Manual::changeState(StateContext* context)
{
  context->setState(StateContext::nextState);
}


InTheGame::~InTheGame() {}
	
	
static char alternateMarkSetting(short move, short field, InTheGameImage image) 
{
	if(move % 2 == 0) {
			image.drawMark('x', field);
		  return 'x';
	} else {
			image.drawMark('o', field);
		  return 'o';
	}
}


static int makeCounterMove(PlayControl pc, int totalFieldForPlayControl[3][3], InTheGameImage inTheGameImage)
{
	int n = pc.bestPlay(totalFieldForPlayControl);
	inTheGameImage.drawMark('x', n);
	return n;
}


static void updateTotalFieldForPlayControl(char totalField[9], int totalFieldForPlayControl[3][3])
{
	for(int i = 0; i < 3; ++i) {
			for(int j = 0; j < 3; ++j) {
				if(totalField[3 * i + j] == 'e') {
					totalFieldForPlayControl[i][j] = -1;
				} else if(totalField[3 * i + j] == 'o') {
					totalFieldForPlayControl[i][j] = 0;
				} else if(totalField[3 * i + j] == 'x') {
					totalFieldForPlayControl[i][j] = 1;
				}
			}
		}
}
	

void InTheGame::init()
{
	InTheGameImage inTheGameImage;
	inTheGameImage.print();
	char totalField[9] = {'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e'};
	int totalFieldForPlayControl[3][3] = {{-1, -1, -1}, {-1, -1, -1}, {-1, -1, -1}};
	short move = -1;
	PlayControl pc;
	
	while(1) {
	  
		updateTotalFieldForPlayControl(totalField, totalFieldForPlayControl);
		
		if(pc.isAMatch(totalFieldForPlayControl)) {
			inTheGameImage.drawMatch(totalFieldForPlayControl);
			delayAfterDeadLockOrWin();
			StateContext::nextState = new AfterGame();
			break;
		}
		
		if(pc.isADeadlock(totalFieldForPlayControl)) {
			inTheGameImage.drawDeadLock();
			delayAfterDeadLockOrWin();
			StateContext::nextState = new AfterGame();
			break;
		}
		
		int touchedArea = waitForTouchinput(inTheGameImage);
		
		if(touchedArea == ONE) {
			if(totalField[0] == 'e') {
				if(GameConf::playerVsPlayerAndNotPlayerVsComputer) {
					totalField[0] = alternateMarkSetting(++move, 0, inTheGameImage);
				} else {
					totalField[0] = 'o';
					inTheGameImage.drawMark('o', 0);
					updateTotalFieldForPlayControl(totalField, totalFieldForPlayControl);
					totalField[makeCounterMove(pc, totalFieldForPlayControl, inTheGameImage)] = 'x';
				}
			}	
		} else if(touchedArea == TWO) {
			if(totalField[1] == 'e') {
				if(GameConf::playerVsPlayerAndNotPlayerVsComputer) {
					totalField[1] = alternateMarkSetting(++move, 1, inTheGameImage);
				} else {
					totalField[1] = 'o';
					inTheGameImage.drawMark('o', 1);
					updateTotalFieldForPlayControl(totalField, totalFieldForPlayControl);
					totalField[makeCounterMove(pc, totalFieldForPlayControl, inTheGameImage)] = 'x';
				}
			}
		} else if(touchedArea == THREE) {
			if(totalField[2] == 'e') {
				if(GameConf::playerVsPlayerAndNotPlayerVsComputer) {
					totalField[2] = alternateMarkSetting(++move, 2, inTheGameImage);
				} else {
					totalField[2] = 'o';
					inTheGameImage.drawMark('o', 2);
					updateTotalFieldForPlayControl(totalField, totalFieldForPlayControl);
					totalField[makeCounterMove(pc, totalFieldForPlayControl, inTheGameImage)] = 'x';
				}
			}
		} else if(touchedArea == FOUR) {
			if(totalField[3] == 'e') {
				if(GameConf::playerVsPlayerAndNotPlayerVsComputer) {
					totalField[3] = alternateMarkSetting(++move, 3, inTheGameImage);
				} else {
					totalField[3] = 'o';
					inTheGameImage.drawMark('o', 3);
					updateTotalFieldForPlayControl(totalField, totalFieldForPlayControl);
					totalField[makeCounterMove(pc, totalFieldForPlayControl, inTheGameImage)] = 'x';
				}
			}
		} else if(touchedArea == FIVE) {
			if(totalField[4] == 'e') {
				if(GameConf::playerVsPlayerAndNotPlayerVsComputer) {
					totalField[4] = alternateMarkSetting(++move, 4, inTheGameImage);
				} else {
					totalField[4] = 'o';
					inTheGameImage.drawMark('o', 4);
					updateTotalFieldForPlayControl(totalField, totalFieldForPlayControl);
					totalField[makeCounterMove(pc, totalFieldForPlayControl, inTheGameImage)] = 'x';
				}
			}
		} else if(touchedArea == SIX) {
			if(totalField[5] == 'e') {
				if(GameConf::playerVsPlayerAndNotPlayerVsComputer) {
					totalField[5] = alternateMarkSetting(++move, 5, inTheGameImage);
				} else {
					totalField[5] = 'o';
					inTheGameImage.drawMark('o', 5);
					updateTotalFieldForPlayControl(totalField, totalFieldForPlayControl);
					totalField[makeCounterMove(pc, totalFieldForPlayControl, inTheGameImage)] = 'x';
				}
			}
		} else if(touchedArea == SEVEN) {
			if(totalField[6] == 'e') {
				if(GameConf::playerVsPlayerAndNotPlayerVsComputer) {
					totalField[6] = alternateMarkSetting(++move, 6, inTheGameImage);
				} else {
					totalField[6] = 'o';
					inTheGameImage.drawMark('o', 6);
					updateTotalFieldForPlayControl(totalField, totalFieldForPlayControl);
					totalField[makeCounterMove(pc, totalFieldForPlayControl, inTheGameImage)] = 'x';
				}
			}
		} else if(touchedArea == EIGHT) {
			if(totalField[7] == 'e') {
				if(GameConf::playerVsPlayerAndNotPlayerVsComputer) {
					totalField[7] = alternateMarkSetting(++move, 7, inTheGameImage);
				} else {
					totalField[7] = 'o';
					inTheGameImage.drawMark('o', 7);
					updateTotalFieldForPlayControl(totalField, totalFieldForPlayControl);
					totalField[makeCounterMove(pc, totalFieldForPlayControl, inTheGameImage)] = 'x';
				}
			}
		} else if(touchedArea == NINE) {
			if(totalField[8] == 'e') {
				if(GameConf::playerVsPlayerAndNotPlayerVsComputer) {
					totalField[8] = alternateMarkSetting(++move, 8, inTheGameImage);
				} else {
					totalField[8] = 'o';
					inTheGameImage.drawMark('o', 8);
					updateTotalFieldForPlayControl(totalField, totalFieldForPlayControl);
					totalField[makeCounterMove(pc, totalFieldForPlayControl, inTheGameImage)] = 'x';
				}
			}
		}
  }
}


void InTheGame::changeState(StateContext* context)
{
  context->setState(StateContext::nextState);
}


AfterGame::~AfterGame() {}
	
	
void AfterGame::init()
{
	AfterGameImage afterGameImage;
	afterGameImage.print();
	
	int touchedArea = waitForTouchinput(afterGameImage);
	
	if(touchedArea == RETTOMENUE) {
		StateContext::nextState = new StartMenue();
	} else if(touchedArea == PLAGAIN) {
		StateContext::nextState = new InTheGame();
	}
}


void AfterGame::changeState(StateContext* context)
{
  context->setState(StateContext::nextState);
}
