#pragma once

#include "geometry.h"
#include <iostream>

struct Colour
{
    Colour();
    Colour(int r, int g, int b);
    int R, G, B;

    friend std::ostream& operator<<(std::ostream& os, const Colour& colour)
    {
        os << "(" << colour.R << ", " << colour.G << ", " << colour.B << ")";
        return os;
    }
};

class Vertex : public Vec3f
{
public:
    Vertex(float x, float y, float z);
    Vertex(Vec3f point);
    Vertex(Vec3f point, Colour colour);

    void setColour(Colour Colour);
    const Colour getColour() const;
private:
    Colour _colour;
};