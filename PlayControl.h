/*
* @author Oliver Olbrück
*/

class PlayControl {

    int heightInTree;
    int field;
    public:             // used to judge if min or max shall be returned

        bool isAMatch(int field[3][3]);
        bool isADeadlock(int field[3][3]);
        int bestPlay(int field[3][3]);

        PlayControl()
        {
            this->heightInTree = -1;
        }

     private:

        int calculateNextPlay(int field[3][3]);
        void incrementHeight();
        void decrementHeight();
};
