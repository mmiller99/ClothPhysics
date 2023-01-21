#pragma once
#include "Vector.h"
#include "Particle.h"

class Spring
{
public:
    Particle* p1;
    Particle* p2;
    float ks; 
    float kd; 
    float normalLength;

    Spring();
    Spring(Particle* p1, Particle* p2, float normalLength);
    virtual ~Spring();

    void applyForce();
    void update(float ks, float kd);
    void render();
    void applySpringForce();
    float getLength();
};

