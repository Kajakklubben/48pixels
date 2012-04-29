#include "gameScreen.h"

gameScreen::gameScreen()
{
    //Moved to init
    selectedBlock = 0;
    lastActionTime = -500;
}

gameScreen::~gameScreen()
{
    //dtor
}

void gameScreen::init(Tracker& initTracker, Gamepad& pad)
{
    gamepad = &pad;
    tracker = &initTracker;


	blockHeight = ofGetHeight()/GAMEBLOCK_ROWS;
	blockWidth = blockHeight;

    music.loadSound("../../../sound/music.mp3");
    music.setVolume(0.0f);
    music.play();
    music.setLoop(true);
    SoundLoader::load();
    //music.setPaused(true);


	if(XML.loadFile("../../../settings/settings.xml") ){
		printf("settings.xml loaded!");
		XML.pushTag("root");
	}else{
		printf("unable to load settings.xml check data/ folder");
	}

	//read the colors from XML
	//if the settings file doesn't exist we assigns default values (170, 190, 240)
	printf("\n red: %i \n",XML.getValue("BACKGROUND:COLOR:RED", 170));
	/*green	= XML.getValue("BACKGROUND:COLOR:GREEN", 190);
     blue	= XML.getValue("BACKGROUND:COLOR:BLUE", 240);
     */

	loadBackgrounds();
    currentBackgroundIndex = 0;
    backgrounds[currentBackgroundIndex]->show();
    loadBlocks();
    loadCharacter();

    for(int y=0;y<GAMEBLOCK_ROWS;y++)
    {
        for(int x=0;x<GAMEBLOCK_COLS;x++)
        {
            blocks[x][y].Set(blockWidth,blockHeight,GAME_STARTX+x*blockWidth,y*blockHeight);
            blocks[x][y].xIndex = x;
            blocks[x][y].yIndex = y;
            printf("\nSetting x: %i,%i : pos: %i,%i",GAME_STARTX+x,y,x*blockWidth,y*blockHeight);

            //This is super ugly, but a quick fix and we only have to do it once :
            int leftx = x-1;
            if(leftx<0 || leftx>=GAMEBLOCK_COLS)
                blocks[x][y].leftBlock = &outsideScreenBlock;
            else
                blocks[x][y].leftBlock = &blocks[leftx][y];

            int rightx = x+1;
            if(rightx<0 || rightx>=GAMEBLOCK_COLS)
                blocks[x][y].rightBlock = &outsideScreenBlock;
            else
                blocks[x][y].rightBlock = &blocks[rightx][y];

            int topy = y-1;
            if(topy<0 || topy>=GAMEBLOCK_ROWS)
                blocks[x][y].topBlock = &outsideScreenBlock;
            else
                blocks[x][y].topBlock = &blocks[x][topy];

            int bottomy = y+1;
            if(bottomy<0 || bottomy>=GAMEBLOCK_ROWS)
                blocks[x][y].bottomBlock = &outsideScreenBlock;
            else
                blocks[x][y].bottomBlock = &blocks[x][bottomy];


        }
    }


    player.game = this;
    player.setPosition(ofVec2f(600.0,100.0));
    outsideScreenBlock.SetType(BlockSolid);
}



void gameScreen::loadCharacter()
{
    AnimationLoader::loadCharacter(XML);

}
void gameScreen::loadBackgrounds()
{
    //Loading images
    printf("\n Loading backgorunds! ");
    AnimationLoader::loadBackgroundAnimations(XML);

    //Done loading images, loading settings
    XML.pushTag("backgrounds");
    int num = XML.getNumTags("background");

    for(int i = 0; i < num; i++)
    {
        //xml.pushTag("background", i);
        if(XML.attributeExists("background","spriteIndex",i))
        {
            int spriteIndex = XML.getAttribute("background","spriteIndex",0,i);
            int duration = XML.getAttribute("background","duration",0,i);
            int fade = XML.getAttribute("background","fade",0,i);

            GameBackground* bg = new GameBackground(AnimationLoader::backgroundAnimations[spriteIndex]);

            bg->duration = duration;
            bg->fade = fade;

            backgrounds.push_back(bg);

        }
    }
    XML.popTag();

    printf("\n Done loading backgrounds! ");

}

