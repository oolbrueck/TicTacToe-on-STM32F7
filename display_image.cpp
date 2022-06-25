/*
* @author Oliver Olbrück
*/

#include "display_image.h"
#include "lib.h"

//includes for Display {
#include "Src/FMC.h"
#include "Src/DSI.h"
#include "Src/Port_MCU.h"
#include "Com/Hardware/Port/Port.h"
#include "Peripheral/Disp_OTM8009A.h"
#include "Com/Device/Display/devDisplayGraphic.h"
#include "Com/Hardware/Display/Font/Font_8x12.h"
//                     }

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


//needed for Display
cHwPinConfig::MAP cHwPinConfig::table[] =
{
  // ADC
  ADC3_IN8_PF_10,
  ADC3_IN12_PC_2,

  // UART
  USART1_RX_PA_10,
  USART1_TX_PA_9,

  UART5_RX_PD_2,
  UART5_TX_PC_12,

  USART6_RX_PC_7,
  USART6_TX_PC_6,

  // I2C
  I2C4_SCL_PD_12,
  I2C4_SDA_PB_7,

  //FMC
  FMC_D0_PD_14,  
  FMC_D1_PD_15,  
  FMC_D2_PD_0, 
  FMC_D3_PD_1,
  FMC_D4_PE_7,
  FMC_D5_PE_8, 
  FMC_D6_PE_9, 
  FMC_D7_PE_10, 
  FMC_D8_PE_11, 
  FMC_D9_PE_12, 
  FMC_D10_PE_13, 
  FMC_D11_PE_14, 
  FMC_D12_PE_15,
  FMC_D13_PD_8,  
  FMC_D14_PD_9,
  FMC_D15_PD_10,  
  FMC_D16_PH_8,
  FMC_D17_PH_9,
  FMC_D18_PH_10,
  FMC_D19_PH_11,
  FMC_D20_PH_12,
  FMC_D21_PH_13,
  FMC_D22_PH_14,
  FMC_D23_PH_15,
  FMC_D24_PI_0,
  FMC_D25_PI_1,
  FMC_D26_PI_2,
  FMC_D27_PI_3,
  FMC_D28_PI_6,
  FMC_D29_PI_7,
  FMC_D30_PI_9,
  FMC_D31_PI_10,

  FMC_A0_PF_0,
  FMC_A1_PF_1,
  FMC_A2_PF_2,
  FMC_A3_PF_3,
  FMC_A4_PF_4,
  FMC_A5_PF_5,
  FMC_A6_PF_12,
  FMC_A7_PF_13,
  FMC_A8_PF_14,
  FMC_A9_PF_15,
  FMC_A10_PG_0,
  FMC_A11_PG_1,
  FMC_A12_PG_2,
  FMC_A13_PG_3,
  FMC_A14_PG_4,
  FMC_A15_PG_5,

  FMC_SDCLK_PG_8,
  FMC_SDNCAS_PG_15,
  FMC_SDCKE0_PH_2,
  FMC_SDNE0_PH_3,
  FMC_SDNWE_PH_5,
  FMC_SDNRAS_PF_11,
  FMC_NBL0_PE_0,
  FMC_NBL1_PE_1, 
  FMC_NBL2_PI_4,
  FMC_NBL3_PI_5,


  END_OF_TABLE
};

//- Display ---------------------------------------------------------
cHwPort_N  portJ( cHwPort_N::PJ );
cHwFMC fmc(cHwFMC::SDRAM_Bank1);  
cHwDSI hwDSI( fmc.startAddr() );
cHwPort::Pin   lcdResetPin( portJ, 15 );
                         
cHwDisp_OTM8009Aram dispHw( hwDSI,lcdResetPin, 
                            cHwDisp_OTM8009A::LANDSCAPE_90, 
                            fontFont_8x12, 2 );

cDevDisplayGraphic disp( dispHw );
//--------------------------------------------------------------------


StartMenueImage::StartMenueImage()
: touchAreaStartButton(40, 286, 220, 336), touchAreaManualButton(40, 386, 220, 436)
{	
}


void StartMenueImage::print()
{
	#ifdef USE_GRAPHIC_DISPLAY
    disp.setBackColor(RGB2COLOR(156, 26, 26));
	  disp.refresh();
		disp.clear();
	  disp.drawText( 280,100, 22, "TIC TAC TOE");
	  disp.drawText( 50,300, 22, "Start Game");
	  disp.drawText( 50,400, 22, "Game rules");
	  disp.drawFrame(40,286,180,50,3, cHwDisplayGraphic::White );
	  disp.drawFrame(40,386,180,50,3, cHwDisplayGraphic::White );
	#endif
		disp.refresh();
}


int StartMenueImage::identifyTouchedContext(int& x, int& y)
{
	if(this->touchAreaStartButton.containsPoint(x, y)) {
		return START;
	}
	if(this->touchAreaManualButton.containsPoint(x, y)) {
		return MAN;
	}
	return NOTHING;
}


