#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    tracker.setup();
    ofBackground(0,0,0);
	ofSetWindowTitle("Farmors Baghave - a Kajakklubben project");
	ofSetFrameRate(60);

    trackerDebug = false;
    game.init(tracker);

}

//--------------------------------------------------------------
void testApp::update(){
    game.update(1);
    tracker.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    game.draw();

    if(trackerDebug){
        tracker.drawDebug();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(!trackerDebug)
        game.keyPressed(key);

    if(key=='c')
        trackerDebug = !trackerDebug ;

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    if(trackerDebug){
        tracker.mouseMoved(x, y);
    }
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    if(trackerDebug){
        tracker.mouseDragged(x, y, button);
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(trackerDebug){
        tracker.mousePressed(x, y, button);
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    if(trackerDebug){
        tracker.mouseReleased(x, y, button);
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
