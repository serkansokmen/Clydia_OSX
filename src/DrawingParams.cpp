//
//  DrawingParams.cpp
//  Clydia
//
//  Created by Serkan Sokmen on 30/09/15.
//
//

#include "DrawingParams.h"


DrawingParams::DrawingParams(){    
}

void DrawingParams::setup(string name){
    parameters.setName(name);
    
    parameters.add(bDrawBrush.set("Draw brush", true));
    parameters.add(maxBranches.set("Total branches", 250, 1, 500));
    parameters.add(bUseFlatColors.set("Flat colors", true));
    parameters.add(bFreeDraw.set("Freehand", false));
    parameters.add(branchLength.set("Branch length", 1, 1, 400));
    parameters.add(branchDiffusion.set("Branch diffusion", 0.08, DIFFUSION_MIN, DIFFUSION_MAX));
    parameters.add(bClearOnDraw.set("Clear on Draw", true));
    parameters.add(branchColor.set("Branch color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    parameters.add(bgColor.set("BG color", bUseFlatColors ? ofColor::white : ofColor::black, ofColor::black, ofColor::white));
}

void DrawingParams::draw(){

}
