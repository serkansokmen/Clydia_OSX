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


void Branch::update(const float& speed, const int& length, const float& diffusion, const ofColor& color, clDrawAlphaMode alphaMode)
{
    // Die after length
    if (positions.size() >= length) {
        lifeState = CL_BRANCH_DYING;
    }

    
    switch (alphaMode) {
        case CL_BRANCH_DRAW_FLAT:
            break;
        case CL_BRANCH_DRAW_GRADIENT:
            this->color.setBrightness(255.f - ofMap(age, CL_BRANCH_AGE_MIN, CL_BRANCH_AGE_MAX, 0.f, 255.f));
            break;
        case CL_BRANCH_DRAW_AGE_ALPHA:
            this->color.a = 255.f - ofMap(age, CL_BRANCH_AGE_MIN, CL_BRANCH_AGE_MAX, 0.f, 255.f);
            break;
        default:
            break;
    }
    
    age += ageCoeff;
    theta += speed;
    b_acc.set(ofRandomf(), ofRandomf(), 0);
    b_acc *= diffusion + 0.01f;
    b_vel += b_acc;
    b_pos += b_vel;
    ofPtr<ofPoint> current(new ofPoint(b_pos));
    positions.push_back(current);
    
    switch (lifeState)
    {
        case CL_BRANCH_SPAWNING:
            if (age < CL_BRANCH_AGE_MAX)
            {
                // Check for border bounds
                if (current->x < border.x || current->x > border.width + border.x)
                    b_vel *= -1;
                if (current->y < border.y || current->y > border.height + border.y)
                    b_vel *= -1;
            }
            else {
                lifeState = CL_BRANCH_DYING;
            }
            break;
            
        case CL_BRANCH_DYING:
        {
            if (positions.size() > length) {
                positions.pop_front();
            } else {
                lifeState = CL_BRANCH_DEAD;
            }
        }
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
        {
            vboMesh.clear();
            for (int i=0; i<positions.size(); i++) {
                
                if (i > 1) {
//                    ofVec2f prev = *positions[i-1];
//                    ofVec2f back = back - current;
                    vboMesh.addColor(color);
                    vboMesh.addVertex(ofVec3f(*positions[i]));
//                    vboMesh.addColor(color);
//                    vboMesh.addVertex(prev);
                }
            }
            vboMesh.draw();
            break;
        }
            
        default :
            break;
    }
}
