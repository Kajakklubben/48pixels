#include "GameBlock.h"

GameBlock::GameBlock()
{
    lifetime = 0;
    type = BlockNone;
    margin = 10;

}

void GameBlock::Set(int w, int h, int x, int y)
{
    this->x = x;
    this->y = y;

    this->h = h;
    this->w = w;

    this->type = BlockNone;
}
void GameBlock::SetType(GameBlockType type)
{
    if(this->type != type)
        lifetime = 0;

    this->type = type;
    if(AnimationLoader::blockAnimations.size()>type && type != BlockNone)
        sprite = AnimationLoader::blockAnimations[type];
    else
        printf("Block animation index %i does not exist",type);

}

GameBlock::~GameBlock()
{

}

void GameBlock::Update(float deltatime)
{
    lifetime +=deltatime;
}

void GameBlock::Draw()
{
    if(type == BlockNone)
        return;

    float  frameLength = sprite->duration;

    int frame = (int)roundf(lifetime / sprite->duration)%(int)sprite->frames[0].size();
    ofSetColor(255);

    ofNoFill();

    double newMargin = (double)w/(double)sprite->frames[0][frame].width*(double)margin;
    /*ofRect(x-newMargin,y-newMargin,w+newMargin*2,h+newMargin*2);
    ofRect(x,y,w,h);*/

    sprite->draw(frame,x-newMargin,y-newMargin,w+newMargin*2,h+newMargin*2);

}
