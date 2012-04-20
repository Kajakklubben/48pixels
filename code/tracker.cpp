#include "tracker.h"

void Tracker::setup(){
    videoGrabber.initGrabber(640, 480);
}

void Tracker::update(){
    videoGrabber.update();
}

void Tracker::drawDebug(){
    videoGrabber.draw(0, 0, 640, 480);
}

