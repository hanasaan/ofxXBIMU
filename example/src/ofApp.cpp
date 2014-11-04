#include "ofMain.h"

class ofApp : public ofBaseApp{
    
public:
    void setup()
    {
        ofSetVerticalSync(true);
        ofSetFrameRate(60);
    }
    
    void update() {}
    void draw()
    {
        ofClear(0);
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
