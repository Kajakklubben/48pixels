#ifndef GAMEBACKGROUND_H
#define GAMEBACKGROUND_H

#include "ofMain.h"
#include "SpriteAnimation.h"


class GameBackground
{
    public:
        GameBackground(SpriteAnimation* sprite);
        virtual ~GameBackground();
        void draw();
        void update(float deltatime);


        SpriteAnimation* sprite;
        int fade;
        int duration;

    protected:
    private:
};

#endif // GAMEBACKGROUND_H