void gameScreen::loadBlocks()
{
    //Loading images
    printf("\n Loading blocks! ");
    AnimationLoader::loadBlockAnimations(XML);

    //Done loading images, loading settings
   /* XML.pushTag("blocks");
    int num = XML.getNumTags("block");

    for(int i = 0; i < num; i++)
    {
        //xml.pushTag("background", i);
        if(XML.attributeExists("block","spriteIndex",i))
        {
            int spriteIndex = XML.getAttribute("block","spriteIndex",0,i);
            int layer = XML.getAttribute("block","layer",0,i);

            GameBlock* block = new GameBlock();
            block->layer = layer;

            backgrounds.push_back(bg);

        }
    }
    XML.popTag();*/

    printf("\n Done loading blocks! ");

}

void gameScreen::update(float deltatime)
{
    for(int y=GAMEBLOCK_ROWS-1;y>=0;y--)
    {
        for(int x=0;x<GAMEBLOCK_COLS;x++)
        {
            blocks[x][y].updateSprite(1);

        }
        for(int x=GAMEBLOCK_COLS-1;x>=0;x--)
        {
            blocks[x][y].updateSprite(-1);

        }
    }

    if(gamepad->gamepadLeft())
    {
        lastActionTime = ofGetSystemTime();
        player.move(-1);
    }

    else if(gamepad->gamepadRight())
    {
        lastActionTime = ofGetSystemTime();
          player.move(1);

    }
    else
    {
        player.move(0);
    }
    if(gamepad->gamepadUp())
    {
         lastActionTime = ofGetSystemTime();
        player.jump();
    }


    if(gamepad->gamepadDown())
    {
         lastActionTime = ofGetSystemTime();
        player.down();
    }



    for(int y=0;y<GAMEBLOCK_ROWS;y++)
    {
        for(int x=0;x<GAMEBLOCK_COLS;x++)
        {
            blocks[x][y].Update(deltatime);
        }
    }
    for(int i = 0; i<backgrounds.size();i++)
        backgrounds[i]->update(deltatime);

    if(!backgrounds[currentBackgroundIndex]->showing)
    {
        nextBackgroundIndex = currentBackgroundIndex;
        currentBackgroundIndex =  (currentBackgroundIndex+1)%backgrounds.size();
        backgrounds[currentBackgroundIndex]->show();
    }

    player.update(deltatime);

    //return;
    #ifndef USE_TRACKER
        return;
    #endif
    bool changed = false;
    for(int x=0;x<GAMEBLOCK_COLS;x++){
            for(int y=0;y<GAMEBLOCK_ROWS;y++){
                int trackerX = GAMEBLOCK_COLS-x-1;
                if(tracker->blocks[x][y].invalid){
                    blocks[x][y].SetType(BlockNone);
                } else {

                    switch (tracker->blocks[x][y].blockColor) {
                        case BlockGreen:
                            if(blocks[x][y].SetType(BlockGrass))
                            {
                                changed = true;
                                for(int l=0;l<8;l++)
                                    SoundLoader::greenSounds[l].stop();
                                SoundLoader::greenSounds[GAMEBLOCK_ROWS-y].play();
                            }

                            break;
                        case BlockBrown:
                            if(blocks[x][y].SetType(BlockGround))
                            {
                                changed = true;
                                for(int l=0;l<8;l++)
                                    SoundLoader::redSounds[l].stop();
                                SoundLoader::redSounds[GAMEBLOCK_ROWS-y].play();
                            }

                            break;
                        case BlockBlue:
                            if(blocks[x][y].SetType(BlockWater))
                            {
                                changed = true;
                                for(int l=0;l<8;l++)
                                    SoundLoader::blueSounds[l].stop();
                                SoundLoader::blueSounds[GAMEBLOCK_ROWS-y].play();
                            }

                            break;
                        default:
                            if(blocks[x][y].SetType(BlockNone))
                            changed = true;
                            break;
                    }
                }

            }
        }
        if(changed)
            lastActionTime = ofGetSystemTime();


    if(ofGetSystemTime()-lastActionTime>MUSIC_INACTIVITY_TIME)
        music.setPaused(true);

    if(ofGetSystemTime()-lastActionTime<MUSIC_FADEOUT_TIME && !music.getIsPlaying())
        music.setPaused(false);




}

void gameScreen::drawBackground()
{

    backgrounds[nextBackgroundIndex]->draw();
    backgrounds[currentBackgroundIndex]->draw();
}

