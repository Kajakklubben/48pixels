#include "GameBackground.h"
class GameScreen;

GameBackground::GameBackground(SpriteAnimation* sprite)
{
    this->sprite = sprite;
    showing = false;
    timer = 0;
    targetAlpha = 0;
    alpha = 0;

}

GameBackground::~GameBackground()
{
    //dtor
}


void GameBackground::draw()
{
    ofSetColor(255,255,255,(int)(alpha*255));

    sprite->draw(0,GAME_STARTX,0,GAME_WIDTH,ofGetHeight());

    ofSetColor(0);
}
void GameBackground::update(float deltatime)
{

    timer+=deltatime;

    if(showing && timer>duration)
    {
        timer=0;
        //targetAlpha = 0;
        showing = false;
    }
    alpha += (targetAlpha>alpha?1:-1)*deltatime/fade;

}

void GameBackground::show()
{
        showing = true;
        timer=0;
        alpha = 0;
        targetAlpha = 1;
}
