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
        if(topBlock->type==BlockNone || topBlock->type==BlockGrass ||
           topBlock->sprite==AnimationLoader::blockAnimations[S_spring]
           || topBlock->sprite==AnimationLoader::blockAnimations[S_springStalk]
           || topBlock->sprite==AnimationLoader::blockAnimations[S_cloud])
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
            if((leftBlock->type==BlockGround || leftBlock->type==BlockSolid || leftBlock->sprite==AnimationLoader::blockAnimations[S_pond]) &&
               (bottomBlock->type==BlockGround || bottomBlock->type==BlockSolid ||bottomBlock->sprite==AnimationLoader::blockAnimations[S_pond]))
            {
                sprite = AnimationLoader::blockAnimations[S_pond];
                return;
            }



        }
        else if(dir<0)
        {
            if((rightBlock->type==BlockGround || rightBlock->type==BlockSolid || rightBlock->sprite==AnimationLoader::blockAnimations[S_pond]) &&
               (leftBlock->type==BlockGround || leftBlock->type==BlockSolid || leftBlock->sprite==AnimationLoader::blockAnimations[S_pond]) &&
               (bottomBlock->type==BlockGround || bottomBlock->type==BlockSolid ||bottomBlock->sprite==AnimationLoader::blockAnimations[S_pond]))
           {
                sprite = AnimationLoader::blockAnimations[S_pond];
                return;
            }
            else if(bottomBlock->sprite==AnimationLoader::blockAnimations[S_spring] || bottomBlock->sprite==AnimationLoader::blockAnimations[S_springStalk])
            {
                sprite = AnimationLoader::blockAnimations[S_spring];
                bottomBlock->sprite = AnimationLoader::blockAnimations[S_springStalk];
                return;
            }
            else if(bottomBlock->type==BlockGround || bottomBlock->type==BlockSolid || bottomBlock->sprite==AnimationLoader::blockAnimations[S_pond])
            {
                sprite = AnimationLoader::blockAnimations[S_spring];
                return;
            }
        }

        sprite = AnimationLoader::blockAnimations[S_cloud];

    }

    if(type==BlockGrass)
    {

        if(dir>0)
        {
            /*if((leftBlock->type==BlockGround || leftBlock->type==BlockSolid || leftBlock->sprite==AnimationLoader::blockAnimations[S_pond]) &&
               (bottomBlock->type==BlockGround || bottomBlock->type==BlockSolid ||bottomBlock->sprite==AnimationLoader::blockAnimations[S_pond]))
            {
                sprite = AnimationLoader::blockAnimations[S_pond];
                return;
            }*/



        }
        else if(dir<0)
        {
            //vines
            if((rightBlock->type==BlockGround || rightBlock->type==BlockSolid) &&
               (leftBlock->type==BlockGround || leftBlock->type==BlockSolid) &&
               (bottomBlock->type==BlockGround || bottomBlock->type==BlockSolid))
           {
                sprite = AnimationLoader::blockAnimations[S_vine];
                return;
            }
            else if(bottomBlock->sprite==AnimationLoader::blockAnimations[S_vine] || bottomBlock->sprite==AnimationLoader::blockAnimations[S_vinetrunk])
            {
                sprite = AnimationLoader::blockAnimations[S_vine];
                bottomBlock->sprite = AnimationLoader::blockAnimations[S_vinetrunk];
                return;
            }
            //trees
            else if(bottomBlock->type==BlockGround || bottomBlock->type==BlockSolid)
           {
                sprite = AnimationLoader::blockAnimations[S_bush];
                return;
            }
            else if(bottomBlock->sprite==AnimationLoader::blockAnimations[S_bush] || bottomBlock->sprite==AnimationLoader::blockAnimations[S_trunk])
            {
                sprite = AnimationLoader::blockAnimations[S_bush];
                bottomBlock->sprite = AnimationLoader::blockAnimations[S_trunk];
                return;
            }
            //lillypads
            else if(bottomBlock->sprite==AnimationLoader::blockAnimations[S_pond] || bottomBlock->sprite==AnimationLoader::blockAnimations[S_lillypad])
           {
                sprite = AnimationLoader::blockAnimations[S_lillypadFlower];
                bottomBlock->sprite = AnimationLoader::blockAnimations[S_lillypad];
                return;
            }
            else if(bottomBlock->sprite==AnimationLoader::blockAnimations[S_lillypadFlower] || bottomBlock->sprite==AnimationLoader::blockAnimations[S_lillypadStalk])
            {
                sprite = AnimationLoader::blockAnimations[S_lillypadFlower];
                bottomBlock->sprite = AnimationLoader::blockAnimations[S_lillypadStalk];
                return;
            }

        }

        sprite = AnimationLoader::blockAnimations[S_floatingPlant];

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
