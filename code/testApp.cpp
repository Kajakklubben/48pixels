#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    tracker.setup();
    ofBackground(255,255,255);
	ofSetWindowTitle("48 by Kajakklubben");
	ofSetFrameRate(60);

}

//--------------------------------------------------------------
void testApp::update(){
    game.update(1);
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
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

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
