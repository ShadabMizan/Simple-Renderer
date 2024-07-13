#pragma once 

#include <fstream>
#include "geometry.h"
#include "Vertex.h"
#include <vector>
#include <iostream>

class SceneObject
{
public:
    SceneObject(std::string name);

    // Pure virtual function
    virtual void print(std::ostream& os) const = 0;

    friend std::ostream& operator<<(std::ostream& os, const SceneObject& sceneObj)
    {
        sceneObj.print(os);
        return os;
    }
    
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
    Cube(std::string name, Colour colour);
    
    void print(std::ostream& os) const override;

    void setColour(Colour colour);
    void setColour(uint8_t index, Colour colour);
    Colour getColour(uint8_t index);
};