//
//  DrawingParams.h
//  Clydia
//
//  Created by Serkan Sokmen on 30/09/15.
//
//

#ifndef DrawingParams_h
#define DrawingParams_h

#include "ofParameterGroup.h"
#include "ofParameter.h"
#include "ofMain.h"

#define DIFFUSION_MIN       0.024
#define DIFFUSION_MAX       0.16

class DrawingParams;

template<typename ParameterType>
class ofDrawingParamsParam: public ofReadOnlyParameter<ParameterType,DrawingParams>{
    friend class DrawingParams;
};

class DrawingParams {
public:
    DrawingParams();
    
    void setup(string name);
    void draw();
    
    ofParameterGroup        parameters;
    ofParameter<ofColor>    branchColor;
    ofParameter<ofColor>    bgColor;
    ofParameter<int>        maxBranches;
    ofParameter<int>        branchLength;
    ofParameter<double>     branchDiffusion;
    ofParameter<bool>       bFreeDraw;
    ofParameter<bool>       bUseFlatColors;
    ofParameter<bool>       bClearOnDraw;
};


#endif /* DrawingParams_h */
