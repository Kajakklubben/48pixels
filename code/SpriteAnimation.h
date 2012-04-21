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
        void draw (int frame, int x, int y);
		void draw(int frame, int x, int y, int w, int h);
		void fadeTo(int alpha, float time);

        int layer;
        int duration;
        int fade;

        int margin;
        int width;
        int height;

        int alpha;
        int targetAlpha;

        float tmpTweenTime;
        float tweenTime;

        float frameTime;
        int currentframe;


        vector<vector<ofImage> > frames; //and variations

        static void load();

    protected:
    private:
};

#endif // SPRITEANIMATION_H
