#pragma once

#include "ofMain.h"

const int CL_BRANCH_AGE_MIN = 80;
const int CL_BRANCH_AGE_MAX = 800;
const int CL_BRANCH_AGING_COEFF_MIN = 2.f;
const int CL_BRANCH_AGING_COEFF_MAX = 6.f;
const float CL_BRANCH_TAIL_LENGTH = 64.f;

enum clBranchLifeState
{
    CL_BRANCH_SPAWNING,
    CL_BRANCH_DEAD
};

enum clBranchDrawMode
{
    CL_BRANCH_DRAW_LEAVES,
    CL_BRANCH_DRAW_CIRCLES
};

class Branch
{
private:
    clBranchLifeState lifeState;
    clBranchDrawMode  drawMode;
	
	ofVec2f	b_pos;
	ofVec2f	b_acc;
	ofVec2f	b_vel;
    
    int age;
    int ageOfDeath;
    int ageCoeff;
    float theta;
    
	ofColor		color;
	ofRectangle border;
	
public:
    ~Branch();
    
	void setup(const ofPoint &pos, const ofRectangle &b);
	void update(const float &speed);
	void draw();
    
    inline ofVec2f getPosition() const {
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
    
	
	vector <ofPoint*> positions;
};