#include "GameCharacter.h"
#include "gameScreen.h"

GameCharacter::GameCharacter()
{
    moveDir = 0;
    lastDiretion = 1;
    gravity = ofVec2f(0,1200);


    drawWidth = ofGetWidth()/8;
    drawHeight = ofGetHeight()/6;
    width = drawWidth/2;
    height = drawHeight/2;
    state = C_Air;
    moveDown = false;
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
    lifetime +=deltatime;

    velocity.x += GameCharacter_movespeed*moveDir;
    velocity.x = CLAMP(velocity.x,-GameCharacter_maxMovespeed,GameCharacter_maxMovespeed);
    if(moveDir==0)
    {
        if( fabs(velocity.x)<GameCharacter_movespeed)
            velocity.x *=0.7; //Hardcoded smooth stop. Fine for installation that only runs on one computer.
        else
            velocity.x += velocity.x<0?GameCharacter_movespeed:-GameCharacter_movespeed;
    }
    if(velocity.y<=0)
        moveDown = false;

    velocity += gravity*deltatime;
    if(velocity.length()>GameCharacter_maxVelocity)
        velocity = velocity.normalized()*GameCharacter_maxVelocity;

    float CollisionThreshold = 10;
    ofVec2f nextPos = position+velocity*deltatime;

    GameBlock* currentBlock = game->GetBlock(position.x,position.y);
    GameBlock* bottomBlock = game->GetBlock(position.x,position.y+height/2);
    if(currentBlock->solid)
    {
            state =C_Trapped;
            return;
    }
    else
    if(currentBlock->water || bottomBlock->water)
    {
        if(state !=C_Swimming)
            velocity.y *=0.5;

            state =C_Swimming;
        if(currentBlock->water)
            velocity +=-1.5*gravity*deltatime;
    }
    else
        state =C_Air;



    GameBlock* nextBlockBL = game->GetBlock(nextPos.x-width/2+2,nextPos.y+height/2+CollisionThreshold);
    GameBlock* nextBlockBR = game->GetBlock(nextPos.x+width/2-1,nextPos.y+height/2+CollisionThreshold);

    bool hitBR = (nextBlockBR->solid && position.y+height/2<nextBlockBR->y+CollisionThreshold);
    bool hitBL = (nextBlockBL->solid && position.y+height/2<nextBlockBL->y+CollisionThreshold);

    if(velocity.y>=0 && (hitBR || hitBL))
    {
        velocity.y = 0;
        if(state!=C_Swimming)
            state =C_Grounded;

        position.y = (hitBR?nextBlockBR->y:nextBlockBL->y)-height/2;
    }

    GameBlock* nextBlockTL = game->GetBlock(nextPos.x-width/2+3,nextPos.y-height/2-CollisionThreshold);
    GameBlock* nextBlockTR = game->GetBlock(nextPos.x+width/2-3,nextPos.y-height/2-CollisionThreshold);

    bool hitTR = (nextBlockTR->solid && position.y-height/2> nextBlockTR->y-CollisionThreshold);
    bool hitTL = (nextBlockTL->solid && position.y-height/2> nextBlockTL->y-CollisionThreshold);

    if(velocity.y<0 && (hitTR || hitTL))
    {
        velocity.y = 0;

        position.y = (hitTL?nextBlockTL->y:nextBlockTR->y)+height/2+nextBlockTR->h;

    }


    GameBlock* nextBlockLT = game->GetBlock(nextPos.x-width/2-CollisionThreshold,position.y+height/2);
    GameBlock* nextBlockLB = game->GetBlock(nextPos.x-width/2-CollisionThreshold,position.y-height/2);


    bool hitLT = (nextBlockLT->solid && position.x-width/2> nextBlockLT->x+nextBlockLT->w-CollisionThreshold);
    bool hitLB = (nextBlockLB->solid && position.x-width/2> nextBlockLB->x+nextBlockLB->w-CollisionThreshold);

    if(velocity.x<0 && (hitLT || hitLB))
    {
        velocity.x = 0;
        position.x = (hitTL?nextBlockLT->x:nextBlockLB->x)+nextBlockLB->w+width/2;
    }


    GameBlock* nextBlockRT = game->GetBlock(nextPos.x+width/2+CollisionThreshold,position.y+height/2);
    GameBlock* nextBlockRB = game->GetBlock(nextPos.x+width/2+CollisionThreshold,position.y-height/2);


    bool hitRT = (nextBlockRT->solid && position.x+width/2< nextBlockRT->x+CollisionThreshold);
    bool hitRB = (nextBlockRB->solid && position.x+width/2< nextBlockRB->x+CollisionThreshold);

    if(velocity.x>0 && (hitRT || hitRB))
    {
        velocity.x = 0;
        position.x = (hitRT?nextBlockRT->x:nextBlockRB->x)-width/2;
    }

    position+=velocity*deltatime;
}

void GameCharacter::draw()
{
    /*ofFill();
    ofSetColor(255,255,255,255);
    ofRect(position.x-width/2,position.y-height/2,width,height);*/





    int x = position.x-drawWidth/2;
    int y = position.y-drawHeight/2;
    int w = drawWidth;
    int h = drawHeight;
    if(lastDiretion>0)
    {
        x += drawWidth;
        w = -drawWidth;
    }
    if(moveDown && state!=C_Grounded)
    {
        y += drawHeight;
        h = -drawHeight;
    }

    if(state!=C_Grounded && velocity.y<0)
        AnimationLoader::characterAnimations[S_jump]->draw(0,x,y,w,h);
    else if(state!=C_Grounded)
        AnimationLoader::characterAnimations[S_fall]->draw(0,x,y,w,h);
    else if(fabs(velocity.x)>0)
    {
        int frame = (int)roundf(lifetime / AnimationLoader::characterAnimations[S_walk]->duration)%(int)AnimationLoader::characterAnimations[S_walk]->frames[0].size();
        AnimationLoader::characterAnimations[S_walk]->draw(frame,x,y,w,h);
    }
    else
        AnimationLoader::characterAnimations[S_idle]->draw(0,x,y,w,h);

}


void GameCharacter::move(int dir)
{
    moveDir = dir;

    if(moveDir != 0)
        lastDiretion = moveDir;


}

void GameCharacter::jump()
{


    if(state==C_Grounded)
    {
        velocity.y = -GameCharacter_jumpheight;
    }
    if(state==C_Swimming)
    {
        velocity.y = -GameCharacter_waterjumpheight;
    }

}
void GameCharacter::down()
{

    moveDown = true;
    if(velocity.y<80)
        velocity.y = 80;

}


