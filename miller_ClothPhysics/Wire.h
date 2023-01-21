#pragma once
#include "Triangle.h"
#include "Particle.h"
#include "Vector.h"
#include "Spring.h"

class Wire
{
public:
    Particle** particles;
    int numberOfParticles;

    Triangle** triangles;
    int numberOfTriangles;

    Spring** springs;
    int numberOfSprings;

    float delta;
    int resolution;

    Wire(int resolution, int size);
    virtual ~Wire();

    void addParticle(Particle* particle, int i, int j);
    void generateTriangles(int resolution);
    void renderParticles();
    void renderTriangles(bool showWireframe, bool flatShade);
    void renderSprings();
    void update(int sphereRadius, float gravityForce, Vector gravityDirection, float ks, float kd, float particleRadius, float* objPos);
    void generateSprings();
    void calculateParticleNormal(int c, int r);
};

