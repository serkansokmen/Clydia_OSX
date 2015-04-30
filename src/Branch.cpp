#include "Branch.h"

Branch::~Branch()
{
    for (int i=0; i<positions.size(); i++)
    {
        delete positions[i];
        positions[i] = 0;
    }
    positions.clear();
}


void Branch::setup(const ofPoint &pos, const ofRectangle &b)
{
    lifeState = CL_BRANCH_SPAWNING;
    drawMode = CL_BRANCH_DRAW_CIRCLES;
    
    age = 0;
    ageCoeff = ofRandom(CL_BRANCH_AGING_COEFF_MIN, CL_BRANCH_AGING_COEFF_MAX);
	ageOfDeath = ofRandom(CL_BRANCH_AGE_MIN, CL_BRANCH_AGE_MAX);
    
    theta = 0.0f;
	
	b_pos.set(pos);
    b_vel.set(ofRandom(-0.4, 0.4), ofRandom(-0.4, 0.4));
    b_acc.set(0.009, -0.016);
	
	border.set(b);
    
    float red = 0; //ofRandom(50, 255);
	float green = ofRandom(40, 255);
	float blue = 0; //ofRandom(0, 0);
	float alpha = ofRandom(100, 255);
	
    color.set(red, green, blue, alpha);
}


void Branch::update()
{
	switch (lifeState)
    {
        case CL_BRANCH_SPAWNING:
            if (age < CL_BRANCH_AGE_MAX)
            {
                age += ageCoeff;
                theta += 0.1;
                
                b_acc.set(ofRandom(-1, 1), ofRandom(-1, 1));
                b_acc *= ofRandom(-0.5f, 0.5f);
                b_vel += b_acc;
                b_pos += b_vel;
                
                ofPoint *current = new ofPoint;
                current->set(b_pos.x, b_pos.y);
                positions.push_back(current);
                
                // Erase from tail
                if (positions.size() > 40)
                    positions.erase(positions.begin());
                
                // Check for border bounds
                if (current->x < border.x || current->x > border.width + border.x)
                    b_vel *= -1;
                if (current->y < border.y || current->y > border.height + border.y)
                    b_vel *= -1;
            }
            else
                lifeState = CL_BRANCH_DEAD;
            break;
            
        case CL_BRANCH_DEAD:
            break;
            
        default: break;
    }
}

void Branch::draw()
{
    switch (drawMode)
    {
        case CL_BRANCH_DRAW_LEAVES:
            ofSetColor(color);
            ofSetPolyMode(OF_POLY_WINDING_NONZERO);
            ofBeginShape();
            for (int i=0; i<positions.size(); i++)
                ofVertex(positions[i]->x, positions[i]->y);
            ofEndShape(false);
            break;
            
        case CL_BRANCH_DRAW_CIRCLES:
            float alpha = ofMap(age, 0, ageOfDeath, 0, 150.0);
            float radius = ofMap(age, 0, ageOfDeath, 10.0f, 0.1f);
            ofSetColor(color, alpha);
            for (int i=0; i<positions.size(); i++)
                ofCircle(positions[i]->x, positions[i]->y, radius);
            
            break;
    }
}

