#include "tracker.h"

//------------------

void Tracker::setup(){
    videoGrabber.initGrabber(CAM_W, CAM_H);
    
    //Defeault settings
    setCalibrationCorner(ofVec2f(0.1,0.1), 0);
    setCalibrationCorner(ofVec2f(0.9,0.1), 1);
    setCalibrationCorner(ofVec2f(0.9,0.9), 2);
    setCalibrationCorner(ofVec2f(0.1,0.9), 3);
    
    handleHover = -1;
    handleSelected = -1;
    
    for(int x=0;x<8;x++){
        for(int y=0;y<6;y++){
            blocks[x][y].runningAverageColor.r = 0;
            blocks[x][y].runningAverageColor.g = 0;
            blocks[x][y].runningAverageColor.b = 0;
            
            blocks[x][y].age = 0;
        }
    }
}


//------------------


void Tracker::setCalibrationCorner(ofVec2f p, int corner){
    if(corner >= 0 && corner < 4){
        calibrationCorners[corner] = p * ofVec2f(CAM_W,CAM_H);
        calibrationCorners[corner].x = MAX(0,MIN(CAM_W,calibrationCorners[corner].x));
        calibrationCorners[corner].y = MAX(0,MIN(CAM_H,calibrationCorners[corner].y));
    }

    ofVec2f src[4];
    src[0] = ofVec2f(0,0);
    src[1] = ofVec2f(7,0);
    src[2] = ofVec2f(7,5);
    src[3] = ofVec2f(0,5);
    
    coordWarper.calculateMatrix(src, calibrationCorners);
}

//------------------

ofVec2f Tracker::blockPixelLocationInCamera(int x, int y){
    return coordWarper.transform(x,y);    
}

//------------------

void Tracker::update(){
    videoGrabber.update();
    
    
    unsigned char * pixels = videoGrabber.getPixels();
    
    for(int x=0;x<8;x++){
        for(int y=0;y<6;y++){
            //Find the local average for this block
            ofVec2f pixelLoc = blockPixelLocationInCamera(x, y);
            
            Color color;
            color.r = 0;
            color.g = 0;
            color.b = 0;
            
            for(int row=pixelLoc.y-trackingAreaSize/2 ; row<pixelLoc.y+trackingAreaSize/2 ; row++){
                unsigned char * pixel = (unsigned char*) pixels + int((row*CAM_W + (pixelLoc.x - trackingAreaSize/2)))*3;
                
                for(int i=0 ; i<trackingAreaSize ; i++, pixel += 3){
                    color.r += pixel[0]/255.0;
                    color.g += pixel[1]/255.0;
                    color.b += pixel[2]/255.0;
                }
            }

            color.r /= trackingAreaSize*trackingAreaSize;
            color.g /= trackingAreaSize*trackingAreaSize;
            color.b /= trackingAreaSize*trackingAreaSize;
            
            blocks[x][y].runningAverageColor.r = blocks[x][y].runningAverageColor.r * (1-runningAverageAmount) + (color.r * runningAverageAmount);
            blocks[x][y].runningAverageColor.g = blocks[x][y].runningAverageColor.g * (1-runningAverageAmount) + (color.g * runningAverageAmount);
            blocks[x][y].runningAverageColor.b = blocks[x][y].runningAverageColor.b * (1-runningAverageAmount) + (color.b * runningAverageAmount);}
    }
}

//------------------

void Tracker::drawDebug(){
    float w = CAM_W;
    float h = CAM_H;
    
    ofSetColor(255, 255, 255);
    
    videoGrabber.draw(0, 0, w, h);
    
    ofPushStyle(); {
        ofEnableAlphaBlending();
        
        //Draw block locations        
        for(int x=0;x<8;x++){
            for(int y=0;y<6;y++){
                ofVec2f pos = blockPixelLocationInCamera(x, y);

                ofNoFill();
                ofSetLineWidth(1);
                glColor3f(blocks[x][y].runningAverageColor.r, blocks[x][y].runningAverageColor.g, blocks[x][y].runningAverageColor.b);
                ofCircle(pos.x, pos.y, 7);
                
            }
        }
        
        //Draw handles
        int prevI = 3;
        for(int i=0;i<4;i++){
            //Handle
            ofFill();
            ofSetColor(255, 255, 255, 50);
            if(handleHover == i)
                ofSetColor(60, 60, 255, 150);
            ofCircle(calibrationCorners[i].x, calibrationCorners[i].y, 7);            

            ofNoFill();
            ofSetColor(100, 100, 150, 255);
            ofCircle(calibrationCorners[i].x, calibrationCorners[i].y, 7);

            //Line between handles
            ofSetColor(100, 100, 100, 255);
            ofLine(calibrationCorners[i].x, calibrationCorners[i].y, calibrationCorners[prevI].x, calibrationCorners[prevI].y);
            
            prevI = i;
        }            
        
    } ofPopStyle();
    
    
    //Info box
    int infoBoxX = 650;
    int infoBoxY = 0;
    if(blockHover.x != -1){
        ofFill();
        ofSetColor(0, 0, 0,100);
        ofRect(infoBoxX, infoBoxY, 200, 480);

        ofNoFill();
        ofSetColor(255, 255, 255, 200);
        ofRect(infoBoxX, infoBoxY, 200, 480);
     
        ofSetColor(255, 255, 255);
        int y= 15;
        ofDrawBitmapString("Super Info Box", infoBoxX +5, infoBoxY + y);
        
        ofSetColor(220, 220, 220);

        y += 15;
        ofDrawBitmapString("Block "+ofToString(blockHover.x)+"x"+ofToString(blockHover.y), infoBoxX +5, infoBoxY + y);

        Block block = blocks[(int)blockHover.x][(int)blockHover.y];
        
        y += 25;
        ofDrawBitmapString("r: "+ofToString(block.runningAverageColor.r,2)+"\n"+"g: "+ofToString(block.runningAverageColor.g,2)+"\n"+"b: "+ofToString(block.runningAverageColor.b,2), infoBoxX +5, infoBoxY + y);
        
        ofFill();
        glColor3f(block.runningAverageColor.r, block.runningAverageColor.g, block.runningAverageColor.b);
        ofRect(infoBoxX + 70, y-10, 30, 40);
    }
    
    ofSetColor(0, 0, 0);
    
}


void Tracker::mouseMoved(int x, int y ){
    handleHover = -1;
    for(int i=0;i<4;i++){
        ofVec2f mouse = ofVec2f(x,y);
        if(mouse.distance(calibrationCorners[i]) < 8){
            handleHover = i; 
        }
    }
    
    blockHover = ofVec2f(-1,-1);
    for(int bx=0;bx<8;bx++){
        for(int by=0;by<6;by++){
            ofVec2f mouse = ofVec2f(x,y);
            if(mouse.distance(blockPixelLocationInCamera(bx,by)) < 8){
                blockHover = ofVec2f(bx,by); 
            }
        }
    }
}
void Tracker::mouseDragged(int x, int y, int button){
    if(handleSelected >= 0){
        ofVec2f pos = ofVec2f((float)x/CAM_W, (float)y/CAM_H);
        
        setCalibrationCorner(pos, handleSelected);
    }
}
void Tracker::mousePressed(int x, int y, int button){
    handleSelected = -1;
    for(int i=0;i<4;i++){
        ofVec2f mouse = ofVec2f(x,y);
        if(mouse.distance(calibrationCorners[i]) < 8){
            handleSelected = i; 
        }
    }
}
void Tracker::mouseReleased(int x, int y, int button){
    handleSelected = -1;
}


