#ifndef GAMEBLOCK_H
#define GAMEBLOCK_H
#include "ofMain.h"

enum GameBlockType {
    BlockGrass=0,
    BlockGround=1,
    BlockWater=2,
    BlockNone=3,
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

        GameBlockType type;
        int x;
        int y;

        int h;
        int w;

    protected:
    private:
};

#endif // GAMEBLOCK_H
