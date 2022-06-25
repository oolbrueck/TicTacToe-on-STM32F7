/*
* @author Oliver Olbrück
*/


class TouchArea
{
	private:
		int xLowerBound;
    int xUpperBound;
    int yLowerBound;
    int yUpperBound;
	
	public:
		TouchArea();
		TouchArea(int& xLowerBound, int& xUpperBound, int& yLowerBound, int& yUpperBound);
	  TouchArea(int xLowerBound, int xUpperBound, int yLowerBound, int yUpperBound);
	  ~TouchArea();
		bool containsPoint(int& x, int& y);
};
