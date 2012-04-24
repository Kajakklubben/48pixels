#ifndef GAMECHARACTER_H
#define GAMECHARACTER_H

#include "ofMain.h"

class gameScreen;

const float GameCharacter_movespeed = 20.0;
const float GameCharacter_maxMovespeed = 200.0;
const float GameCharacter_jumpheight = 800.0;
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

        gameScreen* game;
        int height,width;
        ofVec2f gravity;
        ofVec2f velocity;
        ofVec2f position;
        bool isGrounded;

    protected:
    private:
};

#endif // GAMECHARACTER_H
