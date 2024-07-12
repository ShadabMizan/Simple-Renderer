#pragma once 

#include <fstream>
#include "geometry.h"
#include "Vertex.h"
#include <vector>
#include <unordered_map>

class SceneObject
{
public:
    SceneObject(std::string name);
    void print();

protected:
    std::string _name;
    std::vector<Vertex> _vertices;

private:
    std::ifstream _inFile;
};

class Cube : public SceneObject
{
public:
    Cube(std::string name);
    
    void setColour(uint8_t index, Colour colour);
    Colour getColour(uint8_t index);
private:

};