#include "Camera.h"
#include <cmath>


Camera::Camera() : position{Vec3f()}, rotation{Vec3f()} {}
Camera::Camera(Vec3f pos, Vec3f rot) : position{pos}, rotation{rot} {}

void Camera::setFocalLength(float focalLength) {  if (focalLength > 0) { this->focalLength = focalLength;} }

// Float casts for trig functions
float cosf(float angle) { return (float)(cos(angle * M_PI/180)); }
float sinf(float angle) { return (float)(sin(angle * M_PI/180)); }

const Matrix44f Camera::getCameraToWorld() const
{
    float r = rotation.x;
    float s = rotation.y;
    float t = rotation.z;
    float x = position.x;
    float y = position.y;
    float z = position.z;

    // Rotation around X
    Matrix44f rotx = 
    {
        1, 0, 0, 0,
        0, cosf(r), sinf(r), 0,
        0, -sinf(r), cosf(r), 0,
        0, 0, 0, 1
    };

    // Rotation around Y (Negative because x-axis points in opposite direction than convenetional when looking down the y-axis)
    Matrix44f roty = 
    {
        cosf(-s), 0, sinf(-s), 0,
        0, 1, 0, 0,
        -sinf(-s), 0, cosf(-s), 0,
        0, 0, 0, 1
    };

    // Rotation around Z
    Matrix44f rotz = 
    {
       cosf(t), sinf(t), 0, 0,
       -sinf(t), cosf(t), 0, 0, 
       0, 0, 1, 0,
       0, 0, 0, 1 
    };

    // Translation to (x,y,z)
    Matrix44f translation = 
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    };

    // Compose 3 Rotations and a translation together
    Matrix44f temp;
    Matrix44f::multiply(rotx, roty, temp);

    Matrix44f temp2;
    Matrix44f::multiply(temp, rotz, temp2);

    Matrix44f cameraToWorld;
    Matrix44f::multiply(temp2, translation, cameraToWorld);

    return cameraToWorld;
}

const Matrix44f Camera::getWorldToCamera() const { return getCameraToWorld().inverse(); }