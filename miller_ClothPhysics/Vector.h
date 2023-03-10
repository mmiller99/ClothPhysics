#pragma once
class Vector
{
public:

    float x;
    float y;
    float z;

    Vector();
    Vector(float x, float y, float z);
    virtual ~Vector();

    float getLength();

    //This method will normalize the current vector
    void normalize();

    //The following methods places the computed result in *res
    static void substract(Vector* res, Vector v1, Vector v2);
    static void crossProduct(Vector* res, Vector v1, Vector v2);
    static float dotProduct(Vector v1, Vector v2);
    static void multiplicationByScalar(Vector* res, Vector v1, float scalarValue);
};

