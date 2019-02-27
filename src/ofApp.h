#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "Branch.h"
#include "PointAnimator.h"
#include "ColorAnimator.h"

#include "Constants.h"

#include "DrawingParams.h"
#include "AnimatorParams.h"
#include "TrackingParams.h"


#define IMAGE_DRAW_RES_X    50
#define IMAGE_DRAW_RES_Y    50
#define DRAG_DRAW_RES       5

class ofApp : public ofBaseApp {
    
private:
    void toggleCamera(bool& isOpen);
    void toggleColorMode(bool& isHsv);
    void clearCanvas();
    void saveCanvas();
    void thresholdChanged(int& threshold);
    void addBranchAt(const ofPoint& pos, const ofColor& color);
    void setupGui();
    
    void addBranchesFromImage(const ofImage& image, const ofVec2f& pos);
    
    ofVideoGrabber cam;

    ofxCv::ContourFinder        contourFinder;
    ofxCv::TrackingColorMode    trackingColorMode;
    
    ofParameterGroup        parameters;
    AnimatorParams          animatorParams;
    DrawingParams           drawingParams;
    TrackingParams          trackingParams;
    
    ofxButton       clearBtn;
    ofxButton       saveBtn;
    ofxPanel        gui;
    
    ofFbo           fbo;
    
    ofPoint    mousePos;
    bool bIsMouseDown;
    bool bDrawGui;
    
    PointAnimator   pointAnimator;
    ColorAnimator   colorAnimator;
    
    int             counter;
    
    // Branches
    deque <ofPtr<Branch > > branches;
    
    
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
