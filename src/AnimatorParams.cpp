//
//  AnimatorParams.cpp
//  Clydia
//
//  Created by Serkan Sokmen on 30/09/15.
//
//

#include "AnimatorParams.h"


AnimatorParams::AnimatorParams(){    
}

void AnimatorParams::setup(string name){
    parameters.setName(name);
    
    parameters.add(enabled.set("ANIMATOR", false));
    parameters.add(pointSpeed.set("Speed", 0.24, 0.0, 0.40));
    parameters.add(pointRadius.set("Draw radius", ofVec2f(200, 200), ofVec2f(10, 10), ofVec2f(ofGetHeight()-120.f, ofGetHeight()-120.f)));
}

void AnimatorParams::draw(){

}
