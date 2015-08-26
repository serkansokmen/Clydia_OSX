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


class PointAnimator {
    
    float x;
    float y;
    
public:
    void setup(const ofPoint& center);
    void update(float dt, float diff);
    void moveTo(const ofPoint& pos, bool immediate=true);
    
    ofPoint                 center;
    ofVec2f                 radius;
    ofxAnimatableOfPoint    point;
    
    float       speed;
    float       angle;
};


