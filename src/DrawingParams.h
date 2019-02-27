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
#include "Constants.h"

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
    ofParameter<bool>       bDrawBrush;
};


#endif /* DrawingParams_h */
