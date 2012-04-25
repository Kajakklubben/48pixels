#ifndef GAMECHARACTER_H
#define GAMECHARACTER_H

#include "ofMain.h"
#include "AnimationLoader.h"
class gameScreen;

const float GameCharacter_movespeed = 20.0;
const float GameCharacter_maxMovespeed = 220.0;
const float GameCharacter_jumpheight = 850.0;
const float GameCharacter_maxVelocity = 850.0;
const float GameCharacter_waterjumpheight = 500.0;
enum CharacterAnimationType {
    S_idle=0,
    S_walk=1,
    S_jump=2,
    S_fall=3,
    S_swim=3,
    S_trapped=3

};


enum CharacterState {
    C_Air=0,
    C_Trapped=1,
    C_Grounded=2,
    C_Swimming=3,

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
        void down();
        bool moveDown;
        int moveDir;
        int lastDiretion;

        gameScreen* game;
        int height,width;
        int drawHeight, drawWidth;
        ofVec2f gravity;
        ofVec2f velocity;
        ofVec2f position;
        CharacterState state;

    protected:
    private:
};

#endif // GAMECHARACTER_H
