#include "Particle.h"


Particle::Particle()
{
    //ctor
    forceAcum = new Vector();
    velocity = new Vector();
    normal = new Vector();
    this->radius = 5;
    this->numberOfAdjTriangles = 0;
}

Particle::~Particle()
{
    //dtor
}

void Particle::setPosition(float pX, float pY, float pZ, int posI, int posJ)
{
    //The position relative to the index in the mesh
    this->posI = posI;
    this->posJ = posJ;

    //position variables
    position = new Vector(pX, pY, pZ);
    prevPosition = new Vector(pX, pY, pZ);
}

void Particle::clearForces()
{
    forceAcum->x = 0.0f;
    forceAcum->y = 0.0f;
    forceAcum->z = 0.0f;
}

void Particle::addForce(float x, float y, float z)
{
    forceAcum->x += x;
    forceAcum->y += y;
    forceAcum->z += z;
}

void Particle::render()
{
    glPointSize((int)radius);

    if (isFixed)
        glColor3f(0, 0, 1);
    else
        glColor3f(1, 1, 1);

    glBegin(GL_POINTS);
    glVertex3f(position->x, position->y, position->z);
    glEnd();
}

Vector* Particle::verletIntegrationToPosition()
{
    Vector* nextPos = new Vector();

    nextPos->x = (2 * position->x) - prevPosition->x + forceAcum->x * 0.1f * 0.1f;
    nextPos->y = (2 * position->y) - prevPosition->y + forceAcum->y * 0.1f * 0.1f;
    nextPos->z = (2 * position->z) - prevPosition->z + forceAcum->z * 0.1f * 0.1f;

    return nextPos;
}

void Particle::update(int sphereRadius, float* objPos)
{
    velocity->x = forceAcum->x * 0.1f;
    velocity->y = forceAcum->y * 0.1f;
    velocity->z = forceAcum->z * 0.1f;

    float tempX = position->x;
    float tempY = position->y;
    float tempZ = position->z;

    Vector* nextPos = verletIntegrationToPosition();

    if (!isFixed)
    {
        position->x = nextPos->x;
        position->y = nextPos->y;
        position->z = nextPos->z;
    }

    prevPosition->x = tempX;
    prevPosition->y = tempY;
    prevPosition->z = tempZ;

    checkCollisionWithSphere(sphereRadius, objPos);
    //checkCollisionWithCapsule(p0, p1, cylinderRadius);
}

void Particle::checkCollisionWithCapsule(Vector p0, Vector p1, float radius)
{
    Vector* A = new Vector();
    Vector::substract(A, p0, p1);

    Vector* D = new Vector();
    Vector::substract(D, p0, *(position));

    float d = Vector::dotProduct(*D, *A);

    if (d < 0)
    {
        d = 0;
    }
    else if (d > A->getLength())
    {
        d = A->getLength();
    }

    Vector* aD = new Vector();
    Vector::multiplicationByScalar(aD, *A, d);

    Vector* R = new Vector();
    R->x = p0.x + aD->x;
    R->y = p0.y + aD->y;
    R->z = p0.z + aD->z;

    Vector* distHR = new Vector();
    Vector::substract(distHR, *R, *position);

    float b = distHR->getLength();

    if (b < (this->radius + radius))
    {
        float p = b - (this->radius + radius);

        Vector* N = new Vector();
        N->x = distHR->x;
        N->y = distHR->y;
        N->z = distHR->z;
        N->normalize();

        Vector* P = new Vector();
        Vector::multiplicationByScalar(P, *N, p);

        this->position->x += P->x;
        this->position->y += P->y;
        this->position->z += P->z;
    }
}

void Particle::checkCollisionWithSphere(int radius, float* objPos)
{
    float penDist;
    Vector* spherePos = new Vector(objPos[0], objPos[1], objPos[2]);
    Vector* dist = new Vector();
    Vector::substract(dist, *position, *spherePos);
    penDist = dist->getLength() - (radius + this->radius);

    dist->normalize();

    Vector* P = new Vector();
    Vector::multiplicationByScalar(P, *dist, penDist);

    if (penDist < 0)
    {
        this->position->x += P->x;
        this->position->y += P->y;
        this->position->z += P->z;
    }
}