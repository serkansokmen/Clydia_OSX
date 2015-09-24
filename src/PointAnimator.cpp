//
//  PointAnimator.cpp
//  Clydia
//
//  Created by Serkan Sökmen on 17/08/15.
//
//

#include "PointAnimator.h"


void PointAnimator::setup(const ofPoint& center){
    
    this->speed = 0;
    this->radius.set(0, 0);
    this->angle = 0;
    this->center = center;
    
    this->point.setPosition(center);
    this->point.setDuration(1);
    this->point.setRepeatType(PLAY_ONCE);
    this->point.setCurve(EASE_IN_EASE_OUT);
}

void PointAnimator::update(float dt){
    this->dt = dt;
    point.update(dt);
}

void PointAnimator::moveCircular(float diff){
    
    float radiusX = radius.x;
    float radiusY = radius.x;
    
    if (!point.isOrWillBeAnimating()) {
        angle = ofGetElapsedTimef() * speed;
        if (radiusX > radiusX/2) {
            radiusX -= radiusX*diff;
        }
        if (radiusY > radiusY/2) {
            radiusY -= radiusY*diff;
        }
        
        x = radiusX * cos(angle*4) + center.x;
        y = radiusY * sin(angle*4) + center.y;
        z = ofNoise(ofGetElapsedTimeMillis());
        
        moveTo(ofPoint(x, y, z), true);
    }
    
}

void PointAnimator::moveTo(const ofPoint &pos, bool immediate){
    if (!immediate) {
        if (!point.isOrWillBeAnimating()){
            point.animateTo(pos);
        }
    } else {
//        point.animateTo(pos);
//        point.setDuration(0);
        point.setPosition(pos);
    }
}
