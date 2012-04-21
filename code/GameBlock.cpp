#include "GameBlock.h"

GameBlock::GameBlock()
{
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
    this->type = type;
}

GameBlock::~GameBlock()
{

}

void GameBlock::Update(float deltatime)
{

}

void GameBlock::Draw()
{
    ofFill();
    if(this->type==BlockNone)
        ofSetColor(200,255,255);
    if(this->type==BlockGrass)
        ofSetColor(100,255,50);
    if(this->type==BlockWater)
        ofSetColor(100,50,255);
    if(this->type==BlockGround)
        ofSetColor(100,100,100);

    if(this->type!=BlockNone)
        ofRect(x*w,y*h,h,w);
}
