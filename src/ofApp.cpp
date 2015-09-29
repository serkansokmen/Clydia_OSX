#include "ofApp.h"

#define CAM_WIDTH           320
#define CAM_HEIGHT          240
#define MAX_BRANCHES        500
#define IMAGE_DRAW_RES_X    50
#define IMAGE_DRAW_RES_Y    50
#define DIFFUSION_MIN       0.024f
#define DIFFUSION_MAX       0.16f
#define DRAG_DRAW_RES       5

//--------------------------------------------------------------
void ofApp::setup(){
    //------------------------------------------------
    // Main setup
    //------------------------------------------------
    ofBackground(0);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofEnableAntiAliasing();
    ofSetVerticalSync(false);
    ofSetFrameRate(0);
    
    //------------------------------------------------
    // Contour finder
    //------------------------------------------------
    contourFinder.setMinAreaRadius(10);
    contourFinder.setMaxAreaRadius(200);
    // wait for half a frame before forgetting something
    contourFinder.getTracker().setPersistence(15);
    // an object can move up to 32 pixels per frame
    contourFinder.getTracker().setMaximumDistance(32);
    
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
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //app timebase, to send to all animatable objects
    float dt = 1.0f / 60.0f;
    
    colorAnimator.update(dt);
    
    if (colorAnimator.color.getTargetColor() != branchColor) {
        if (bUseFlatColors) {
            colorAnimator.colorTo(ofColor::black);
        } else {
            colorAnimator.colorTo(branchColor);
        }
    }
    
    if (bUseAnimator) {
        pointAnimator.update(dt);
        pointAnimator.speed = pointSpeed;
        pointAnimator.radius.set(pointRadius);
        pointAnimator.moveCircular(pointDiff);
        addBranchAt(pointAnimator.point.getCurrentPosition(), branchColor);
    } else {
        pointAnimator.point.reset();
    }
    if (bFreeDraw) {
        mousePos.set(mouseX, mouseY, 0);
        if (bIsMouseDown) {
            addBranchAt(mousePos, branchColor);
        }
    }
    
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
            ofPoint centroid = ofxCv::toOf(contourFinder.getCentroid(i));
            ofPoint *tPos = new ofPoint;
            tPos->x = ofMap(centroid.x, 0, cam.getWidth(), 0, ofGetWidth());
            tPos->y = ofMap(centroid.y, 0, cam.getHeight(), 0, ofGetHeight());
            tPos->z = 0;
            addBranchAt(*tPos, branchColor);
        }
    }
    
    // Update branches
    for (int i=0; i<branches.size(); i++) {
        auto b = branches[i];
        if (b->getIsAlive()) {
            b->update(pointSpeed*0.1f, branchDiffusion, colorAnimator.color.getCurrentColor(),
                      bUseFlatColors ? CL_BRANCH_DRAW_GRADIENT : CL_BRANCH_DRAW_AGE_ALPHA);
        } else {
            branches.pop_back();
        }
    }
    
    // Draw to fbo
    fbo.begin();
    if (bClearOnDraw) {
        ofClear(0, 0, 0, 0);
    }
    ofEnableDepthTest();
    ofPushMatrix();
    for (auto b : branches) {
        ofSetColor(255);
        ofSetLineWidth(0.1f);
        if (bUseVbo) {
            b->drawVbo();
        } else {
            b->draw();
        }
    }
    ofPopMatrix();
    ofDisableDepthTest();
    fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
//    ofBackgroundGradient(ofColor(33,33,33), ofColor::black, OF_GRADIENT_CIRCULAR);
    ofBackground(bgColor);
    
    fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    float diff = ofNormalize(branchDiffusion, DIFFUSION_MIN, DIFFUSION_MAX);
    
    if (bDrawGui && bUseAnimator) {
        ofColor aColor(bgColor);
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
    
    if (bDrawGui && bDrawVideo && bUseCamera) {
        ofxCv::RectTracker& tracker = contourFinder.getTracker();
        ofSetColor(255);
        cam.draw(ofVec2f(camPosition), cam.getWidth(), cam.getHeight());
    }
    
    if (bDrawGui) {
        ofSetColor(255);
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
            branchColor = ofColor(ofRandom(255.f), ofRandom(255.f), ofRandom(255.f), ofRandom(55.f) + 200.f);
            break;
        case 'd':
            branchDiffusion = ofRandom(DIFFUSION_MIN, ofLerp(DIFFUSION_MIN, DIFFUSION_MAX, .1));
        case 'D':
            branchDiffusion = ofRandom(ofLerp(DIFFUSION_MIN, DIFFUSION_MAX, .5), DIFFUSION_MAX);
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
    ofColor c(bgColor);
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
//    pointAnimator.moveTo(ofPoint(x, y, ofRandomf()*100.f), true);
    
    ofRectangle rect(ofVec2f(camPosition), CAM_WIDTH, CAM_HEIGHT);
    if (rect.inside(x, y)) {
        auto pixels = cam.getPixels();
        targetColor = pixels.getColor(x - camPosition->x, y - camPosition->y);
        contourFinder.setTargetColor(targetColor, trackingColorMode);
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
    if (branches.size() > MAX_BRANCHES) {
//        ofLog() << "Max branch count reached." << endl;
        return;
    }
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
    gui.add(bFreeDraw.setup("Freehand", false));
    gui.add(bUseFlatColors.setup("Use flat colors", false));
    gui.add(branchColor.setup("Branch Color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    
    gui.add(bgColor.setup("Background Color", bUseFlatColors ? ofColor::white : ofColor::black, ofColor::black, ofColor::white));
    gui.add(bUseVbo.setup("Use VBO", false));
    gui.add(bClearOnDraw.setup("Clear on Draw", true));
    
    gui.add(pointSpeed.setup("Speed", 0.24f, 0.01f, 0.40f));
    gui.add(branchDiffusion.setup("Branch Diffusion", 0.12f, DIFFUSION_MIN, DIFFUSION_MAX));
    gui.add(pointDiff.setup("Difference on loop", 0.24f, 0.00001f, 0.99999f));
    
    gui.add(pointRadius.setup("Draw Radius", ofVec2f(200, 200), ofVec2f(10, 10), ofVec2f(ofGetHeight()-120.f, ofGetHeight()-120.f)));
    gui.add(bUseAnimator.setup("Use Animator", false));
    
    gui.add(trackingLabel.setup("Color Tracking", ""));
    gui.add(bUseCamera.setup("Track with Camera", false));
    gui.add(targetColor.setup("Color to Track", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    gui.add(threshold.setup("Threshold", 15, 10, 50));
    gui.add(bUseHSV.setup("Use HSV", true));
    gui.add(camPosition.setup("Video Grabber",
                              ofGetWindowRect().getTopLeft(),
                              ofGetWindowRect().getTopLeft(),
                              ofPoint(ofGetWindowRect().getBottomRight().x - CAM_WIDTH,
                                      ofGetWindowRect().getBottomRight().y - CAM_HEIGHT)));
    gui.add(bDrawVideo.setup("Draw Video", true));
    gui.add(bUseDiff.setup("Use Difference", true));
    
    gui.add(generalLabel.setup("General", ""));
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

