#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "Branch.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void clearCanvas();
    
    void thresholdChanged(int & threshold);
    
    ofVideoGrabber cam;
    
    ofxCv::ContourFinder contourFinder;
    ofxCv::TrackingColorMode trackingColorMode;
    
    ofxColorSlider  targetColor;
    ofxFloatSlider  threshold;
    ofxFloatSlider  camX;
    ofxFloatSlider  camY;
    ofxToggle       bDrawDebug;
    ofxToggle       bDrawVideo;
    ofxToggle       bClearOnDraw;
    ofxToggle       bUseDiff;
    ofxToggle       bUseColor;
    ofxButton       clearBtn;
    ofxPanel        gui;
    
    // Clydias
    vector<Branch*>	branches;
    
    bool bClearCanvas;
    bool bDrawGui;
};
