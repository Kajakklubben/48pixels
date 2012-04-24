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


class GameBlock
{
    public:

        GameBlock();

        void Set(int h, int w, int x, int y);
        void SetType(GameBlockType type);

        virtual ~GameBlock();
        void Draw();
        void Update(float delta);
        void updateSprite(int dir);

        GameBlockType type;
        int x;
        int y;

        int h;
        int w;

        int margin;
        bool solid;

        double lifetime;

        SpriteAnimation* sprite;

        GameBlock* topBlock;
        GameBlock* bottomBlock;
        GameBlock* leftBlock;
        GameBlock* rightBlock;


    protected:
    private:
};

#endif // GAMEBLOCK_H
