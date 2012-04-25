#include "gamepad.h"



bool Gamepad::gamepadRight(){
    return gamepadState[1];
}
bool Gamepad::gamepadLeft(){
    return gamepadState[3];
}
bool Gamepad::gamepadUp(){
    return gamepadState[0];
}
bool Gamepad::gamepadDown(){
    return gamepadState[2];
}



//------


void Gamepad::setup(){
    serial.listDevices();
    printf("\nserial ready: %i ready %i",serial.available());
#ifdef TARGET_OSX
    serialConnected = serial.setup("/dev/tty.usbserial-A800eIrv", 9600);
#else
    serialConnected = serial.setup("\\\\.\\COM11", 9600);
#endif

    if(!serialConnected){
        ofLog(OF_LOG_ERROR, "Gamepad not connected");
    }

    for(int i=0;i<4;i++){
        gamepadState[i] = false;
    }

}
void Gamepad::update(){
    if(serialConnected){
        while(serial.available() > 0){
            int read = serial.readByte();
            printf("\ninput%i,", read);
            bool on = true;
            if(read & 128){
                read -= 128;
                on = false;
            }

            switch (read) {
                case 9:
                    gamepadState[3] = on;
                    break;
                case 10:
                    gamepadState[0] = on;
                    break;
                case 11:
                    gamepadState[1] = on;
                    break;
                case 8:
                    gamepadState[2] = on;
                    break;
                default:
                    break;
            }
        }
    }

}

void Gamepad::keyPressed(int key){
    if(key == OF_KEY_UP){
        gamepadState[0] = true;
    }
    if(key == OF_KEY_RIGHT){
        gamepadState[1] = true;
    }
    if(key == OF_KEY_DOWN){
        gamepadState[2] = true;
    }
    if(key == OF_KEY_LEFT){
        gamepadState[3] = true;
    }
}
void Gamepad::keyReleased(int key){
    if(key == OF_KEY_UP){
        gamepadState[0] = false;
    }
    if(key == OF_KEY_RIGHT){
        gamepadState[1] = false;
    }
    if(key == OF_KEY_DOWN){
        gamepadState[2] = false;
    }
    if(key == OF_KEY_LEFT){
        gamepadState[3] = false;
    }
}
