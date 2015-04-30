#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
    
    //------------------------------------------------
    // Main setup
    //------------------------------------------------
    ofBackground(0);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetFrameRate(60);
    
    float camWidth = 640;
    float camHeight = 480;
    cam.initGrabber(camWidth, camHeight);
    
    contourFinder.setMinAreaRadius(10);
    contourFinder.setMaxAreaRadius(200);
    // wait for half a frame before forgetting something
    contourFinder.getTracker().setPersistence(15);
    // an object can move up to 32 pixels per frame
    contourFinder.getTracker().setMaximumDistance(32);
    
    trackingColorMode = TRACK_COLOR_RGB;
    
    
    clearBtn.addListener(this, &ofApp::clearCanvas);
    
    gui.setup();
    gui.add(targetColor.setup("Track Color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    gui.add(threshold.setup("Threshold", 14, 10, 120));
    gui.add(camX.setup("Camera X", 0, 0, ofGetWidth()-camWidth));
    gui.add(camY.setup("Camera Y", 0, 0, ofGetHeight()-camHeight));
    gui.add(bDrawDebug.setup("Draw Debug", true));
    gui.add(bDrawVideo.setup("Draw Video", true));
    gui.add(bClearOnDraw.setup("Clear on Draw", true));
    gui.add(bUseDiff.setup("Use Difference", true));
    gui.add(bUseColor.setup("Use Color", false));
    gui.add(clearBtn.setup("Clear"));
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetBackgroundAuto(bClearOnDraw);
    
    if (bClearCanvas) {
        clearCanvas();
        bClearCanvas = false;
    }
    
    cam.update();
    if (cam.isFrameNew()) {
        blur(cam, 25);
        contourFinder.setThreshold(threshold);
        contourFinder.findContours(cam);
    }
    
    // Update tracked positions
    int n = contourFinder.size();
    for(int i = 0; i < n; i++) {
        
        // minimum area circle that encloses the contour
        ofSetColor(cyanPrint);
        float circleRadius;
        ofVec2f circleCenter = toOf(contourFinder.getMinEnclosingCircle(i, circleRadius));
        ofCircle(circleCenter, circleRadius);
        
        ofVec2f centroid = toOf(contourFinder.getCentroid(i));
        ofVec2f *tPos = new ofVec2f;
        tPos->x = ofMap(centroid.x, 0, cam.getWidth(), 0, ofGetWidth());
        tPos->y = ofMap(centroid.y, 0, cam.getHeight(), 0, ofGetHeight());
        
        Branch *branch = new Branch;
        branch->setup(*tPos, ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));
        branch->setDrawMode(CL_BRANCH_DRAW_LEAVES);
        branches.push_back(branch);
    }
    
    // update clydias
    for (int i=0; i<branches.size(); i++){
        if (branches[i]->getIsAlive()){
            branches[i]->update();
        } else {
            delete branches[i];
            branches[i] = 0;
            branches.erase(branches.begin()+i);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    RectTracker& tracker = contourFinder.getTracker();
    
    if (bDrawVideo) {
        ofSetColor(255);
        cam.draw(camX, camY);
    }
    
    for (auto branch : branches){
        ofSetColor(255);
        ofSetLineWidth(1);
        branch->draw();
    }
    
    if (bDrawDebug){
        
//        contourFinder.draw();
        
        ofSetColor(255);
        int n = contourFinder.size();
        for(int i = 0; i < n; i++) {
            // convex hull of the contour
            ofPushMatrix();
            ofTranslate(camX, camY);
            
            ofSetColor(yellowPrint);
            ofPolyline convexHull = toOf(contourFinder.getConvexHull(i));
            convexHull.draw();
            
            ofPoint center = toOf(contourFinder.getCenter(i));
            ofPushMatrix();
            ofTranslate(center.x, center.y);
            int label = contourFinder.getLabel(i);
            string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
            ofDrawBitmapString(msg, 0, 0);
            ofVec2f velocity = toOf(contourFinder.getVelocity(i));
            ofScale(5, 5);
            ofLine(0, 0, velocity.x, velocity.y);
            ofPopMatrix();
            ofPopMatrix();
        }
        
        // this chunk of code visualizes the creation and destruction of labels
        const vector<unsigned int>& currentLabels = tracker.getCurrentLabels();
        const vector<unsigned int>& previousLabels = tracker.getPreviousLabels();
        const vector<unsigned int>& newLabels = tracker.getNewLabels();
        const vector<unsigned int>& deadLabels = tracker.getDeadLabels();
        ofSetColor(cyanPrint);
        for(int i = 0; i < currentLabels.size(); i++) {
            int j = currentLabels[i];
            ofLine(j, 0, j, 4);
        }
        ofSetColor(magentaPrint);
        for(int i = 0; i < previousLabels.size(); i++) {
            int j = previousLabels[i];
            ofLine(j, 4, j, 8);
        }
        ofSetColor(yellowPrint);
        for(int i = 0; i < newLabels.size(); i++) {
            int j = newLabels[i];
            ofLine(j, 8, j, 12);
        }
        ofSetColor(ofColor::white);
        for(int i = 0; i < deadLabels.size(); i++) {
            int j = deadLabels[i];
            ofLine(j, 12, j, 16);
        }
    }
    
    if(bDrawGui){
        ofSetColor(255);
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    clearBtn.removeListener(this, &ofApp::clearCanvas);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch (key){
        case 'h':
            trackingColorMode = TRACK_COLOR_HSV;
            contourFinder.setTargetColor(targetColor, trackingColorMode);
            break;
        case 'r':
            trackingColorMode = TRACK_COLOR_RGB;
            contourFinder.setTargetColor(targetColor, trackingColorMode);
            break;
        case ' ':
            if (bDrawGui) {
                bClearCanvas = true;
            } else {
                bDrawGui = !bDrawGui;
            }
            break;
        case 'd':
            bDrawDebug = !bDrawDebug;
            break;
        case 'v':
            bDrawVideo = !bDrawVideo;
            break;
        case 's':
            gui.saveToFile("settings.xml");
            break;
        case 'l':
            gui.loadFromFile("settings.xml");
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::clearCanvas(){
    for (int i=0;i<branches.size();i++){
        delete branches[i];
        branches[i] = 0;
    }
    branches.clear();
    
    ofBackground(0);
    ofSetColor(0, 255);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    ofRectangle rect(camX, camY, cam.getWidth(), cam.getHeight());
    if (rect.inside(x, y)) {
        auto pixels = cam.getPixelsRef();
        targetColor = pixels.getColor(x - camX, y - camY);
        contourFinder.setTargetColor(targetColor, trackingColorMode);
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
