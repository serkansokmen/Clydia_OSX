#include "Branch.h"

Branch::~Branch()
{
    for (auto p : positions) {
        delete p;
        p = 0;
    }
    positions.clear();
}


void Branch::setup(const ofColor &color, const ofPoint &pos, const ofRectangle &b)
{
    lifeState = CL_BRANCH_SPAWNING;
    drawMode = CL_BRANCH_DRAW_LEAVES;
    
    age = 0;
    ageCoeff = ofRandom(CL_BRANCH_AGING_COEFF_MIN, CL_BRANCH_AGING_COEFF_MAX);
	ageOfDeath = ofRandom(CL_BRANCH_AGE_MIN, CL_BRANCH_AGE_MAX);
    
    theta = 0.0f;
	
	b_pos.set(pos);
    b_vel.set(ofRandomf()*0.1f, ofRandomf()*0.1f);
    b_acc.set(0.009, -0.016);
	
	border.set(b);
    
    float alpha = ofMap(ageOfDeath,
                        CL_BRANCH_AGE_MIN, CL_BRANCH_AGE_MAX,
                        10.f, 255.f);
	
    this->color.set(color);
    this->color.a = alpha;
}


void Branch::update(const float &speed)
{
	switch (lifeState)
    {
        case CL_BRANCH_SPAWNING:
            if (age < CL_BRANCH_AGE_MAX)
            {
                age += ageCoeff;
                theta += speed;
                
                b_acc.set(ofRandomf(), ofRandomf());
                b_acc *= ofRandomf()*0.1f;
                b_vel += b_acc;
                b_pos += b_vel;
                
                ofPoint *current = new ofPoint;
                current->set(b_pos.x, b_pos.y);
                positions.push_back(current);
                
                // Erase from tail
                if (positions.size() > CL_BRANCH_TAIL_LENGTH)
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
    ofPushStyle();
    switch (drawMode)
    {
        case CL_BRANCH_DRAW_LEAVES:
        {
            ofFill();
            ofSetColor(color);
            ofSetPolyMode(OF_POLY_WINDING_NONZERO);
            ofBeginShape();
            for (auto p : positions) {
                ofVertex(p->x, p->y);
            }
            ofEndShape(false);
            break;
        }
            
        case CL_BRANCH_DRAW_CIRCLES:
        {
            float alpha = ofMap(age, 0, ageOfDeath, 0, 150.0);
            float radius = ofMap(age, 0, ageOfDeath, 10.0f, 0.1f);
            ofFill();
            ofSetColor(color, alpha);
            for (auto p : positions) {
                ofDrawCircle(p->x, p->y, radius);
            }
            break;
        }
            
        default :
            break;
    }
    ofPopStyle();
}

