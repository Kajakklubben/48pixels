#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#define GAMEBLOCK_COLS 8
#define GAMEBLOCK_ROWS 6


#include "GameBlock.h"
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "SpriteAnimation.h"
#include "AnimationLoader.h"
#include "tracker.h"
#include "GameBackground.h"

class gameScreen
{
    public:
        gameScreen();
        virtual ~gameScreen();
        void init(Tracker& tracker);
        void loadBackgrounds();
        void update(float delta);
        void draw();
        void drawBackground();
        void keyPressed  (int key);

        Tracker* tracker;
        GameBlock blocks[GAMEBLOCK_COLS*GAMEBLOCK_ROWS];


        int blockWidth;
		int blockHeight;
		ofxXmlSettings XML;


        vector<GameBackground*> backgrounds;

    protected:
    private:
};

#endif // GAMESCREEN_H
