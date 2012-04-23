#ifndef ANIMATIONLOADER_H
#define ANIMATIONLOADER_H

#define ASSET_PATH "../../../assets/"
#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "SpriteAnimation.h"

class AnimationLoader
{
    public:
        AnimationLoader();
        virtual ~AnimationLoader();

        static SpriteAnimation* loadSprite(ofxXmlSettings xml);

        static void loadBackgroundAnimations(ofxXmlSettings xml);

        static vector<SpriteAnimation*> backgroundAnimations;


    protected:
    private:
};

#endif // ANIMATIONLOADER_H
