//
//  ColorAnimator.cpp
//  Clydia
//
//  Created by Serkan Sökmen on 17/08/15.
//
//

#include "ColorAnimator.h"


void ColorAnimator::setup(const ofColor& color){
    
    this->color.setDuration(4);
    this->color.setColor(color);
    this->color.setRepeatType(PLAY_ONCE);
    this->color.setCurve(EASE_OUT);
}

void ColorAnimator::update(float dt){
    
    color.update(dt);
    
//    if (!color.isOrWillBeAnimating()) {
//        colorTo(ofColor::white, false);
//    }
}

void ColorAnimator::colorTo(const ofColor &color, bool immediate){
    if (!immediate) {
        if (!this->color.isOrWillBeAnimating()){
            this->color.animateTo(color);
        }
    } else {
        this->color.animateTo(color);
    }
}
