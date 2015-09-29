#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    //------------------------------------------------
    // Main setup
    //------------------------------------------------
    ofBackground(0);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    this->mousePos = ofPoint(mouseX, mouseY, 0);
    
    setupGui();
    bDrawGui = true;
    
    // Setup pointAnimator point
    pointAnimator.setup(ofGetWindowRect().getCenter());
    colorAnimator.setup(ofColor::black);
    
    ofFbo::Settings settings;
    settings.numSamples = 4; // also try 8, if your GPU supports it
    settings.useDepth = true;
    settings.width = ofGetWidth();
    settings.height = ofGetHeight();
    fbo.allocate(settings);
    
    clearCanvas();
    
    
    //------------------------------------------------
    // Contour finder
    //------------------------------------------------
    contourFinder.setMinAreaRadius(10);
    contourFinder.setMaxAreaRadius(200);
    // wait for half a frame before forgetting something
    contourFinder.getTracker().setPersistence(15);
    // an object can move up to 32 pixels per frame
    contourFinder.getTracker().setMaximumDistance(32);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // app timebase, to send to all animatable objects
    float dt = 1.0f / 60.0f;
    
    ofLog() << branches.size() << endl;
    
    colorAnimator.update(dt);
    
    if (colorAnimator.color.getTargetColor() != drawingParams.branchColor) {
        if (drawingParams.bUseFlatColors) {
            colorAnimator.colorTo(ofColor::black);
        } else {
            colorAnimator.colorTo(drawingParams.branchColor);
        }
    }
    
    if (animatorParams.enabled) {
        pointAnimator.update(dt);
        pointAnimator.speed = animatorParams.pointSpeed;
        pointAnimator.radius.set(animatorParams.pointRadius);
        pointAnimator.moveCircular();
        addBranchAt(pointAnimator.point.getCurrentPosition(), drawingParams.branchColor);
    } else {
        pointAnimator.point.reset();
    }
    
    if (drawingParams.bFreeDraw) {
        mousePos.set(mouseX, mouseY, 0);
        if (bIsMouseDown) {
            addBranchAt(mousePos, drawingParams.branchColor);
        }
    }
    
    if (trackingParams.enabled && cam.isInitialized()) {
        cam.update();
        if (cam.isFrameNew()) {
            
            ofxCv::blur(cam, 25);
            contourFinder.setThreshold(trackingParams.threshold);
            contourFinder.findContours(cam);
            
            // Update tracked positions
            int n = contourFinder.size();
            for(int i = 0; i < n; i++) {
                ofPoint centroid = ofxCv::toOf(contourFinder.getCentroid(i));
                ofPoint *tPos = new ofPoint;
                tPos->x = ofMap(centroid.x, 0, cam.getWidth(), 0, ofGetWidth());
                tPos->y = ofMap(centroid.y, 0, cam.getHeight(), 0, ofGetHeight());
                tPos->z = 0;
                addBranchAt(*tPos, drawingParams.branchColor);
            }
        }
    }
    
    // Update branches
    for (int i=0; i<branches.size(); i++) {
        auto b = branches[i];
        
        if (b->getIsAlive()) {
            b->update(animatorParams.pointSpeed, drawingParams.branchLength, drawingParams.branchDiffusion, colorAnimator.color.getCurrentColor(),
                      drawingParams.bUseFlatColors ? CL_BRANCH_DRAW_GRADIENT : CL_BRANCH_DRAW_AGE_ALPHA);
        } else {
            branches.pop_back();
        }
        
        if (i > drawingParams.maxBranches) {
            b->die();
        }
    }
    
    // Draw to fbo
    fbo.begin();
    if (drawingParams.bClearOnDraw) {
        ofClear(0, 0, 0, 0);
    }
    ofEnableDepthTest();
    ofPushMatrix();
    for (auto b : branches) {
        ofSetColor(255);
        ofSetLineWidth(0.1f);
        b->draw();
    }
    ofPopMatrix();
    ofDisableDepthTest();
    fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    if (bDrawGui) {
        
        if (animatorParams.enabled) {
            float diff = ofNormalize(drawingParams.branchDiffusion, DIFFUSION_MIN, DIFFUSION_MAX);
            ofColor aColor(drawingParams.bgColor);
            ofPushStyle();
            ofSetColor(255.f-aColor.r, 255.f-aColor.g, 255.f-aColor.b);
            ofNoFill();
            ofSetCircleResolution(100.f);
            ofSetLineWidth(diff);
            ofDrawCircle(pointAnimator.point.getCurrentPosition(), 10.f + diff*25.f);
            ofSetColor(255.f-aColor.r, 255.f-aColor.g, 255.f-aColor.b, 20);
            ofFill();
            ofDrawCircle(pointAnimator.point.getCurrentPosition(), 10.f + diff*24.f);
            ofPopStyle();
        }
        
        if (trackingParams.enabled) {
            ofxCv::RectTracker& tracker = contourFinder.getTracker();
            cam.draw(trackingParams.camPosition.get(), cam.getWidth(), cam.getHeight());
        }
        
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key){
        case ' ':
            bDrawGui = !bDrawGui;
            break;
        case 'c':
            drawingParams.branchColor = ofColor(ofRandom(255.f), ofRandom(255.f), ofRandom(255.f), ofRandom(55.f) + 200.f);
            break;
        case 'd':
            drawingParams.branchDiffusion = ofRandom(DIFFUSION_MIN, ofLerp(DIFFUSION_MIN, DIFFUSION_MAX, .1));
        case 'D':
            drawingParams.branchDiffusion = ofRandom(ofLerp(DIFFUSION_MIN, DIFFUSION_MAX, .5), DIFFUSION_MAX);
            break;
        case 'C':
            clearCanvas();
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
    pointAnimator.moveTo(ofGetWindowRect().getCenter(), true);
    branches.clear();
    ofColor c(drawingParams.bgColor);
    fbo.begin();
    ofClear(c.r, c.g, c.b, 0);
    fbo.end();
}

