#ifndef GAMEBACKGROUND_H
#define GAMEBACKGROUND_H

#include "ofMain.h"
#include "SpriteAnimation.h"
#include "GameScreen.h"

class GameBackground
{
    public:
        GameBackground(SpriteAnimation* sprite);
        virtual ~GameBackground();
        void draw();
        void update(float deltatime);
        void show();

        SpriteAnimation* sprite;
        float fade;
        int duration;
        float timeOffset;

        float timer;
        bool showing;
        float targetAlpha;
        float alpha;


    protected:
    private:
};

#endif // GAMEBACKGROUND_H
