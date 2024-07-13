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


// Cube with all vertices set to black
Cube::Cube(std::string name) : SceneObject(name) { setColour(Colour()); }

// Construct a cube with a Colour
Cube::Cube(std::string name, Colour colour) : SceneObject(name) { setColour(colour); }

// Overloaded print function
void Cube::print(std::ostream& os) const
{   
    for (const Vertex& vertex : _vertices)
    {
        os << "Vertex: " << vertex << "\t" << "Colour: " << vertex.getColour() << "\n";
    }
}

// Sets all vertices to the same colour
void Cube::setColour(Colour colour)
{
    for (Vertex& vertex : _vertices)
    {
        vertex.setColour(colour);
    }
}

void Cube::setColour(uint8_t index, Colour colour)
{
    _vertices[index].setColour(colour);
}

Colour Cube::getColour(uint8_t index) 
{
    return _vertices[index].getColour();
}