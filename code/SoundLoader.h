#ifndef SOUNDLOADER_H
#define SOUNDLOADER_H

#define SOUND_PATH "../../../sound/"
#include "ofMain.h"

class SoundLoader
{
    public:
        SoundLoader();
        virtual ~SoundLoader();
        static void load();
        static ofSoundPlayer blueSounds[8];
         static ofSoundPlayer redSounds[8];
         static ofSoundPlayer greenSounds[8];

    protected:
    private:
};

#endif // ANIMATIONLOADER_H
