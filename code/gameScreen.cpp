#include "gameScreen.h"

gameScreen::gameScreen()
{
    blockWidth = ofGetWidth()/GAMEBLOCK_COLS;
	blockHeight = ofGetHeight()/GAMEBLOCK_ROWS;
}

gameScreen::~gameScreen()
{
    //dtor
}

void gameScreen::update(float deltatime)
{


    for(int i=0;i<GAMEBLOCK_COLS*GAMEBLOCK_ROWS;i++)
    {
        blocks[i].Update(1.1f);
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
