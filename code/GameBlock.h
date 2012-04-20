#ifndef GAMEBLOCK_H
#define GAMEBLOCK_H
#include "ofMain.h"

class GameBlock
{
    public:

        GameBlock();

        void Set(int h, int w, int x, int y, int type);
        virtual ~GameBlock();
        void Draw();
        void Update(float delta);

        int type;
        int x;
        int y;

        int h;
        int w;

    protected:
    private:
};

#endif // GAMEBLOCK_H
