#ifndef GAMECHARACTER_H
#define GAMECHARACTER_H

#include "ofMain.h"

const float GameCharacter_movespeed = 150.0;
const float GameCharacter_jumpheight = 1000.0;
class GameCharacter
{
    public:
        GameCharacter();
        virtual ~GameCharacter();

        void setPosition(ofVec2f pos);
        void update(float delta);
        void draw();

        void move(int dir);
        void jump();
        int moveDir;

        int height,width;
        ofVec2f gravity;
        ofVec2f velocity;
        ofVec2f position;

    protected:
    private:
};

#endif // GAMECHARACTER_H
