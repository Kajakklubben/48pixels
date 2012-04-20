#pragma once

#include "ofMain.h"
#include "GameBlock.h"
#include <vector>
using namespace std;


#define GAMEBLOCK_COLS 8
#define GAMEBLOCK_ROWS 6

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		float 	counter;
		bool	bSmooth;

        int blockWidth;
		int blockHeight;
		GameBlock blocks[GAMEBLOCK_COLS*GAMEBLOCK_ROWS];
};

