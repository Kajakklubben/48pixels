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
            velocity.x *=0.7; //Hardcoded smooth stop. Fine for installation that only runs on one computer.
        else
            velocity.x += velocity.x<0?GameCharacter_movespeed:-GameCharacter_movespeed;
    }

    velocity += gravity*deltatime;

    float CollisionThreshold = 5;
    ofVec2f nextPos = position+velocity*deltatime;

    GameBlock* nextBlockBL = game->GetBlock(nextPos.x-width/2,nextPos.y+height/2+CollisionThreshold);
    GameBlock* nextBlockBR = game->GetBlock(nextPos.x+width/2,nextPos.y+height/2+CollisionThreshold);

    bool hitBR = (nextBlockBR->solid && position.y+height/2>nextBlockBR->y-CollisionThreshold);
    bool hitBL = (nextBlockBL->solid && position.y+height/2> nextBlockBL->y-CollisionThreshold);

    if(velocity.y>0 && (hitBL || hitBR))
    {
        velocity.y = 0;
        isGrounded = true;

        position.y = (hitBL?nextBlockBL->y:nextBlockBR->y)-height/2;

    }

    GameBlock* nextBlockTL = game->GetBlock(nextPos.x-width/2,nextPos.y-height/2-CollisionThreshold);
    GameBlock* nextBlockTR = game->GetBlock(nextPos.x+width/2,nextPos.y-height/2-CollisionThreshold);

    bool hitTR = (nextBlockTR->solid && position.y-height/2> nextBlockTR->y+CollisionThreshold);
    bool hitTL = (nextBlockTL->solid && position.y-height/2> nextBlockTL->y+CollisionThreshold);

    if(velocity.y<0 && (hitTR || hitTL))
    {
        velocity.y = 0;

        position.y = (hitTL?nextBlockTL->y:nextBlockTR->y)+height/2+nextBlockTR->h;

    }


    GameBlock* nextBlockLT = game->GetBlock(nextPos.x-width/2-CollisionThreshold,nextPos.y);
    GameBlock* nextBlockLB = game->GetBlock(nextPos.x-width/2-CollisionThreshold,nextPos.y);


    bool hitLT = (nextBlockLT->solid && position.x-width/2< nextBlockLT->x+nextBlockLT->w+CollisionThreshold);
    bool hitLB = (nextBlockLB->solid && position.x-width/2< nextBlockLB->x+nextBlockLB->w+CollisionThreshold);

    if(velocity.x<0 && (hitLT || hitLB))
    {
        velocity.x = 0;
        position.x = (hitTL?nextBlockLT->x:nextBlockLB->x)+nextBlockLB->w+width/2;
    }




    position+=velocity*deltatime;
}

void GameCharacter::draw()
{
    ofFill();
    ofSetColor(255,255,255,255);
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


