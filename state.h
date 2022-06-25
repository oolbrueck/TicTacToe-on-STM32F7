/*
* @author Oliver Olbrück
*/


class State;
class StartMenue;
class Image;

class StateContext
{
	State *currentState;
	static StateContext *stateContext;
	public:
		static State *nextState; //TODO make private and provide getter func
	  ~StateContext();
	  static StateContext* getInstance();
		void getToNextState();
		void setState(State *state);
	private:
		StateContext(State *state);
};


class State
{
	public:
		virtual void changeState(StateContext* context) = 0;
	  virtual ~State();
	  virtual void init();
};


class StartState : public State
{
	virtual ~StartState();
	virtual void changeState(StateContext* context);
	virtual void init();
};


class StartMenue : public State
{
	virtual ~StartMenue();
	virtual void changeState(StateContext* context);
	virtual void init();
};


class SubMenue : public State
{
	virtual ~SubMenue();
	virtual void changeState(StateContext* context);
	virtual void init();
};

class Manual : public State
{
	virtual ~Manual();
	virtual void changeState(StateContext* context);
	virtual void init();
};


class InTheGame : public State
{
	virtual ~InTheGame();
	virtual void changeState(StateContext* context);
	virtual void init();
};


class AfterGame : public State
{
	virtual ~AfterGame();
	virtual void changeState(StateContext* context);
	virtual void init();
};
