#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#define GAMEBLOCK_COLS 8
#define GAMEBLOCK_ROWS 6
#define GAME_MAX_LAYERS 10

#include "GameBlock.h"
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "SpriteAnimation.h"
#include "AnimationLoader.h"
#include "tracker.h"
#include "GameBackground.h"
#include "GameCharacter.h"
#include "gamepad.h"

class GameBackground;
const int GAME_WIDTH  = 1200;
const int GAME_HEIGHT = 900;
const int GAME_STARTX = 120;

const int MUSIC_INACTIVITY_TIME = 30000;
const int MUSIC_FADEOUT_TIME = 10000;

class gameScreen
{
    public:
        gameScreen();
        virtual ~gameScreen();
        void init(Tracker& tracker, Gamepad& pad);
        void loadBackgrounds();
        void loadBlocks();
        void loadCharacter();
        void update(float delta);
        void draw();
        void drawBackground();
        void keyPressed  (int key);
        void keyReleased (int key);
        void mousePressed(int x, int y, int button);

        GameBlock* GetBlock(int x, int y);

        Tracker* tracker;
        Gamepad* gamepad;
        GameBlock blocks[GAMEBLOCK_COLS][GAMEBLOCK_ROWS];
        GameBlock outsideScreenBlock;
        GameBlock* selectedBlock;
        ofSoundPlayer music;
        int lastActionTime;
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
