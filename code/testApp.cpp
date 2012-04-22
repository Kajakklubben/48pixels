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
    game.update(1.0/ofGetFrameRate());
    tracker.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    game.draw();


    tracker.drawDebug();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    game.keyPressed(key);

    tracker.keyPressed(key);

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

    tracker.mouseMoved(x, y);

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    tracker.mouseDragged(x, y, button);

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    tracker.mousePressed(x, y, button);

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    tracker.mouseReleased(x, y, button);

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
