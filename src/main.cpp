#include <iostream>
#include "Camera.h"
#include "SceneObject.h"

int main(int argc, char const *argv[])
{
    Cube block1{"Block_1"};

    std::cout << block1 << std::endl;

    Cube block2{"Block_2", Colour(255,0,0)};
    block2.setColour(3, Colour(0, 255, 0));
    std::cout << block2 << std::endl;
    
    Cube block3{"Block_3", Colour::BLUE};
    std::cout << block3 << std::endl;

    
    return 0;
}
