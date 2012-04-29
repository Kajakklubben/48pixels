#include "SoundLoader.h"

ofSoundPlayer  SoundLoader::greenSounds[8];
ofSoundPlayer  SoundLoader::redSounds[8];
ofSoundPlayer  SoundLoader::blueSounds[8];

SoundLoader::SoundLoader()
{
}
void SoundLoader::load()
{
    //ctor
greenSounds[0].loadSound("../../../sound/1_1.mp3");
greenSounds[1].loadSound("../../../sound/1_2.mp3");
greenSounds[2].loadSound("../../../sound/1_3.mp3");
greenSounds[3].loadSound("../../../sound/1_4.mp3");
greenSounds[4].loadSound("../../../sound/1_5.mp3");
greenSounds[5].loadSound("../../../sound/1_6.mp3");
greenSounds[6].loadSound("../../../sound/1_7.mp3");
greenSounds[7].loadSound("../../../sound/1_8.mp3");

redSounds[0].loadSound("../../../sound/2_1.mp3");
redSounds[1].loadSound("../../../sound/2_2.mp3");
redSounds[2].loadSound("../../../sound/2_3.mp3");
redSounds[3].loadSound("../../../sound/2_4.mp3");
redSounds[4].loadSound("../../../sound/2_5.mp3");
redSounds[5].loadSound("../../../sound/2_6.mp3");
redSounds[6].loadSound("../../../sound/2_7.mp3");
redSounds[7].loadSound("../../../sound/2_8.mp3");


blueSounds[0].loadSound("../../../sound/3_1.mp3");
blueSounds[1].loadSound("../../../sound/3_2.mp3");
blueSounds[2].loadSound("../../../sound/3_3.mp3");
blueSounds[3].loadSound("../../../sound/3_4.mp3");
blueSounds[4].loadSound("../../../sound/3_5.mp3");
blueSounds[5].loadSound("../../../sound/3_6.mp3");
blueSounds[6].loadSound("../../../sound/3_7.mp3");
blueSounds[7].loadSound("../../../sound/3_8.mp3");

}

SoundLoader::~SoundLoader()
{
    //dtor
}
