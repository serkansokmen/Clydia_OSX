#include "Branch.h"

Branch::~Branch()
{
    positions.clear();
    vboMesh.clear();
}


void Branch::setup(const ofColor& color, const ofPoint&pos, const ofRectangle&b)
{
    lifeState = CL_BRANCH_SPAWNING;
    drawMode = CL_BRANCH_DRAW_LEAVES;
    
    age = 0;
    ageCoeff = ofRandom(CL_BRANCH_AGING_COEFF_MIN, CL_BRANCH_AGING_COEFF_MAX);
	ageOfDeath = ofRandom(CL_BRANCH_AGE_MIN, CL_BRANCH_AGE_MAX);
    
    theta = 0.0f;
	
	b_pos.set(pos);
    b_vel.set(ofRandomf()*0.1f, ofRandomf()*0.1f, ofRandomf()*0.1f);
    b_acc.set(0.009, -0.016, ofRandomf()*100.f);
	
	border.set(b);
    
    this->color.set(color);
    
    vboMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
}


void Branch::update(const float& speed, const float& diffusion, const ofColor& color, clDrawAlphaMode alphaMode)
{
    // Erase from tail
    if (positions.size() >= CL_BRANCH_TAIL_LENGTH) {
        positions.pop_back();
    }
    
    switch (alphaMode) {
        case CL_BRANCH_DRAW_FLAT:
            break;
        case CL_BRANCH_DRAW_GRADIENT:
            alpha = ofMap(age, CL_BRANCH_AGE_MIN, CL_BRANCH_AGE_MAX, color.a, 0);
            this->color.a = alpha;
            break;
        default:
            break;
    }

	switch (lifeState)
    {
        case CL_BRANCH_SPAWNING:
            if (age < CL_BRANCH_AGE_MAX)
            {
                age += ageCoeff;
                theta += speed;
                
                b_acc.set(ofRandomf(), ofRandomf(), ofRandomf());
                b_acc *= ofRandomf()*diffusion + 0.01f;
                b_vel += b_acc;
                b_pos += b_vel;
                
                ofPtr<ofPoint> current(new ofPoint(b_pos));
                positions.push_back(current);
                
                // Check for border bounds
                if (current->x < border.x || current->x > border.width + border.x)
                    b_vel *= -1;
                if (current->y < border.y || current->y > border.height + border.y)
                    b_vel *= -1;
            }
            else {
                if (positions.size() > 0) {
                    positions.pop_front();
                } else {
                    lifeState = CL_BRANCH_DEAD;
                }
                
            }
            break;
            
        case CL_BRANCH_DEAD:
            positions.clear();
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
            ofSetPolyMode(OF_POLY_WINDING_NONZERO);
            ofBeginShape();
            ofEnableDepthTest();
            for (auto p : positions) {
                ofSetColor(color);
                ofVertex(p->x, p->y, p->z);
            }
            ofDisableDepthTest();
            ofEndShape(false);
            break;
        }
            
        default :
            break;
    }
    ofPopStyle();
}

void Branch::drawVbo()
{
    switch (drawMode)
    {
        case CL_BRANCH_DRAW_LEAVES:
        {
            vboMesh.clear();
            for (auto p : positions) {
                vboMesh.addColor(color);
                vboMesh.addVertex(*p);
            }
            vboMesh.draw();

            break;
        }
            
        default :
            break;
    }
}

