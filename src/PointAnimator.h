//
//  PointAnimator.hpp
//  Clydia
//
//  Created by Serkan Sökmen on 17/08/15.
//
//

#pragma once

#include "ofMain.h"
#include "ofxAnimatableOfPoint.h"
#include "ofxAnimatableOfColor.h"


class PointAnimator {
    
    float x;
    float y;
    
public:
    void setup(const ofPoint& center, const ofColor& color);
    void update(float dt);
    void moveTo(const ofPoint& pos, bool immediate=true);
    void colorTo(const ofColor& color);
    
    ofxAnimatableOfColor    color;
    ofPoint                 center;
    ofxAnimatableOfPoint    point;
    
    float       speed;
    float       angle;
    float       radiusX;
    float       radiusY;
};


