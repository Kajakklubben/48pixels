#pragma once

#include "ofMain.h"

class Gamepad {
public:
    void setup();
    void update();
    
    void keyPressed(int key);
    void keyReleased(int key);
    
    //Call these functions to get state
    bool gamepadRight();
    bool gamepadLeft();
    bool gamepadUp();
    bool gamepadDown();
    
    bool serialConnected;

private:    
    ofSerial serial;
    bool gamepadState[4];
};