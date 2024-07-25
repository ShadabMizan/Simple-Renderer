// This is a class that will create a camera object from which the scene will be rendered in.
#pragma once

#include "geometry.h"

struct Camera
{
    Camera();
    Camera(Vec3f pos, Vec3f rot);

    void setFocalLength(float focalLength);
    
    const Matrix44f getCameraToWorld() const;
    const Matrix44f getWorldToCamera() const;

    Vec3f position;     // Position in world coordinates
    Vec3f rotation;     // Rotation ...

    float focalLength = 40;         // Distance between the eye and the image plane, in mm
    float filmApertureWidth = 36;   // Used to determine the angle of view (AOV) and film gate aspect ratio, in mm
    float filmApertureHeight = 24;  // ... in mm
    float nearClippingPlane = 0.1;   // Determines the minimum distance from the eye for objects to be rendered in the camera's view. in m
    float farClippingPlane = 1000;    // Determines the maximum distance ... in m
};