#pragma once

#include "ofMain.h"
#include "Constants.h"

enum clBranchLifeState
{
    CL_BRANCH_SPAWNING,
    CL_BRANCH_DYING,
    CL_BRANCH_DEAD
};

enum clBranchDrawMode
{
    CL_BRANCH_DRAW_LEAVES
};

enum clDrawAlphaMode
{
    CL_BRANCH_DRAW_FLAT,
    CL_BRANCH_DRAW_GRADIENT,
    CL_BRANCH_DRAW_AGE_ALPHA
};

class Branch
{
private:
    
    ofColor		color;
    ofRectangle border;
    ofVboMesh   vboMesh;
    
    ofPoint	b_pos;
	ofPoint	b_acc;
	ofPoint	b_vel;
    
    clBranchLifeState lifeState;
    clBranchDrawMode  drawMode;
    
    float   theta;
    float   speed;
    float   diffusion;
    
    int     age;
    int     ageOfDeath;
    int     ageCoeff;
	
public:
    ~Branch();
    
	void setup(const ofColor& color, const ofPoint& pos, const ofRectangle& b);
	void update(const float& speed, const int& length, const float& diffusion, const ofColor& color, clDrawAlphaMode alphaMode);
	void draw();
    inline void die() {
        this->lifeState = CL_BRANCH_DYING;
    }
    
    inline ofPoint getPosition() const {
        return b_pos;
    }
    inline int getAge() const      {
        return age;
    }
    inline void setDrawMode(clBranchDrawMode mode) {
        drawMode = mode;
    }
    inline bool getIsAlive() const
    {
        return lifeState == CL_BRANCH_SPAWNING || lifeState == CL_BRANCH_DYING;
    }
    
	
	deque<ofPtr<ofPoint>> positions;
};
