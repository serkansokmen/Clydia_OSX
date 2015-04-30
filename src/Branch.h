#pragma once

#include "ofMain.h"

const int CL_BRANCH_AGE_MIN = 100;
const int CL_BRANCH_AGE_MAX = 200;
const int CL_BRANCH_AGING_COEFF_MIN = 1;
const int CL_BRANCH_AGING_COEFF_MAX = 10;

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
	void update();
	void draw();
    
    ofVec2f getPosition() const { return b_pos; }
    int     getAge() const      { return age; }
	
    bool getIsAlive() const
    {
        if (lifeState == CL_BRANCH_DEAD)
            return false;
        return true;
    }
    
    void setDrawMode(clBranchDrawMode mode) { drawMode = mode; }
	
	vector <ofPoint*> positions;
};