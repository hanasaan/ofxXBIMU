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
    ofLight light0;
public:
    void setup()
    {
        ofSetVerticalSync(true);
        ofSetFrameRate(60);
        
        xbimu.setup("/dev/tty.usbserial-00001014");
    }
    
    void update()
    {
        rot = xbimu.getRotation();
        acc = xbimu.getAcc();
        gyr = xbimu.getGyro();
        mag = xbimu.getMag();
        batt = xbimu.getBattery();
    }
    
    void draw()
    {
        ofClear(0);
        
        ecam.begin();
        ofDrawAxis(300);
        
        ofPushStyle();
        ofEnableDepthTest();
        ofEnableLighting();
        light0.setPosition(100, 1000, 1000);
        light0.enable();
        
        ofPushMatrix();
        ofMultMatrix(ofMatrix4x4(rot.conj()));
        ofDrawBox(200);
        ofDrawAxis(200);
        ofPopMatrix();
        
        ofDisableLighting();
        ofDisableDepthTest();
        ofPopStyle();
        
        ecam.end();
        
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
    
    void keyPressed(int key) {}
    void keyReleased(int key) {}
    void mouseMoved(int x, int y ) {}
    void mouseDragged(int x, int y, int button) {}
    void mousePressed(int x, int y, int button) {}
    void mouseReleased(int x, int y, int button) {}
    void windowResized(int w, int h) {}
    void dragEvent(ofDragInfo dragInfo) {}
    void gotMessage(ofMessage msg) {}
};

//========================================================================
int main( ){
	ofSetupOpenGL(1280,720,OF_WINDOW);			// <-------- setup the GL context
    
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
}
