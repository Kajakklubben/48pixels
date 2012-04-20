#include "gameScreen.h"

gameScreen::gameScreen()
{
    blockWidth = ofGetWidth()/GAMEBLOCK_COLS;
	blockHeight = ofGetHeight()/GAMEBLOCK_ROWS;


	if(XML.loadFile("../settings/settings.xml") ){
		printf("settings.xml loaded!");
	}else{
		printf("unable to load settings.xml check data/ folder");
	}

	//read the colors from XML
	//if the settings file doesn't exist we assigns default values (170, 190, 240)
	printf("red: %i \n",XML.getValue("BACKGROUND:COLOR:RED", 170));
	/*green	= XML.getValue("BACKGROUND:COLOR:GREEN", 190);
	blue	= XML.getValue("BACKGROUND:COLOR:BLUE", 240);
	*/
}

gameScreen::~gameScreen()
{
    //dtor
}

void gameScreen::update(float deltatime)
{


    for(int i=0;i<GAMEBLOCK_COLS*GAMEBLOCK_ROWS;i++)
    {
        blocks[i].Update(1.0f);
    }

}

void gameScreen::draw()
{
    for(int i=0;i<GAMEBLOCK_COLS*GAMEBLOCK_ROWS;i++)
    {
        blocks[i].Draw();
    }
}

void gameScreen::keyPressed  (int key){

    int x = rand()%GAMEBLOCK_COLS;
    int y = rand()%GAMEBLOCK_ROWS;

	if (key == 'w'){
		blocks[y*GAMEBLOCK_COLS+x].Set(blockWidth,blockHeight,x,y,'w');
		printf("Set new block\n");
	}

    if (key == 'g'){
		blocks[y*GAMEBLOCK_COLS+x].Set(blockWidth,blockHeight,x,y,'g');
		printf("Set new block\n");
	}
}