//--------------------------------------------------------------
void ofApp::saveCanvas(){
    string filename = "Frames/clydia_" + ofToString(ofGetUnixTime()) + ".png";
    
    ofImage img;
    ofPixels pixels;
    fbo.readToPixels(pixels);
    img.setFromPixels(pixels);
    img.save(filename);
    clearCanvas();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    bIsMouseDown = true;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    bIsMouseDown = true;
    
    if (trackingParams.enabled) {
        ofRectangle rect(ofVec2f(trackingParams.camPosition), CAM_WIDTH, CAM_HEIGHT);
        if (rect.inside(x, y)) {
            auto pixels = cam.getPixels();
            trackingParams.targetColor = pixels.getColor(x - trackingParams.camPosition->x, y - trackingParams.camPosition->y);
            contourFinder.setTargetColor(trackingParams.targetColor, trackingColorMode);
        }
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    bIsMouseDown = false;
//    pointAnimator.moveTo(ofPoint(x, y, ofRandomf()*100.f), true);
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
    
    clearCanvas();
    
    ofPoint pos(ofGetWindowRect().getCenter());
    
    if( dragInfo.files.size() > 0 ){
        for(unsigned int k = 0; k < dragInfo.files.size(); k++){
            ofImage img;
            if (img.load(dragInfo.files[k])) {
                pos.x -= img.getWidth()/2;
                pos.y -= img.getHeight()/2;
                addBranchesFromImage(img, pos);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::addBranchesFromImage(const ofImage& image, const ofVec2f& pos) {
    
    ofLog() << "Adding images" << endl;
    ofLog() << "width: " << image.getWidth() << endl;
    ofLog() << "height: " << image.getHeight() << endl;
    
    for (int i = 0; i < image.getWidth(); i += IMAGE_DRAW_RES_X){
        for (int j = 0; j < image.getHeight(); j += IMAGE_DRAW_RES_Y){
            auto c = image.getColor(i, j);
            if (c.getBrightness() > 5.f) {
                addBranchAt(pos + ofVec2f(i, j), image.getColor(i, j));
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::addBranchAt(const ofPoint& pos, const ofColor& color)
{
    ofPtr<Branch> branch(new Branch());
    branch->setup(color, pos, ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));
    branch->setDrawMode(CL_BRANCH_DRAW_LEAVES);
    branches.push_front(branch);
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
        contourFinder.setTargetColor(trackingParams.targetColor, trackingColorMode);
    } else {
        trackingColorMode = ofxCv::TRACK_COLOR_RGB;
        contourFinder.setTargetColor(trackingParams.targetColor, trackingColorMode);
    }
}

//--------------------------------------------------------------
void ofApp::setupGui(){
    
    animatorParams.setup("ANIMATOR");
    drawingParams.setup("DRAWING");
    trackingParams.setup("COLOR TRACKING");
    
    parameters.setName("CLYD");
    parameters.add(drawingParams.parameters);
    parameters.add(animatorParams.parameters);
    parameters.add(trackingParams.parameters);
    
    gui.setup(parameters);
    gui.add(clearBtn.setup("Clear"));
    gui.add(saveBtn.setup("Save"));
    
    //    gui.setDefaultTextPadding(40);
    
    trackingParams.enabled.addListener(this, &ofApp::toggleCamera);
    trackingParams.bUseHSV.addListener(this, &ofApp::toggleColorMode);
    clearBtn.addListener(this, &ofApp::clearCanvas);
    saveBtn.addListener(this, &ofApp::saveCanvas);
    
    gui.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::exit(){
    trackingParams.enabled.removeListener(this, &ofApp::toggleCamera);
    trackingParams.bUseHSV.removeListener(this, &ofApp::toggleColorMode);
    clearBtn.removeListener(this, &ofApp::clearCanvas);
    saveBtn.removeListener(this, &ofApp::saveCanvas);
}

