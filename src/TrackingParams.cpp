//
//  TrackingParams.cpp
//  Clydia
//
//  Created by Serkan Sokmen on 30/09/15.
//
//

#include "TrackingParams.h"


TrackingParams::TrackingParams(){    
}

void TrackingParams::setup(string name){
    parameters.setName(name);
    
    parameters.add(enabled.set("COLOR TRACKER", false));
    parameters.add(threshold.set("Threshold", 15.0, 0.0, 50.0));
    parameters.add(bUseHSV.set("HSV", true));
    parameters.add(targetColor.set("Color to track", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    
    parameters.add(camPosition.set("Video draw position",
                                   ofGetWindowRect().getTopLeft(),
                                   ofGetWindowRect().getTopLeft(),
                                   ofPoint(ofGetWindowRect().getBottomRight().x - CAM_WIDTH,
                                           ofGetWindowRect().getBottomRight().y - CAM_HEIGHT)));
}

void TrackingParams::draw(){

}
