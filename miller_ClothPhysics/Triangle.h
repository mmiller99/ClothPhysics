#pragma once
#include "Particle.h"
class Triangle
{
public:
    Particle* particles[3];
    Vector* normal;

    Triangle();
    virtual ~Triangle();

    void setTriangle(Particle* p1, Particle* p2, Particle* p3);
    void calculateNormal();
    void render(int resolution, bool showWireframe, bool flatShade);
};

