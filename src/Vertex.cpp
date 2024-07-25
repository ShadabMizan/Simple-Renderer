#include "Vertex.h"

// Constructs a black colour
Colour::Colour() : Vec3<unsigned char>() {}

Colour::Colour(unsigned char c) : Vec3<unsigned char>(c) {}
// Constructor for a desired Colour
Colour::Colour(unsigned char red, unsigned char green, unsigned char blue) : Vec3<unsigned char>(red, green, blue) {}

const Colour Colour::RED = Colour(255,0,0);
const Colour Colour::GREEN = Colour(0, 255, 0);
const Colour Colour::BLUE = Colour(0, 0, 255);

Vertex::Vertex() : Vec3f() {}
Vertex::Vertex(float x, float y, float z) : Vec3f(x,y,z), colour{Colour()} {}
Vertex::Vertex(Vec3f point) : Vec3f(point), colour{Colour()} {}
Vertex::Vertex(Vec3f point, Colour colour) : Vec3f(point), colour{colour} {}