#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include "ofMain.h"
#include <vector>

using namespace std;
class SpriteAnimation
{
    public:
        SpriteAnimation();
        virtual ~SpriteAnimation();

        void update(float delta);
		void draw(int x, int y);
		void fadeTo(int alpha, float time);

        int margin;
        int width;
        int height;

        int alpha;
        int targetAlpha;

        float tmpTweenTime;
        float tweenTime;

        float frameTime;
        int currentframe;


        static vector<ofImage> frames;

        static void load();

    protected:
    private:
};

#endif // SPRITEANIMATION_H
