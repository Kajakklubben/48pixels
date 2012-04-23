#include "GameCharacter.h"

GameCharacter::GameCharacter()
{
    moveDir = 1;
    gravity = ofVec2f(0,200);
    width = 20;
    height = 30;
}

GameCharacter::~GameCharacter()
{
    //dtor
}

void GameCharacter::setPosition(ofVec2f pos)
{
    position = pos;
}

void GameCharacter::update(float deltatime)
{
    velocity.x = GameCharacter_movespeed*moveDir;

    velocity += gravity*deltatime;

    ofVec2f nextPos = position+velocity*deltatime;

    if(nextPos.y>ofGetHeight()-height/2)
    {
        velocity.y = 0;
        position.y =ofGetHeight()-height/2;
    }


    if(nextPos.y<0+10)
        velocity.y = 0;

    if(nextPos.x>ofGetWidth()-width/2)
    {
        velocity.x = 0;
        position.x =ofGetWidth()-width/2;
    }
    if(nextPos.x<0+width/2)
    {
        velocity.x = 0;
        position.x = width/2;
    }

    position+=velocity*deltatime;
}

void GameCharacter::draw()
{
    ofRect(position.x-width/2,position.y-height/2,width,height);
}


void GameCharacter::move(int dir)
{
    moveDir = dir;

}

void GameCharacter::jump()
{

}
