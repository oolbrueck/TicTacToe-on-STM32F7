/*
* @author Oliver Olbrück
*/

#include "touch_area.h"


class Image
{
	public:
		virtual void print() = 0;
	  virtual int identifyTouchedContext(int& x, int& y) = 0;
};


class StartMenueImage : public Image
{
  private:
	  TouchArea touchAreaStartButton;
	  TouchArea touchAreaManualButton;
	public:
		StartMenueImage();
		virtual void print();
	  virtual int identifyTouchedContext(int& x, int& y);
};


class SubMenueImage : public Image
{
	private:
	  TouchArea touchAreaPlVsPlButton;
	  TouchArea touchAreaPlVsCoButton;
  public:
		SubMenueImage();
		virtual void print();
	  virtual int identifyTouchedContext(int& x, int& y);
};


class ManualImage : public Image
{
	private:
	  TouchArea goBackButton;
  public:
		ManualImage();
		virtual void print();
	  virtual int identifyTouchedContext(int& x, int& y);
};


class InTheGameImage : public Image
{
	private:
	  TouchArea touchAreaFieldOne;
	  TouchArea touchAreaFieldTwo;
	  TouchArea touchAreaFieldThree;
	  TouchArea touchAreaFieldFour;
	  TouchArea touchAreaFieldFive;
	  TouchArea touchAreaFieldSix;
	  TouchArea touchAreaFieldSeven;
	  TouchArea touchAreaFieldEight;
	  TouchArea touchAreaFieldNine;
	public:
		InTheGameImage();
		virtual void print();
	  virtual int identifyTouchedContext(int& x, int& y);
	  void drawMark(char mark, int field);
	  void drawDeadLock();
	  void drawMatch(int field[3][3]);
};


class AfterGameImage : public Image
{
	private:
	  TouchArea touchAreaPlayAgainButton;
	  TouchArea touchAreaReturnToMenueButton;
	public:
		AfterGameImage();
		virtual void print();
	  virtual int identifyTouchedContext(int& x, int& y);
};
