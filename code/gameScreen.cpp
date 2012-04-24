#include "gameScreen.h"

gameScreen::gameScreen()
{
    //Moved to init
    selectedBlock = 0;
}

gameScreen::~gameScreen()
{
    //dtor
}

void gameScreen::init(Tracker& initTracker, Gamepad& pad)
{
    gamepad = &pad;
    tracker = &initTracker;

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
    loadCharacter();

    for(int y=0;y<GAMEBLOCK_ROWS;y++)
    {
        for(int x=0;x<GAMEBLOCK_COLS;x++)
        {
            blocks[y*GAMEBLOCK_ROWS+x].Set(blockWidth,blockHeight,x*blockWidth,y*blockHeight);


            //This is super ugly, but a quick fix and we only have to do it once :
            int leftx = x-1;
            if(leftx<0 || leftx>GAMEBLOCK_COLS)
                blocks[y*GAMEBLOCK_ROWS+x].leftBlock = &outsideScreenBlock;
            else
                blocks[y*GAMEBLOCK_ROWS+x].leftBlock = &blocks[y*GAMEBLOCK_ROWS+leftx];

            int rightx = x+1;
            if(rightx<0 || rightx>=GAMEBLOCK_COLS)
                blocks[y*GAMEBLOCK_ROWS+x].rightBlock = &outsideScreenBlock;
            else
                blocks[y*GAMEBLOCK_ROWS+x].rightBlock = &blocks[y*GAMEBLOCK_ROWS+rightx];

            int topy = y-1;
            if(topy<0 || topy>=GAMEBLOCK_ROWS)
                blocks[y*GAMEBLOCK_ROWS+x].topBlock = &outsideScreenBlock;
            else
                blocks[y*GAMEBLOCK_ROWS+x].topBlock = &blocks[topy*GAMEBLOCK_ROWS+x];

            int bottomy = y+1;
            if(bottomy<0 || bottomy>=GAMEBLOCK_ROWS)
                blocks[y*GAMEBLOCK_ROWS+x].bottomBlock = &outsideScreenBlock;
            else
                blocks[y*GAMEBLOCK_ROWS+x].bottomBlock = &blocks[bottomy*GAMEBLOCK_ROWS+x];


        }
    }


    player.game = this;
    player.setPosition(ofVec2f(100.0,100.0));
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
            blocks[y*GAMEBLOCK_ROWS+x].updateSprite(1);

        }
        for(int x2=GAMEBLOCK_COLS-1;x2>=0;x2--)
        {
            blocks[y*GAMEBLOCK_ROWS+x2].updateSprite(-1);

        }
    }

    /*if(gamepad->gamepadLeft())
        player.move(-1);
    if(gamepad->gamepadRight())
        player.move(1);
    if(gamepad->gamepadUp())
        player.jump();*/

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

    #ifndef USE_TRACKER
        return;
    #endif
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
    player.draw();


}

void gameScreen::mousePressed(int x, int y, int button)
{
    selectedBlock = GetBlock(x, y);
    printf("\n Set selected block %i,%i",x,y);
}
void gameScreen::keyPressed  (int key){

	if (key == 'e'){
		selectedBlock->SetType(BlockGround);
		printf("Set new test block\n");
	}

    if (key == 'g'){
		selectedBlock->SetType(BlockGrass);
		printf("Set new test block\n");
	}
    if (key == 'w'){
		selectedBlock->SetType(BlockWater);
		printf("Set new test block\n");
	}


    if(key=='j')
    {
        player.jump();
    }

    if(key=='m')
    {
        player.move(1);
    }
    if(key=='n')
    {
        player.move(-1);
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
    for(int i=0;i<GAMEBLOCK_COLS*GAMEBLOCK_ROWS;i++)
    {
        if(
           x>blocks[i].x
           && x<blocks[i].x+blockWidth
           && y>blocks[i].y
           && y<blocks[i].y+blockHeight
           )
           {
                return &blocks[i];
           }

    }
    if(x>ofGetWidth())
    {
        outsideScreenBlock.x = ofGetWidth();
        outsideScreenBlock.y = y-blockHeight/2;
    }
    if(x<0)
    {
        outsideScreenBlock.x = -blockWidth;
        outsideScreenBlock.y = y-blockHeight/2;
    }

    if(y>ofGetHeight())
    {
        outsideScreenBlock.x = x-blockWidth/2;
        outsideScreenBlock.y = ofGetWidth();
    }
    if(y<0)
    {
        outsideScreenBlock.x = x-blockWidth/2;
        outsideScreenBlock.y = -blockHeight;
    }


    if(y>ofGetHeight()/2)
        outsideScreenBlock.y = ofGetHeight();
    else
        outsideScreenBlock.y = -blockHeight;


    outsideScreenBlock.w = blockWidth;
    return &outsideScreenBlock;
}
