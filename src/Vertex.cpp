#include "Vertex.h"

// Constructs a black colour
Colour::Colour() : R{0}, G{0}, B{0} {}

// Constructor for a desired Colour
Colour::Colour(int red, int green, int blue) : R{red}, G{green}, B{blue} {}

Vertex::Vertex(float x, float y, float z) : Vec3f(x,y,z), _colour{Colour()} {}
Vertex::Vertex(Vec3f point) : Vec3f(point), _colour{Colour()} {}
Vertex::Vertex(Vec3f point, Colour colour) : Vec3f(point), _colour{colour} {}

void Vertex::setColour(Colour colour) { _colour = colour; }
Colour Vertex::getColour() { return _colour; }



