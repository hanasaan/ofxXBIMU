//
//  ofxXBIMU.cpp
//  example
//
//  Created by Yuya Hanai on 11/4/14.
//
//

#include "ofxXBIMU.h"
#include "XbimuReceiver.h"

static inline float toFloat(int fixedQ115) {
    bool sign = ((fixedQ115 >> 15) & 0x01) == 0x01;
    if (sign) {
        return 0.0001f * (fixedQ115 - 65535);
    } else {
        return 0.0001f * fixedQ115;
    }
}

QuaternionStruct quat;
SensorStruct sensor;
BatteryStruct battery;


ofxXBIMU::ofxXBIMU() : impl(NULL)
{
    impl = new XbimuReceiver();
    serialOpen = false;
    lastReceivedTs = 0;
}

ofxXBIMU::~ofxXBIMU()
{
    stop();
    delete impl;
}

void ofxXBIMU::setup(string port)
{
    stop();
    serialOpen = serial.setup(port, 115200);
    
    if(serialOpen){
        serial.flush();
        startThread();
    }
}

void ofxXBIMU::stop()
{
    if(serialOpen){
        waitForThread(true);
        serial.flush();
        serial.close();
        serialOpen = false;
    }
}

void ofxXBIMU::threadedFunction()
{
    serial.flush();
    
    while(isThreadRunning()) {
        while (serial.available() > 0) {
            char c = (char) serial.readByte();
            lock();
            impl->processNewChar(c);
            if (impl->isQuaternionGetReady()) {
                quat = impl->getQuaternion();
                lastReceivedTs = ofGetElapsedTimeMillis();
            }
            if (impl->isSensorGetReady()) {
                sensor = impl->getSensor();
            }
            if (impl->isBatteryGetReady()) {
                battery = impl->getBattery();
            }
            unlock();
        }
        ofSleepMillis(1);
    }
}

bool ofxXBIMU::isOpen() const
{
    return serialOpen;
}

bool ofxXBIMU::isConnected() const
{
    return (ofGetElapsedTimeMillis() - lastReceivedTs) < 1000;
}

ofQuaternion ofxXBIMU::getRotation() {
    ofQuaternion q;
    q.x() = toFloat(quat.x);
    q.y() = toFloat(quat.y);
    q.z() = toFloat(quat.z);
    q.w() = toFloat(quat.w);
    return q;
}

ofQuaternion ofxXBIMU::getYupRotation() {
    ofQuaternion rot = getRotation();
    return ofQuaternion(90, ofVec3f(1, 0, 0)) * rot.conj() * ofQuaternion(-90, ofVec3f(1, 0, 0));
}

ofVec3f ofxXBIMU::getAcc() {
    ofVec3f v = ofVec3f(toFloat(sensor.accX), toFloat(sensor.accY), toFloat(sensor.accZ));
    return v;
}

ofVec3f ofxXBIMU::getGyro() {
    ofVec3f v = ofVec3f(toFloat(sensor.gyrX), toFloat(sensor.gyrY), toFloat(sensor.gyrZ));
    return v;
}

ofVec3f ofxXBIMU::getMag() {
    ofVec3f v = ofVec3f(toFloat(sensor.magX), toFloat(sensor.magY), toFloat(sensor.magZ));
    return v;
}

int ofxXBIMU::getBattery() {
    return battery.voltage;
}
