//
//  PointAnimator.cpp
//  Clydia
//
//  Created by Serkan Sökmen on 17/08/15.
//
//

#include "PointAnimator.h"


void PointAnimator::setup(const ofPoint& center, const ofColor& color){
    
    this->speed = 0;
    this->radiusX = 0;
    this->radiusY = 0;
    this->angle = 0;
    this->center = center;
    
    this->color.setDuration(4);
    this->color.setColor(color);
    this->color.setRepeatType(PLAY_ONCE);
    this->color.setCurve(EASE_OUT);
    
    this->point.setPosition(center);
    this->point.setDuration(1);
    this->point.setRepeatType(PLAY_ONCE);
    this->point.setCurve(EASE_OUT_ELASTIC);
}

void PointAnimator::update(float dt){
    
    point.update(dt);
    color.update(dt);
    
    if (!point.isOrWillBeAnimating()) {
        angle = ofGetElapsedTimef() * speed;
        if (radiusX > radiusX/2) {
            radiusX -= radiusX*0.00008f;
        }
        if (radiusY > radiusY/2) {
            radiusY -= radiusY*0.00008f;
        }
        
        x = radiusX * cos(angle*4) + center.x;
        y = radiusY * sin(angle*4) + center.y;
        
        moveTo(ofPoint(x, y), false);
    }
    
}

void PointAnimator::moveTo(const ofPoint &pos, bool immediate){
    if (!immediate) {
        if (!point.isOrWillBeAnimating()){
            point.animateTo(pos);
        }
    } else {
        point.animateTo(pos);
    }
}

void PointAnimator::colorTo(const ofColor &color){
    if (!this->color.isOrWillBeAnimating()){
        this->color.animateTo(color);
    }
}

