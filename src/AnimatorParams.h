//
//  AnimatorParams.h
//  Clydia
//
//  Created by Serkan Sokmen on 30/09/15.
//
//

#ifndef AnimatorParams_h
#define AnimatorParams_h

#include "ofParameterGroup.h"
#include "ofParameter.h"
#include "ofMain.h"

class AnimatorParams;

template<typename ParameterType>
class ofAnimatorParamsParam: public ofReadOnlyParameter<ParameterType,AnimatorParams>{
    friend class AnimatorParams;
};

class AnimatorParams {
public:
    AnimatorParams();
    
    void setup(string name);
    void draw();
    
    ofParameterGroup        parameters;
    
    ofParameter<ofVec2f>    pointRadius;
    ofParameter<double>     pointSpeed;
    ofParameter<bool>       enabled;
};


#endif /* AnimatorParams_h */
