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
//    bool sign = ((fixedQ115 >> 15) & 0x01) == 0x01;
//    float f = (fixedQ115 & 0x7fff) / (float)(0x8000);
//    if (sign) {
//        return f;
//    } else {
//        return -f;
//    }
    return 0.0001f * fixedQ115;
}

QuaternionStruct quat;
SensorStruct sensor;
BatteryStruct battery;


ofxXBIMU::ofxXBIMU() : impl(NULL)
{
    impl = new XbimuReceiver();
    serialOpen = false;
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

ofQuaternion ofxXBIMU::getRotation() {
    ofQuaternion q;
    q.x() = toFloat(quat.x);
    q.y() = toFloat(quat.y);
    q.z() = toFloat(quat.z);
    q.w() = toFloat(quat.w);
    return q;
}

ofVec3f ofxXBIMU::getAcc() {
    ofVec3f v = ofVec3f(sensor.accX, sensor.accY, sensor.accZ);
    return v;
}

ofVec3f ofxXBIMU::getGyro() {
    ofVec3f v = ofVec3f(sensor.gyrX, sensor.gyrY, sensor.gyrZ);
    return v;
}

ofVec3f ofxXBIMU::getMag() {
    ofVec3f v = ofVec3f(sensor.magX, sensor.magY, sensor.magZ);
    return v;
}

int ofxXBIMU::getBattery() {
    return battery.voltage;
}
