#include "SceneObject.h"
#include "geometry.h"
#include <fstream>
#include <sstream>
#include <iostream>

const std::string OBJ_FILE = "../data/blocks.obj";

SceneObject::SceneObject(std::string name) : _name{name}
{
    // Verify the object exists in the file path name
    try
    {
        _inFile.open(OBJ_FILE);
        if (!_inFile.is_open())
        {
            std::cerr << "Could not open file" << std::endl;
        }
    } catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    std::string line, temp;
    bool objLocated = false;
    // Parse the obj file for the object we want to get data from
    while (std::getline(_inFile, line))
    {
        std::istringstream iss{line};
        iss >> temp;

        // Object declaration started
        if (temp == "o")
        {
            if (objLocated)
            {
                // Break out of reading the file since we hit another object's data now.
                _inFile.close();
                break;
            }

            iss >> temp;
            if (temp == name)
            {
                // Found our target object to get data on
                objLocated = true;
            }
        } else if (objLocated)
        {
            if (temp == "v")
            {
                // Found vertex data
                float x, y, z;
                iss >> x >> y >> z;
                
                _vertices.emplace_back(x,y,z);
            }
        }
    }
}

void SceneObject::print()
{
    std::cout << _name << ":" << std::endl;
    for (const Vertex& vertex : _vertices)
    {
        std::cout << vertex << std::endl;
    }
    std::cout << std::endl << std::endl;
}

Cube::Cube(std::string name) : SceneObject(name) {}

void Cube::setColour(uint8_t index, Colour colour)
{
    _vertices[index].setColour(colour);
}

Colour Cube::getColour(uint8_t index) 
{
    return _vertices[index].getColour();
}