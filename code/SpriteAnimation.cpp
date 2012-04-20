#include "SpriteAnimation.h"
vector<ofImage> SpriteAnimation::frames;

SpriteAnimation::SpriteAnimation()
{
    currentframe = 0;
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

void SpriteAnimation::draw(int x, int y)
{
    if(frames.size() > currentframe)
       frames[currentframe].draw(x-margin,y-margin,width+margin,height+margin);
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
