#include "gameScreen.h"

gameScreen::gameScreen()
{
    blockWidth = ofGetWidth()/GAMEBLOCK_COLS;
	blockHeight = ofGetHeight()/GAMEBLOCK_ROWS;


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

    for(int i=0;i<GAMEBLOCK_COLS*GAMEBLOCK_ROWS;i++)
    {
        blocks[i].Set(blockWidth,blockHeight,(i%GAMEBLOCK_COLS)*blockWidth,i/GAMEBLOCK_COLS*blockHeight);
    }


    player.setPosition(ofVec2f(100.0,100.0));
}


gameScreen::~gameScreen()
{
    //dtor
}

void gameScreen::init(Tracker& initTracker)
{
    tracker = &initTracker;
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

    for(int i=0;i<GAMEBLOCK_COLS*GAMEBLOCK_ROWS;i++)
    {
        blocks[i].Update(deltatime);
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

return;
    for(int x=0;x<8;x++){
            for(int y=0;y<6;y++){

                if(tracker->blocks[x][y].invalid){
                    blocks[GAMEBLOCK_COLS*y+x].SetType(BlockNone);
                } else {
                    switch (tracker->blocks[x][y].blockColor) {
                        case BlockGreen:
                            blocks[GAMEBLOCK_COLS*y+x].SetType(BlockGrass);
                            break;
                        case BlockBrown:
                            blocks[GAMEBLOCK_COLS*y+x].SetType(BlockGround);
                            break;
                        case BlockBlue:
                            blocks[GAMEBLOCK_COLS*y+x].SetType(BlockWater);
                            break;
                        default:
                            blocks[GAMEBLOCK_COLS*y+x].SetType(BlockNone);
                            break;
                    }
                }

            }
        }

}

void gameScreen::drawBackground()
{

    backgrounds[nextBackgroundIndex]->draw();
    backgrounds[currentBackgroundIndex]->draw();
}

void gameScreen::draw()
{
    drawBackground();



    for(int i=0;i<GAMEBLOCK_COLS*GAMEBLOCK_ROWS;i++)
    {
        blocks[i].Draw();
    }

     player.draw();


}

void gameScreen::keyPressed  (int key){

    int x = rand()%GAMEBLOCK_COLS;
    int y = rand()%GAMEBLOCK_ROWS;

	if (key == 'e'){
		blocks[y*GAMEBLOCK_COLS+x].SetType(BlockGround);
		printf("Set new test block\n");
	}

    if (key == 'g'){
		blocks[y*GAMEBLOCK_COLS+x].SetType(BlockGrass);
		printf("Set new test block\n");
	}
    if (key == 'w'){
		blocks[y*GAMEBLOCK_COLS+x].SetType(BlockWater);
		printf("Set new test block\n");
	}


	player.move(1);
}
