/*
* @author Oliver Olbrück
*/

#include "PlayControl.h"

#define isEqual(n) (n%2 == 0)
#define MIN -10
#define MAX 10
#define REQUESTINGPLAYER 0
#define OPPONENTPLAYER 1
#define EMPTYFIELD -1


void PlayControl::incrementHeight()
{
    ++this->heightInTree;
}


void PlayControl::decrementHeight()
{
    --this->heightInTree;
}


int PlayControl::bestPlay(int field[3][3])
{
    if(this->isAMatch(field) || this->isADeadlock(field)) { return -1; }
    this->calculateNextPlay(field);
    return this->field; //3 * x + y; //Umkehrfunktion x = num /3  y = num % 3
}


bool PlayControl::isAMatch(int field[3][3])
{
    for(int i = 0; i < 3; ++i) {
        if((field[i][0] != -1) && (field[i][0] == field[i][1]) && (field[i][1] == field[i][2])) {
            return true;
        }
    }
    for(int i = 0; i < 3; ++i) {
        if((field[0][i] != -1) && (field[0][i] == field[1][i]) && (field[1][i] == field[2][i])) {
            return true;
        }
    }
    if(((field[0][0] != -1) && (field[0][0] == field[1][1]) && (field[1][1] == field[2][2])) || ((field[0][2] != -1) && (field[0][2] == field[1][1]) && (field[1][1] == field[2][0]))) {
        return true;
    }

    return false;
};


bool PlayControl::isADeadlock(int field[3][3])
{
    if(this->isAMatch(field)) {
        return false;
    }
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            if(field[i][j] == -1) { return false; }
        }
    }
    return true;
};


int PlayControl::calculateNextPlay(int field[3][3]) //is used to decide if maximum or minimum shall be returned
{
    this->incrementHeight();

    if(isADeadlock(field)) {
        this->decrementHeight();
        return 0;
    }

    if(this->isAMatch(field)) {
        this->decrementHeight();
        return isEqual((this->heightInTree)) ? MAX : MIN;
    }

    int x = -1;
    int y = -1;
    int maxi = -10;
    int mini = 10;

    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            if(field[i][j] == EMPTYFIELD) {
                field[i][j] = isEqual(this->heightInTree)? REQUESTINGPLAYER : OPPONENTPLAYER;
                int tmp = calculateNextPlay(field);
                if(!isEqual(this->heightInTree)) { //collect the min
                    if(tmp <= mini) { x = i; y = j; mini = tmp;}
                } else { //collect the max
                    if(tmp >= maxi) { x = i; y = j; maxi = tmp;}
                }
                field[i][j] = EMPTYFIELD; //reset to old value
            }
        }
    }
    this->decrementHeight();
    this->field = 3 * x + y;
    return !isEqual(((this->heightInTree) + 1)) ? mini : maxi;
};

