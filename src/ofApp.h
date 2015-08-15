#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "Branch.h"
#include "ofxAnimatableOfPoint.h"


class ofApp : public ofBaseApp{
    
private:
    void toggleCamera(bool & isOpen);
    void clearCanvas();
    void saveCanvas();
    void thresholdChanged(int & threshold);
    void addBranchAt(const ofVec2f &pos);
    
    ofVideoGrabber cam;
    
    ofxCv::ContourFinder contourFinder;
    ofxCv::TrackingColorMode trackingColorMode;
    
    ofxColorSlider  targetColor;
    ofxFloatSlider  threshold;
    ofxFloatSlider  camX;
    ofxFloatSlider  camY;
    ofxFloatSlider  pointSpeed;
    ofxToggle       bDrawDebug;
    ofxToggle       bDrawVideo;
    ofxToggle       bUseCamera;
    ofxToggle       bClearOnDraw;
    ofxToggle       bUseDiff;
    ofxButton       clearBtn;
    ofxButton       saveBtn;
    ofxPanel        gui;
    
    ofFbo           fbo;
    ofVec2f         *touchPos;
    
    ofxAnimatableOfPoint    pointAnim;
    float                   radiusX;
    float                   radiusY;
    float                   angle;
    
    
    // Clydias
    vector<Branch*>	branches;
    
    bool bClearCanvas;
    bool bSaveCanvas;
    bool bDrawGui;
    
    
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
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
};
