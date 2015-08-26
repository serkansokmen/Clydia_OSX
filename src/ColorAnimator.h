//
//  ColorAnimator.hpp
//  Clydia
//
//  Created by Serkan Sökmen on 17/08/15.
//
//

#pragma once

#include "ofMain.h"
#include "ofxAnimatableOfColor.h"


class ColorAnimator {
    
public:
    void setup(const ofColor& center);
    void update(float dt);
    void colorTo(const ofColor& color, bool immediate=true);
    
    ofxAnimatableOfColor    color;
};