void gameScreen::draw()
{
    drawBackground();


    for(int l=0;l<5;l++)
    {
        for(int y=0;y<GAMEBLOCK_ROWS;y++)
        {
            for(int x=0;x<GAMEBLOCK_COLS;x++)
            {
                blocks[x][y].Draw(l);
            }
        }
    }
    ofSetColor(255,255,255,255);
    ofFill();

    player.draw();

    for(int l=5;l<GAME_MAX_LAYERS;l++)
    {
        for(int y=0;y<GAMEBLOCK_ROWS;y++)
        {
            for(int x=0;x<GAMEBLOCK_COLS;x++)
            {
                blocks[x][y].Draw(l);
            }
        }
    }
    if(player.state==C_Trapped)
    {
        ofSetColor(255,255,255,255);
          player.draw();
    }


    if(selectedBlock != 0)
    {
        ofNoFill();
        ofSetLineWidth(3);
        ofSetColor(230,50,50,50);
        ofRect(selectedBlock->x,selectedBlock->y,selectedBlock->w,selectedBlock->h);

        ofRect(selectedBlock->leftBlock->x+selectedBlock->leftBlock->h/2,selectedBlock->leftBlock->y+selectedBlock->leftBlock->w/2,selectedBlock->leftBlock->w/4,selectedBlock->leftBlock->h/4);
        ofRect(selectedBlock->rightBlock->x+selectedBlock->rightBlock->h/2,selectedBlock->rightBlock->y+selectedBlock->rightBlock->w/2,selectedBlock->rightBlock->w/4,selectedBlock->rightBlock->h/4);
        ofRect(selectedBlock->topBlock->x+selectedBlock->topBlock->h/2,selectedBlock->topBlock->y+selectedBlock->topBlock->w/2,selectedBlock->topBlock->w/4,selectedBlock->topBlock->h/4);
        ofRect(selectedBlock->bottomBlock->x+selectedBlock->bottomBlock->h/2,selectedBlock->bottomBlock->y+selectedBlock->bottomBlock->w/2,selectedBlock->bottomBlock->w/4,selectedBlock->bottomBlock->h/4);
        ofSetLineWidth(1);
        ofFill();
    }
    ofSetColor(255,255,255,255);
    ofFill();



}

void gameScreen::mousePressed(int x, int y, int button)
{
    selectedBlock = GetBlock(x, y);
    printf("\n Set selected block %i,%i",selectedBlock->xIndex,selectedBlock->yIndex);
}
void gameScreen::keyPressed  (int key){

	if (key == 'f'){

		ofSetFullscreen(true);

	}
	if (key == 'e'){
		selectedBlock->SetType(BlockGround);

	}

    if (key == 'g'){
		selectedBlock->SetType(BlockGrass);

	}
    if (key == 'w'){
		selectedBlock->SetType(BlockWater);

	}
    if (key == 'n'){
		selectedBlock->SetType(BlockNone);

	}

}
void gameScreen::keyReleased  (int key){

    if(key=='m')
    {
        player.move(0);
    }
    if(key=='n')
    {
        player.move(0);
    }

}

GameBlock* gameScreen::GetBlock(int x, int y)
{
    for(int yb=0;yb<GAMEBLOCK_ROWS;yb++)
    {
        for(int xb=0;xb<GAMEBLOCK_COLS;xb++)
        {
            if(
           x>=blocks[xb][yb].x
           && x<=blocks[xb][yb].x+blocks[xb][yb].w
           && y>=blocks[xb][yb].y
           && y<=blocks[xb][yb].y+blocks[xb][yb].h
           )
           {
                return &blocks[xb][yb];
           }

        }
    }

    if(x>=GAME_STARTX+GAME_WIDTH)
    {
        outsideScreenBlock.x = GAME_STARTX+GAME_WIDTH;
        outsideScreenBlock.y = y-blockHeight/2;
    }
    if(x<=GAME_STARTX)
    {
        outsideScreenBlock.x = GAME_STARTX-blockWidth;
        outsideScreenBlock.y = y-blockHeight/2;
    }

    if(y>=ofGetHeight())
    {
        outsideScreenBlock.x = x-blockWidth/2;
        outsideScreenBlock.y = ofGetHeight();
    }
    if(y<=0)
    {
        outsideScreenBlock.x = x-blockWidth/2;
        outsideScreenBlock.y = -blockHeight;
    }



    outsideScreenBlock.w = blockWidth;
    outsideScreenBlock.h = blockHeight;
    return &outsideScreenBlock;
}
