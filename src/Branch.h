#pragma once

#include "ofMain.h"

#define CL_BRANCH_AGE_MIN           200
#define CL_BRANCH_AGE_MAX           600
#define CL_BRANCH_AGING_COEFF_MIN   2.f
#define CL_BRANCH_AGING_COEFF_MAX   6.f
#define CL_BRANCH_TAIL_LENGTH       200.f

enum clBranchLifeState
{
    CL_BRANCH_SPAWNING,
    CL_BRANCH_DEAD
};

enum clBranchDrawMode
{
    CL_BRANCH_DRAW_LEAVES
};

enum clDrawAlphaMode
{
    CL_BRANCH_DRAW_FLAT,
    CL_BRANCH_DRAW_GRADIENT
};

class Branch
{
private:
    clBranchLifeState lifeState;
    clBranchDrawMode  drawMode;
	
	ofPoint	b_pos;
	ofPoint	b_acc;
	ofPoint	b_vel;
    
    int age;
    int ageOfDeath;
    int ageCoeff;
    float theta;
    float alpha;
    
	ofColor		color;
	ofRectangle border;
    
    ofVboMesh   vboMesh;
	
public:
    ~Branch();
    
	void setup(const ofColor& color, const ofPoint& pos, const ofRectangle& b);
	void update(const float& speed, const float& diffusion, const ofColor& color, clDrawAlphaMode alphaMode);
	void draw();
    void drawVbo();
    
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
        if (lifeState == CL_BRANCH_DEAD)
            return false;
        return true;
    }
    
	
	deque<ofPtr<ofPoint>> positions;
};