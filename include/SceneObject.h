#pragma once 

#include <fstream>
#include "geometry.h"
#include "Vertex.h"
#include <vector>
#include <iostream>
#include <memory>

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
    
    std::vector<std::vector< std::shared_ptr<Vertex> >> triangles;

protected:
    // Shared pointers so that we can change the properties of every vertex from this one dimensional vector 
    // to reflect in every triangle made from that vertex
    std::vector<std::shared_ptr<Vertex>> vertices;

private:
    std::ifstream _inFile;
    std::string _name;
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