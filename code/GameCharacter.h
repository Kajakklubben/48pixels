#ifndef GAMECHARACTER_H
#define GAMECHARACTER_H

#include "ofMain.h"
#include "AnimationLoader.h"
class gameScreen;

const float GameCharacter_movespeed = 20.0;
const float GameCharacter_maxMovespeed = 220.0;
const float GameCharacter_jumpheight = 900.0;

enum CharacterAnimationType {
    S_idle=0,
    S_walk=1,
    S_jump=2,
    S_fall=3

};

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
        int drawHeight, drawWidth;
        ofVec2f gravity;
        ofVec2f velocity;
        ofVec2f position;
        bool isGrounded;

    protected:
    private:
};

#endif // GAMECHARACTER_H
