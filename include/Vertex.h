#pragma once

#include "geometry.h"
#include <iostream>

struct Colour : public Vec3<unsigned char>
{
    Colour();
    Colour(unsigned char c);
    // Here, xyz values of the vector are used as rgb respectively.
    Colour(unsigned char r, unsigned char g, unsigned char b);

    static const Colour RED;
    static const Colour GREEN;
    static const Colour BLUE;

    friend std::ostream& operator<<(std::ostream& os, const Colour& colour)
    {
        os << "(" << colour.x << ", " << colour.y << ", " << colour.z << ")";
        return os;
    }
};

class Vertex : public Vec3f
{
public:
    Vertex();
    Vertex(float x, float y, float z);
    Vertex(Vec3f point);
    Vertex(Vec3f point, Colour colour);
    
    Colour colour;
};