#ifndef GAMEBLOCK_H
#define GAMEBLOCK_H
#include "ofMain.h"
#include "SpriteAnimation.h"
#include "AnimationLoader.h"

enum GameBlockType {
    BlockNone=-1,
    BlockGrass=0,
    BlockGround=1,
    BlockWater=2,
    BlockSolid=3

};

enum BlockAnimationType {
    S_earth=0,
    S_pond=1,
    S_cloud=2,
    S_spring=3,
    S_bush=4,
    S_trunk=5,
    S_vine=6,
    S_vinetrunk=7,
    S_grass=8,
    S_lillypadFlower=9,
    S_lillypadStalk=10,
    S_lillypad=11,
    S_floatingPlant=12,
    S_springStalk=13,
    S_earthWater=14,
    S_waterplant=15,
    S_waterplantTrunk=16
};

const double BlockFadeTime = 0.8;
const double BlockFadeInTime = 0.8;
class GameBlock
{
    public:

        GameBlock();

        void Set(int h, int w, int x, int y);
        bool SetType(GameBlockType type);

        virtual ~GameBlock();
        void Draw(int layer);
        void drawOldSprite();
        void Update(float delta);
        void updateSprite(int dir);


        GameBlockType type;
        int xIndex;
        int yIndex;

        int x;
        int y;

        int h;
        int w;

        int margin;
        bool solid;
        bool water;

        double lifetime;
        double oldSpriteLifetime;

        SpriteAnimation* oldSprite;
        SpriteAnimation* sprite;

        GameBlock* topBlock;
        GameBlock* bottomBlock;
        GameBlock* leftBlock;
        GameBlock* rightBlock;


    protected:
    private:
};

#endif // GAMEBLOCK_H
