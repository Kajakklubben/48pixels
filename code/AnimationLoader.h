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

        static SpriteAnimation* loadSprite(ofxXmlSettings xml, int layer = 1);

        static void loadBackgroundAnimations(ofxXmlSettings xml);
        static void loadBlockAnimations(ofxXmlSettings xml);
        static void loadCharacter(ofxXmlSettings xml);
        static vector<SpriteAnimation*> backgroundAnimations;
         static vector<SpriteAnimation*> blockAnimations;
         static vector<SpriteAnimation*> characterAnimations;

    protected:
    private:
};

#endif // ANIMATIONLOADER_H
