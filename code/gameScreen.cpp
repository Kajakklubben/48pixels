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


    for(int i=0;i<GAMEBLOCK_COLS*GAMEBLOCK_ROWS;i++)
    {
        blocks[i].Set(blockWidth,blockHeight,i%GAMEBLOCK_COLS,i/GAMEBLOCK_COLS);
    }
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

void gameScreen::update(float deltatime)
{
    for(int i=0;i<GAMEBLOCK_COLS*GAMEBLOCK_ROWS;i++)
    {
        blocks[i].Update(deltatime);
    }

}

void gameScreen::drawBackground()
{
    if(backgrounds.size()>0)
        backgrounds[0]->draw();
}

void gameScreen::draw()
{
    drawBackground();



    for(int i=0;i<GAMEBLOCK_COLS*GAMEBLOCK_ROWS;i++)
    {
        blocks[i].Draw();
    }

    for(int x=0;x<8;x++){
            for(int y=0;y<6;y++){

                ofNoFill();
                ofSetLineWidth(1);
                //glColor3f(blocks[x][y].runningAverageColor.r, blocks[x][y].runningAverageColor.g, blocks[x][y].runningAverageColor.b);
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
                ofCircle(x*20, y*20, 7);
            }
        }

}

void gameScreen::keyPressed  (int key){

    int x = rand()%GAMEBLOCK_COLS;
    int y = rand()%GAMEBLOCK_ROWS;

	if (key == 'w'){
		//blocks[y*GAMEBLOCK_COLS+x].Set(blockWidth,blockHeight,x,y,'w');
		printf("Set new block\n");
	}

    if (key == 'g'){
		//blocks[y*GAMEBLOCK_COLS+x].Set(blockWidth,blockHeight,x,y,'g');
		printf("Set new block\n");
	}
}
