#include "GameBackground.h"

GameBackground::GameBackground(SpriteAnimation* sprite)
{
    this->sprite = sprite;
}

GameBackground::~GameBackground()
{
    //dtor
}


void GameBackground::draw()
{
    ofSetColor(255);

    sprite->draw(0,0,0,ofGetWidth(),ofGetHeight());

    ofSetColor(0);
}
void GameBackground::update(float deltatime)
{

}
