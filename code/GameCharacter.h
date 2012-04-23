#ifndef GAMECHARACTER_H
#define GAMECHARACTER_H

#include "ofMain.h"

class GameCharacter
{
    public:
        GameCharacter();
        virtual ~GameCharacter();

        ofVec2f gravity;
        ofVec2f velocity;
    protected:
    private:
};

#endif // GAMECHARACTER_H
