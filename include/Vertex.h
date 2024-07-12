#pragma once

#include "geometry.h"

struct Colour
{
    Colour();
    Colour(int r, int g, int b);
    int R, G, B;
};

class Vertex : public Vec3f
{
public:
    Vertex(float x, float y, float z);
    Vertex(Vec3f point);
    Vertex(Vec3f point, Colour colour);

    void setColour(Colour Colour);
    Colour getColour();
private:
    Colour _colour;
};