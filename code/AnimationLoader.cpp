#include "AnimationLoader.h"

vector<SpriteAnimation*>  AnimationLoader::backgroundAnimations;
vector<SpriteAnimation*>  AnimationLoader::blockAnimations;
vector<SpriteAnimation*>  AnimationLoader::characterAnimations;

AnimationLoader::AnimationLoader()
{
    //ctor
}

AnimationLoader::~AnimationLoader()
{
    //dtor
}


SpriteAnimation* AnimationLoader::loadSprite(ofxXmlSettings xml)
{

    SpriteAnimation* sprite = new SpriteAnimation();

    int numvar = xml.getNumTags("variation");
    for(int i = 0; i < numvar; i++)
    {
        xml.pushTag("variation", i);

        vector<ofImage> frames;
        int numfra = xml.getNumTags("frame");
        for(int f = 0; f < numfra; f++)
        {
           // xml.pushTag("frame", f);
            ofImage img;
            if(xml.attributeExists("frame","img",f))
            {
                img.loadImage(ASSET_PATH+xml.getAttribute("frame","img","",f));

                if(xml.attributeExists("frame","duration",f))
                    sprite->duration = xml.getAttribute("frame","duration",1.0,f);

                frames.push_back(img);
                //printf("\n Frame loaded successful! ");
            }
            //xml.popTag();

        }

        sprite->frames.push_back(frames); //add varitaion
        xml.popTag();
    }
    return sprite;
}

void AnimationLoader::loadBackgroundAnimations(ofxXmlSettings xml)
{

    xml.pushTag("backgroundSprites");
    xml.pushTag("sprites");
    int num = xml.getNumTags("sprite");
    printf("\n %i number of background sprites",num);
    for(int i = 0; i < num; i++)
    {
        xml.pushTag("sprite", i);
        backgroundAnimations.push_back(loadSprite(xml));
        xml.popTag();
    }
    xml.popTag();
    xml.popTag();
}

void AnimationLoader::loadBlockAnimations(ofxXmlSettings xml)
{

    xml.pushTag("blockSprites");
    xml.pushTag("sprites");
    int num = xml.getNumTags("sprite");
    printf("\n %i number of block sprites",num);
    for(int i = 0; i < num; i++)
    {
        xml.pushTag("sprite", i);
        blockAnimations.push_back(loadSprite(xml));
        xml.popTag();
    }
    xml.popTag();
    xml.popTag();

    printf("\nblockAnimations loaded: %i",blockAnimations.size());
}

void AnimationLoader::loadCharacter(ofxXmlSettings xml)
{

    xml.pushTag("characterSprites");
    xml.pushTag("sprites");
    int num = xml.getNumTags("sprite");
    printf("\n %i number of character sprites",num);
    for(int i = 0; i < num; i++)
    {
        xml.pushTag("sprite", i);
        characterAnimations.push_back(loadSprite(xml));
        xml.popTag();
    }
    xml.popTag();
    xml.popTag();

    printf("\n characteSprites loaded: %i",characterAnimations.size());
}
