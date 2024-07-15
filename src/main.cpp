#include <iostream>
#include "Camera.h"
#include "SceneObject.h"

int main(int argc, char const *argv[])
{
    Camera camera{Vec3f(8.49, 15.41, 6.46), Vec3f(79, 0, 154.4)};
    Cube block1{"Block_1", Colour::BLUE};

    std::cout << block1;
    return 0;
}