#include "GameBlock.h"

GameBlock::GameBlock()
{
    lifetime = 0;
    type = BlockNone;
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
    int frame = 0;
    ofFill();
    if(this->type==BlockNone)
        ofSetColor(200,255,255);
    if(this->type==BlockGrass)
        ofSetColor(100,50,255);
    if(this->type==BlockWater)
        ofSetColor(100,50,255);
    if(this->type==BlockGround)
        ofSetColor(100,100,100);

    if(this->type!=BlockNone)
        ofRect(x*w,y*h,h,w);
}
