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
#include "GameCharacter.h"

class gameScreen
{
    public:
        gameScreen();
        virtual ~gameScreen();
        void init(Tracker& tracker);
        void loadBackgrounds();
        void loadBlocks();
        void update(float delta);
        void draw();
        void drawBackground();
        void keyPressed  (int key);
        void keyReleased (int key);
        void mousePressed(int x, int y, int button);

        GameBlock* GetBlock(int x, int y);

        Tracker* tracker;
        GameBlock blocks[GAMEBLOCK_COLS*GAMEBLOCK_ROWS];
        GameBlock outsideScreenBlock;
        GameBlock* selectedBlock;

        int blockWidth;
		int blockHeight;
		ofxXmlSettings XML;

        GameCharacter player;
        vector<GameBackground*> backgrounds;
        int currentBackgroundIndex;
        int nextBackgroundIndex;

    protected:
    private:
};

#endif // GAMESCREEN_H
