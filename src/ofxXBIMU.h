//
//  ofxXBIMU.h
//  example
//
//  Created by Yuya Hanai on 11/4/14.
//
//
#pragma once

#include "ofMain.h"

class XbimuReceiver;

class ofxXBIMU : public ofThread
{
public:
    ofxXBIMU();
    ~ofxXBIMU();
    
    void setup(string port);
    void stop();
    
    bool isOpen() const;
    ofQuaternion getRotation();
    ofQuaternion getYupRotation();
    ofVec3f getAcc();
    ofVec3f getGyro();
    ofVec3f getMag();
    int getBattery();
    
    bool isConnected() const;
protected:
    void threadedFunction();
    
    XbimuReceiver* impl;
    
    ofSerial serial;
    bool serialOpen;
    
    uint64_t lastReceivedTs;
};