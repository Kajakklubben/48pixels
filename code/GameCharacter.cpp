#include "GameCharacter.h"
#include "gameScreen.h"

GameCharacter::GameCharacter()
{
    moveDir = 0;
    gravity = ofVec2f(0,1200);
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

    isGrounded = false;
    velocity.x += GameCharacter_movespeed*moveDir;
    velocity.x = CLAMP(velocity.x,-GameCharacter_maxMovespeed,GameCharacter_maxMovespeed);
    if(moveDir==0)
    {
        if( fabs(velocity.x)<GameCharacter_movespeed)
            velocity.x *=0.7;
        else
            velocity.x += velocity.x<0?GameCharacter_movespeed:-GameCharacter_movespeed;
    }

    velocity += gravity*deltatime;

    ofVec2f nextPos = position+velocity*deltatime;

    GameBlock nextBlockLeft = game->GetBlock(nextPos.x-width/2,nextPos.y+height/2+5);
    GameBlock nextBlockRight = game->GetBlock(nextPos.x+width/2,nextPos.y+height/2+5);

    bool hitRight = (nextBlockRight.solid && position.y+height/2>nextBlockRight.y-5);
    bool hitLeft = (nextBlockLeft.solid && position.y+height/2> nextBlockLeft.y-5);

    if(velocity.y>0 && (hitLeft || hitRight))
    {
        velocity.y = 0;
        isGrounded = true;

        position.y = (hitLeft?nextBlockLeft.y:nextBlockRight.y)-height/2;

    }


    if(nextPos.y<0+width/2)
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


    if(isGrounded)
    {
        velocity.y = -GameCharacter_jumpheight;
    }

}


