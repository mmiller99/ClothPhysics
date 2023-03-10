#include "Triangle.h"


Triangle::Triangle()
{
    //ctor
    normal = new Vector();
}

Triangle::~Triangle()
{
    //dtor
}

void Triangle::setTriangle(Particle* p1, Particle* p2, Particle* p3)
{
    particles[0] = p1;
    particles[1] = p2;
    particles[2] = p3;

}

void Triangle::render(int resolution, bool showWireframe, bool flatShade)
{
    float uOffset = 1.0 / (resolution - 1);
    float vOffset = 1.0 / (resolution - 1);

    calculateNormal();
    //Watch out for the
    //Draw the triangles
    //Beware of the orientation of the triangles
    if (showWireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (flatShade)
    {
        glNormal3f(normal->x, normal->y, normal->z);
    }

    glBegin(GL_POLYGON);
    for (int i = 0; i < 3; i++)
    {
        glTexCoord2f(particles[i]->posI * uOffset, (resolution - 1 - particles[i]->posJ) * vOffset);
        if (!flatShade)
        {
            glNormal3f(particles[i]->normal->x, particles[i]->normal->y, particles[i]->normal->z);
        }
        glVertex3f(particles[i]->position->x, particles[i]->position->y, particles[i]->position->z);
    }
    glEnd();
}

void Triangle::calculateNormal()
{
    Vector* v1 = new Vector();
    Vector* v2 = new Vector();

    //substract is an static method so we call it with the :: notation
    Vector::substract(v1, *particles[1]->position, *particles[0]->position);
    Vector::substract(v2, *particles[2]->position, *particles[0]->position);

    Vector::crossProduct(normal, *v1, *v2);
}