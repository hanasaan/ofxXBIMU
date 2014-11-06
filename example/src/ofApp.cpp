#include "ofMain.h"
#include "ofxXBIMU.h"

class ofApp : public ofBaseApp{
    ofxXBIMU xbimu;
    ofQuaternion rot;
    ofVec3f acc;
    ofVec3f gyr;
    ofVec3f mag;
    int batt;
    
    ofEasyCam ecam;
    ofCamera cam;
    ofLight light0;
    bool bDebugDraw;
public:
    void setup()
    {
        ofSetVerticalSync(true);
        ofSetFrameRate(60);
        xbimu.setup("/dev/tty.usbserial-00001014");
        bDebugDraw = false;
    }
    
    void update()
    {
        rot = xbimu.getYupRotation();
        acc = xbimu.getAcc();
        gyr = xbimu.getGyro();
        mag = xbimu.getMag();
        batt = xbimu.getBattery();
        
        cam.setGlobalOrientation(rot);
        cam.setPosition(50, 50, 50);
    }
    
    void draw()
    {
        ofClear(0);
        
        if (!bDebugDraw) {
            ecam.begin();
            ofDrawAxis(300);
            
            ofPushStyle();
            ofEnableDepthTest();
            ofEnableLighting();
            light0.setPosition(100, 1000, 1000);
            light0.enable();
            
            ofPushMatrix();
            ofMultMatrix(cam.getGlobalTransformMatrix());
            ofDrawBox(200);
            ofDrawAxis(200);
            ofPopMatrix();
            
            ofDisableLighting();
            ofDisableDepthTest();
            ofPopStyle();
            
            ecam.end();
        } else {
            cam.begin();
            ofDrawGrid(100);
            ofDrawAxis(100);
            cam.end();
        }
        
        {
            stringstream ss;
            ss << "rot : " << rot.getEuler() << endl;
            ss << "acc : " << acc << endl;
            ss << "gyr : " << gyr << endl;
            ss << "mag : " << mag << endl;
            ss << "batt: " << batt << endl;
            ofDrawBitmapStringHighlight(ss.str(), 10, 20);
        }
    }
    
    void keyPressed(int key)
    {
        if (key == OF_KEY_RETURN) {
            bDebugDraw = !bDebugDraw;
        }
    }
};

//========================================================================
int main( ){
	ofSetupOpenGL(1280,720,OF_WINDOW);			// <-------- setup the GL context
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
}