SubMenueImage::SubMenueImage()
: touchAreaPlVsPlButton(400, 800, 10, 480), touchAreaPlVsCoButton(10, 400, 10, 480)
{
}


void SubMenueImage::print()
{
	#ifdef USE_GRAPHIC_DISPLAY
		disp.setBackColor(RGB2COLOR(156, 26, 26));
		disp.clear();
	  disp.drawText( 50,200, 30, "Player vs Computer         Player vs Player");
	  disp.drawLine(400, 25, 400, 455 ,3 , cHwDisplayGraphic::White);
	  disp.refresh();
	#endif	
}


int SubMenueImage::identifyTouchedContext(int& x, int& y)
{
	if(this->touchAreaPlVsCoButton.containsPoint(x, y)) {
		return PLVSCO;
	}
	if(this->touchAreaPlVsPlButton.containsPoint(x, y)) {
		return PLVSPL;
	}
	return NOTHING;
}


ManualImage::ManualImage()
: goBackButton(400, 800, 0, 480)
{
}


void ManualImage::print()
{
	#ifdef USE_GRAPHIC_DISPLAY
		disp.setBackColor(RGB2COLOR(156, 26, 26));
		disp.clear();
	  disp.drawText( 30,100, 30, "The player who");
	  disp.drawText( 30,140, 30, "succeeds in placing");
	  disp.drawText( 30,180, 30, "three of their marks");
	  disp.drawText( 30,260, 30, "in a horizontal,");
	  disp.drawText( 30,300, 35, "vertical, or diagonal");
	  disp.drawText( 30,340, 30, "row is the winner.");
	  disp.drawText( 540,225, 30, "go Back");
	  disp.drawLine(400, 25, 400, 455 ,3 , cHwDisplayGraphic::White);
	  disp.refresh();
	#endif
}


int ManualImage::identifyTouchedContext(int& x, int& y)
{
	if(this->goBackButton.containsPoint(x, y)) {
		return GOBACK;
	}

	return NOTHING;
}


InTheGameImage::InTheGameImage()
: touchAreaFieldOne(185,328,25,168),
  touchAreaFieldTwo(328,471,25,168),
  touchAreaFieldThree(471,615,25,168),
  touchAreaFieldFour(185,328,168,311),
  touchAreaFieldFive(328,471,168,311),
  touchAreaFieldSix(471,615,168,311),
  touchAreaFieldSeven(185,328,311,455),
  touchAreaFieldEight(328,471,311,455),
  touchAreaFieldNine(471,615,311,455)
{
	
}


void InTheGameImage::drawMark(char mark, int field) 
{
	int centerOfFields[9][2] = {{256, 96},  {400, 96},  {543, 96}, //each center consists of x and y coordinate
                              {256, 239}, {400, 239}, {543, 239},
                   	          {256, 383}, {400, 383}, {543, 383}};
	if(mark == 'o') { //draws O
		#ifdef USE_GRAPHIC_DISPLAY
			disp.drawCircle(centerOfFields[field][0], centerOfFields[field][1], 40, cHwDisplayGraphic::White);
		  disp.drawCircle(centerOfFields[field][0], centerOfFields[field][1], 35, RGB2COLOR(156, 26, 26));
		  disp.refresh();
		#endif
	} else { //draws X
		#ifdef USE_GRAPHIC_DISPLAY
		  disp.drawLine(centerOfFields[field][0] + 40, centerOfFields[field][1] - 40,
		                centerOfFields[field][0] - 40, centerOfFields[field][1] + 40,
		                3, cHwDisplayGraphic::White);
		  disp.drawLine(centerOfFields[field][0] + 40, centerOfFields[field][1] + 40,
		                centerOfFields[field][0] - 40, centerOfFields[field][1] - 40,
		                3, cHwDisplayGraphic::White);
		  disp.refresh();
		#endif
	}
	for(int i = 0; i < 50000; ++i) { 
				int a = i * 3.14 + 134.47676; //random calculation to get delay needed to draw completely on display
	}
}


void InTheGameImage::drawDeadLock() //draws a giant X
{
	#ifdef USE_GRAPHIC_DISPLAY
		disp.drawLine(185, 455, 615, 25, 15, cHwDisplayGraphic::Black); 
	  disp.drawLine(185, 25, 615, 455, 15 , cHwDisplayGraphic::Black);
	  disp.refresh();
	#endif
}


