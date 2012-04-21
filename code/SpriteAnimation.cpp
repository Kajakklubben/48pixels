#include "SpriteAnimation.h"

SpriteAnimation::SpriteAnimation()
{
    currentframe = 0;
    margin = 0;
    height = 10;
    width = 10;
}

SpriteAnimation::~SpriteAnimation()
{
    //dtor
}

void SpriteAnimation::update(float deltatime)
{
    if(alpha != targetAlpha)
    {
        alpha += alpha>targetAlpha?-1:1;
    }
    currentframe += 1;
        currentframe =  currentframe%frames.size();
}

void SpriteAnimation::draw(int frame, int x, int y, int w, int h)
{

    if(frames[0].size() > frame)
       frames[0][frame].draw(x-margin,y-margin,w+margin,h+margin);
    else
        printf("\n Warning: Animation is out of frames!");
}

void SpriteAnimation::draw(int frame, int x, int y)
{
    if(frames[0].size() > frame)
       draw(frame,x,y,frames[0][frame].width,frames[0][frame].height);
    else
        printf("\n Warning: Animation is out of frames!");
}

void SpriteAnimation::fadeTo(int target, float time)
{
    this->tmpTweenTime = time;
    this->tweenTime = time;
    this->targetAlpha = target;
}

void SpriteAnimation::load()
{

}
