#include <iostream>
#include "Camera.h"
#include "SceneObject.h"

int main(int argc, char const *argv[])
{
    Cube block1{"Block_1"};

    std::cout << block1;
    return 0;
}
