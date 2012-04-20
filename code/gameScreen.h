#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#define GAMEBLOCK_COLS 8
#define GAMEBLOCK_ROWS 6


#include "ofMain.h"
#include "GameBlock.h"
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "SpriteAnimation.h"


class gameScreen
{
    public:
        gameScreen();
        virtual ~gameScreen();
        void loadBackgrounds();
        void update(float delta);
        void draw();
        void keyPressed  (int key);

        GameBlock blocks[GAMEBLOCK_COLS*GAMEBLOCK_ROWS];


        int blockWidth;
		int blockHeight;
		ofxXmlSettings XML;


        SpriteAnimation background;

    protected:
    private:
};

#endif // GAMESCREEN_H
