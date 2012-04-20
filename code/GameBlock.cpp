#include "GameBlock.h"

GameBlock::GameBlock()
{
}

void GameBlock::Set(int w, int h, int x, int y, int type)
{
    this->x = x;
    this->y = y;

    this->h = h;
    this->w = w;

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
    if(this->type=='w')
        ofSetColor(100,50,255);
    if(this->type=='g')
        ofSetColor(100,255,50);
    if(this->type=='b')
        ofSetColor(100,50,255);


    ofRect(x*w,y*h,h,w);
}
