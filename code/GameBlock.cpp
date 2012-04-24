#include "GameBlock.h"

GameBlock::GameBlock()
{
    lifetime = 0;
    type = BlockNone;
    margin = 10;
    solid = true;

}

void GameBlock::Set(int w, int h, int x, int y)
{
    this->x = x;
    this->y = y;

    this->h = h;
    this->w = w;

    this->type = BlockNone;
    solid = false;
}
void GameBlock::SetType(GameBlockType type)
{
    if(this->type != type)
        lifetime = 0;

    this->type = type;

    if(type==BlockNone)
        solid = false;
    else
        solid = true;
   // else
        //printf("\nBlock animation index %i does not exist",type);

}

GameBlock::~GameBlock()
{

}

void GameBlock::Update(float deltatime)
{
    lifetime +=deltatime;

}
void GameBlock::updateSprite(int dir){

    if(type==BlockGround)
    {
/*
        //Earth
        Default earth
        -empty space above = grass

        */
        //if empty space above, then grass on top, else earth
        if(topBlock->type==BlockNone || topBlock->type==BlockSolid)
            sprite = AnimationLoader::blockAnimations[8];
        else
            sprite = AnimationLoader::blockAnimations[0];

    }

    if(type==BlockWater)
    {

        /*
        //Water
        Default cloud
        -Earth to the sides and below = pond
        -Earth under = spring

        */


        if(dir>0)
        {
            if((leftBlock->type==BlockGround || leftBlock->type==BlockSolid || leftBlock->sprite==AnimationLoader::blockAnimations[1]) &&
               (bottomBlock->type==BlockGround || bottomBlock->type==BlockSolid ||bottomBlock->sprite==AnimationLoader::blockAnimations[1]))
            {
                sprite = AnimationLoader::blockAnimations[1];
                return;
            }



        }
        else if(dir<0)
        {
            if((rightBlock->type==BlockGround || rightBlock->type==BlockSolid || rightBlock->sprite==AnimationLoader::blockAnimations[1]) &&
               (leftBlock->type==BlockGround || leftBlock->type==BlockSolid || leftBlock->sprite==AnimationLoader::blockAnimations[1]) &&
               (bottomBlock->type==BlockGround || bottomBlock->type==BlockSolid ||bottomBlock->sprite==AnimationLoader::blockAnimations[1]))
           {
                sprite = AnimationLoader::blockAnimations[1];
                return;
            }
            else if(bottomBlock->type==BlockGround || bottomBlock->type==BlockSolid || bottomBlock->sprite==AnimationLoader::blockAnimations[1])
            {
                sprite = AnimationLoader::blockAnimations[3];
                return;
            }
        }

        sprite = AnimationLoader::blockAnimations[2];

       /* if((leftBlock->type==BlockGround || leftBlock->sprite==AnimationLoader::blockAnimations[1]) && (rightBlock->type==BlockGround || rightBlock->sprite==AnimationLoader::blockAnimations[1]) && (bottomBlock->type==BlockGround || bottomBlock->sprite==AnimationLoader::blockAnimations[1]))
            sprite = AnimationLoader::blockAnimations[1];
        else if(bottomBlock->type==BlockGround || bottomBlock->sprite==AnimationLoader::blockAnimations[1] )
            sprite = AnimationLoader::blockAnimations[3];
        else
            sprite = AnimationLoader::blockAnimations[2];
            */

    }

    if(type==BlockGrass)
    {

        sprite =AnimationLoader::blockAnimations[9];
        /*
        //Plant
        Default plant
        -Earth to both sides and under = vine
        -Earth to both sides and under, vine above = vinetrunk
        -Vine below = vine

        -Earth below = bush
        -Earth below and bush above = trunk
        -Bush below = bush

        -Pond underneath = lilypad
        -Pond underneath and lilypad above = lilypadStalk
        -lillypad underneath = lilypadflower

        */
        /*
        if(leftBlock->type==BlockGround && rightBlock->type==BlockGround && bottomBlock->type==BlockGround)
            sprite =AnimationLoader::blockAnimations[1];
        else if(bottomBlock->type==BlockGround)
            sprite =AnimationLoader::blockAnimations[3];
        else
            sprite =AnimationLoader::blockAnimations[2];
            */

    }
}
void GameBlock::Draw()
{
    if(type == BlockNone || type == BlockSolid)
        return;

    float  frameLength = sprite->duration;

    int frame = (int)roundf(lifetime / sprite->duration)%(int)sprite->frames[0].size();
    ofSetColor(255);

    ofNoFill();

    double newMargin = (double)w/(double)sprite->frames[0][frame].width*(double)margin;
    /*ofRect(x-newMargin,y-newMargin,w+newMargin*2,h+newMargin*2);
    ofRect(x,y,w,h);*/

    sprite->draw(frame,x-newMargin,y-newMargin,w+newMargin*2,h+newMargin*2);
    ofSetColor(0);
    ofRect(x,y,w,h);

}
