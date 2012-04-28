#include "GameBlock.h"

GameBlock::GameBlock()
{
    lifetime = 0;
    type = BlockNone;
    margin = 10;
    solid = true;
    oldSprite = 0;

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
bool GameBlock::SetType(GameBlockType type)
{
    bool changed = false;
    if(this->type != type)
    {
        oldSpriteLifetime = 0;
        lifetime = 0;
        changed = true;
    }


    this->type = type;

    solid = false;
    water = false;

    if(type==BlockGround || type==BlockSolid)
        solid = true;
    else if(type==BlockWater)
        water = true;

    oldSprite = sprite;
    if(type==BlockNone)
    {
        sprite = AnimationLoader::blockAnimations[0];

    }


   // else
        //printf("\nBlock animation index %i does not exist",type);

        return changed;

}

GameBlock::~GameBlock()
{

}

void GameBlock::Update(float deltatime)
{

    if(oldSprite==0)
    {
        oldSprite = sprite;
        oldSpriteLifetime = 0;
    }

    lifetime +=deltatime;
    oldSpriteLifetime +=deltatime;

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
void GameBlock::drawOldSprite()
{
    if(oldSprite == 0 || oldSprite==sprite)
        return;

    float pct = oldSpriteLifetime/BlockFadeTime;


    ofSetColor(255,255,255,(1.0-pct)*255);

    double newMargin = (double)w/(double)oldSprite->frames[0][0].width*(double)margin;
    oldSprite->draw(0,x-newMargin,y-newMargin,w+newMargin*2,h+newMargin*2);

    if(pct>1)
    {
        oldSprite = 0;
    }
}
void GameBlock::Draw(int layer)
{
    if(sprite != 0 && layer != sprite->layer)
        return;

    drawOldSprite();

    if(type == BlockNone || type == BlockSolid)
        return;

    double yOffset = 0;
    if(sprite==AnimationLoader::blockAnimations[S_floatingPlant] || sprite==AnimationLoader::blockAnimations[S_cloud])
        yOffset = sin(lifetime)*10;

    float  frameLength = sprite->duration;

    int frame = (int)roundf(lifetime / sprite->duration)%(int)sprite->frames[0].size();

    float pct = fmin(1.0,lifetime/BlockFadeInTime);

    ofSetColor(255,255,255,255* pct);

    ofNoFill();

    double newMargin = (double)w/(double)sprite->frames[0][frame].width*(double)margin;
    /*ofRect(x-newMargin,y-newMargin,w+newMargin*2,h+newMargin*2);
    ofRect(x,y,w,h);*/

    sprite->draw(frame,x-newMargin,yOffset+y-newMargin,w+newMargin*2,h+newMargin*2);
    ofSetColor(0);
    //ofRect(x,y,w,h);

}
