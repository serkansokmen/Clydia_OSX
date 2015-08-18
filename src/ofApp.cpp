#include "ofApp.h"

#define CAM_WIDTH       320
#define CAM_HEIGHT      240
#define MAX_BRANCHES    400

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
    
    //------------------------------------------------
    // Contour finder
    //------------------------------------------------
    contourFinder.setMinAreaRadius(10);
    contourFinder.setMaxAreaRadius(200);
    // wait for half a frame before forgetting something
    contourFinder.getTracker().setPersistence(15);
    // an object can move up to 32 pixels per frame
    contourFinder.getTracker().setMaximumDistance(32);
    
    setupGui();
    
    // Setup animator point
    animator.setup(ofGetWindowRect().getCenter(), ofColor::black);
    
    
#ifdef TARGET_OPENGLES
    // with alpha, 32 bits red, 32 bits green, 32 bits blue, 32 bits alpha, from 0 to 1 in 'infinite' steps
    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    ofLogWarning("ofApp") << "GL_RGBA32F_ARB is not available for OPENGLES.  Using RGBA.";
#else
    // with alpha, 32 bits red, 32 bits green, 32 bits blue, 32 bits alpha, from 0 to 1 in 'infinite' steps
    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB);
#endif
    
    clearCanvas();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetBackgroundAuto(bClearOnDraw);
    
    //app timebase, to send to all animatable objects
    float dt = 1.0f / 60.0f;
    animator.speed = pointSpeed;
    animator.radiusX = pointRadiusX;
    animator.radiusY = pointRadiusY;
    animator.update(dt);
    if (animator.color.getTargetColor() != branchColor) {
        animator.colorTo(branchColor);
    }
    addBranchAt(animator.point.getCurrentPosition());
    
    if (bUseCamera && cam.isInitialized()) {
        cam.update();
        if (cam.isFrameNew()) {
            ofxCv::blur(cam, 25);
            contourFinder.setThreshold(threshold);
            contourFinder.findContours(cam);
        }
        
        // Update tracked positions
        int n = contourFinder.size();
        for(int i = 0; i < n; i++) {
            ofVec2f centroid = ofxCv::toOf(contourFinder.getCentroid(i));
            ofVec2f *tPos = new ofVec2f;
            tPos->x = ofMap(centroid.x, 0, cam.getWidth(), 0, ofGetWidth());
            tPos->y = ofMap(centroid.y, 0, cam.getHeight(), 0, ofGetHeight());
            
            addBranchAt(*tPos);
        }
    }
    
    // update clydias
    if (branches.size() < MAX_BRANCHES) {
        for (int i=0; i<branches.size(); i++) {
            auto b = branches[i];
            if (b->getIsAlive()) {
                b->update(pointSpeed*0.1f);
            } else {
                delete b;
                b = 0;
                branches.erase(branches.begin()+i);
            }
        }
    } else {
        branches.erase(branches.begin());
    }
    
    // Draw to fbo
    fbo.begin();
    for (auto b : branches) {
        ofSetColor(255);
        ofSetLineWidth(0.1f);
        b->draw();
    }
    fbo.end();
    
    if (bDrawGui) {
        gui.setBackgroundColor(branchColor);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    

    ofBackgroundGradient(ofColor::black, ofColor::black, OF_GRADIENT_CIRCULAR);
    
    fbo.draw(0, 0);

    ofPushStyle();
    ofSetColor(255);
    ofNoFill();
    ofSetLineWidth(2.0f);
    ofDrawCircle(animator.point.getCurrentPosition(), 20);
    ofSetColor(255, 20);
    ofFill();
    ofDrawCircle(animator.point.getCurrentPosition(), 18);
    ofPopStyle();
    
    ofxCv::RectTracker& tracker = contourFinder.getTracker();
    
    if (bDrawVideo && bUseCamera) {
        ofSetColor(255);
        cam.draw(camX, camY, cam.getWidth(), cam.getHeight());
    }
    
    if (bDrawGui){
        ofSetColor(255);
        gui.draw();
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key){
        case ' ':
            bDrawGui = !bDrawGui;
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
    
    fbo.begin();
    ofClear(255, 255, 255, 0);
    fbo.end();
}

//--------------------------------------------------------------
void ofApp::saveCanvas(){
    string filename = "clydia_" + ofToString(ofGetUnixTime()) + ".png";
    
    ofImage img;
    ofPixels pixels;
    fbo.readToPixels(pixels);
    img.setFromPixels(pixels);
    img.save(filename);
    clearCanvas();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    animator.moveTo(ofPoint(x, y), true);
    addBranchAt(ofVec2f(x, y));
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    animator.moveTo(ofPoint(x, y), true);
    
    ofRectangle rect(camX, camY, CAM_WIDTH, CAM_HEIGHT);
    if (rect.inside(x, y)) {
        auto pixels = cam.getPixels();
        targetColor = pixels.getColor(x - camX, y - camY);
        contourFinder.setTargetColor(targetColor, trackingColorMode);
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    animator.moveTo(ofPoint(x, y), true);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

//--------------------------------------------------------------
void ofApp::addBranchAt(const ofVec2f &pos)
{
    Branch *branch = new Branch;
    branch->setup(branchColor, pos, ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));
    branch->setDrawMode(CL_BRANCH_DRAW_LEAVES);
    branches.push_back(branch);
}

//--------------------------------------------------------------
void ofApp::toggleCamera(bool & isOpen){
    if (isOpen) {
        cam.initGrabber(CAM_WIDTH, CAM_HEIGHT);
    } else {
        if (cam.isInitialized()) {
            cam.close();
        }
        ofBackground(0);
    }
}

//--------------------------------------------------------------
void ofApp::toggleColorMode(bool & isHsv){
    if (isHsv) {
        trackingColorMode = ofxCv::TRACK_COLOR_HSV;
        contourFinder.setTargetColor(targetColor, trackingColorMode);
    } else {
        trackingColorMode = ofxCv::TRACK_COLOR_RGB;
        contourFinder.setTargetColor(targetColor, trackingColorMode);
    }
}

//--------------------------------------------------------------
void ofApp::setupGui(){
    
    gui.setup();
    gui.setDefaultTextPadding(20);
    
    gui.add(drawingLabel.setup("Drawing", ""));
    gui.add(branchColor.setup("Branch Color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    gui.add(pointSpeed.setup("Speed", 0.24f, 0.01f, 0.40f));
    gui.add(pointRadiusX.setup("Radius X", 600, ofGetWindowWidth()*0.08f, ofGetWindowWidth()*0.8f));
    gui.add(pointRadiusY.setup("Radius Y", 600, ofGetHeight()*0.08f, ofGetHeight()*0.8f));
    
    
    gui.add(trackingLabel.setup("Tracking", ""));
    gui.add(bUseCamera.setup("Track with Camera", false));
    gui.add(targetColor.setup("Color to Track", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    gui.add(threshold.setup("Threshold", 15, 10, 50));
    gui.add(bUseHSV.setup("Use HSV", true));
    gui.add(camX.setup("Camera X", 0, 0, ofGetWidth()-CAM_WIDTH));
    gui.add(camY.setup("Camera Y", 0, 0, ofGetHeight()-CAM_HEIGHT));
    gui.add(bDrawDebug.setup("Draw Debug", true));
    gui.add(bDrawVideo.setup("Draw Video", true));
    gui.add(bUseDiff.setup("Use Difference", true));
    
    gui.add(generalLabel.setup("General", ""));
    gui.add(bClearOnDraw.setup("Clear on Draw", true));
    gui.add(clearBtn.setup("Clear"));
    gui.add(saveBtn.setup("Save Image"));
    
    clearBtn.addListener(this, &ofApp::clearCanvas);
    saveBtn.addListener(this, &ofApp::saveCanvas);
    bUseHSV.addListener(this, &ofApp::toggleColorMode);
    bUseCamera.addListener(this, &ofApp::toggleCamera);
    
    gui.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::exit(){
    clearBtn.removeListener(this, &ofApp::clearCanvas);
    saveBtn.removeListener(this, &ofApp::saveCanvas);
    bUseHSV.removeListener(this, &ofApp::toggleColorMode);
    bUseCamera.removeListener(this, &ofApp::toggleCamera);
}