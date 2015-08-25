#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "Branch.h"
#include "PointAnimator.h"
#include "ofxAnimatableOfPoint.h"

class ofApp : public ofBaseApp{

private:
    void toggleCamera(bool& isOpen);
    void toggleColorMode(bool& isHsv);
    void clearCanvas();
    void saveCanvas();
    void thresholdChanged(int& threshold);
    void addBranchAt(const ofVec2f& pos, const ofColor& color);
    void setupGui();
    
    void addBranchesFromImage(const ofImage& image, const ofVec2f& pos);
    
    ofVideoGrabber cam;
    
    ofxCv::ContourFinder contourFinder;
    ofxCv::TrackingColorMode trackingColorMode;
    
    ofxToggle       bUseCamera;
    
    ofxLabel        drawingLabel;
    ofxLabel        trackingLabel;
    ofxLabel        generalLabel;
    
    ofxColorSlider  branchColor;
    ofxColorSlider  targetColor;
    ofxFloatSlider  threshold;
    ofxFloatSlider  camX;
    ofxFloatSlider  camY;
    
    ofxFloatSlider  pointSpeed;
    ofxFloatSlider  branchDiffusion;
    ofxFloatSlider  pointRadiusX;
    ofxFloatSlider  pointRadiusY;
    
    ofxToggle       bUseHSV;
    ofxToggle       bDrawDebug;
    ofxToggle       bDrawVideo;
    ofxToggle       bUseDiff;
    ofxToggle       bBlackAndWhite;
    ofxButton       clearBtn;
    ofxToggle       bUseAnimator;
    ofxButton       saveBtn;
    ofxPanel        gui;
    
    ofFbo           fbo;
    
    bool bDrawGui;
    
    PointAnimator   animator;
    // Branches
    vector<Branch*>	branches;
    
    
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