void InTheGameImage::drawMatch(int field[3][3]) 
{
	int rowColumnDiagonalToMark = 0;
	for(int i = 0; i < 3; ++i) { //horizontal line
        if((field[i][0] != -1) && (field[i][0] == field[i][1]) && (field[i][1] == field[i][2])) { 
            rowColumnDiagonalToMark = i + 1;
						disp.refresh();
        }
  }
  for(int i = 0; i < 3; ++i) { //vertical line
        if((field[0][i] != -1) && (field[0][i] == field[1][i]) && (field[1][i] == field[2][i])) {
            rowColumnDiagonalToMark = (i + 1) * -1;
        }
  }
	if(((field[0][2] != -1) && (field[0][2] == field[1][1]) && (field[1][1] == field[2][0]))) { //Rising diagonal line
        rowColumnDiagonalToMark = 4;
  }
	if(((field[0][0] != -1) && (field[0][0] == field[1][1]) && (field[1][1] == field[2][2]))) { //Falling diagonal line
        rowColumnDiagonalToMark = 5;
  }
	
	#ifdef USE_GRAPHIC_DISPLAY
	  switch(rowColumnDiagonalToMark) { //negative numbers are vertical line, positive number horizontal lines and 4, 5 diagonal lines
			case (-1): //vertical line first column
				disp.drawLine(256, 25, 256, 455, 10 , cHwDisplayGraphic::Black);
			  break;
			case (-2): //vertical line second column
				disp.drawLine(400, 25, 400, 455, 10 , cHwDisplayGraphic::Black);
			  break;
			case (-3): //vertical line third column
				disp.drawLine(543, 25, 543, 455, 10 , cHwDisplayGraphic::Black);
			  break;
			case 1: //horizontal line first line
				disp.drawLine(185, 96, 615, 96, 10 , cHwDisplayGraphic::Black);
			  break;
			case 2: //horizontal line second line
				disp.drawLine(185, 240, 615, 240, 10 , cHwDisplayGraphic::Black);
			  break;
			case 3: //horizontal line third line
				disp.drawLine(185, 383, 615, 383, 10 , cHwDisplayGraphic::Black);
			  break;
			case 4: //Rising diagonal line
				disp.drawLine(185, 455, 615, 25, 10 , cHwDisplayGraphic::Black);
			  break;
			case 5: //Falling diagonal line
				disp.drawLine(615, 455, 185, 25, 10 , cHwDisplayGraphic::Black);
			  break;
		}
	  disp.refresh();
	#endif
}


void InTheGameImage::print()
{
	#ifdef USE_GRAPHIC_DISPLAY
		disp.setBackColor(RGB2COLOR(156, 26, 26));
		disp.clear();
	  disp.drawFrame(185,25,430,430,3, cHwDisplayGraphic::White );
	  disp.drawLine(328, 25, 328, 455 ,3 , cHwDisplayGraphic::White);
	  disp.drawLine(471, 25, 471, 455 ,3 , cHwDisplayGraphic::White);
	  disp.drawLine(185, 168, 615, 168 ,3 , cHwDisplayGraphic::White);
	  disp.drawLine(185, 311, 615, 311 ,3 , cHwDisplayGraphic::White);
	  disp.refresh();
	#endif
}

int InTheGameImage::identifyTouchedContext(int& x, int& y)
{
	if(this->touchAreaFieldOne.containsPoint(x, y)) {
		return ONE;
	}
	if(this->touchAreaFieldTwo.containsPoint(x, y)) {
		return TWO;
	}
	if(this->touchAreaFieldThree.containsPoint(x, y)) {
		return THREE;
	}
	if(this->touchAreaFieldFour.containsPoint(x, y)) {
		return FOUR;
	}
	if(this->touchAreaFieldFive.containsPoint(x, y)) {
		return FIVE;
	}
	if(this->touchAreaFieldSix.containsPoint(x, y)) {
		return SIX;
	}
	if(this->touchAreaFieldSeven.containsPoint(x, y)) {
		return SEVEN;
	}
	if(this->touchAreaFieldEight.containsPoint(x, y)) {
		return EIGHT;
	}
	if(this->touchAreaFieldNine.containsPoint(x, y)) {
		return NINE;
	}
	return NOTHING;
}


AfterGameImage::AfterGameImage()
: touchAreaPlayAgainButton(10, 400, 10, 480), touchAreaReturnToMenueButton(400, 800, 10, 480)
{

}


void AfterGameImage::print()
{
	#ifdef USE_GRAPHIC_DISPLAY
		disp.setBackColor(RGB2COLOR(156, 26, 26));
		disp.clear();
	  disp.drawText( 50,200, 30, "     play again            return to menue");
	  disp.drawLine(400, 25, 400, 455 ,3 , cHwDisplayGraphic::White);
	  disp.refresh();
	#endif	
}


int AfterGameImage::identifyTouchedContext(int& x, int& y)
{
	if(this->touchAreaPlayAgainButton.containsPoint(x, y)) {
		return PLAGAIN;
	}
	if(this->touchAreaReturnToMenueButton.containsPoint(x, y)) {
		return RETTOMENUE;
	}
	return NOTHING;
}
