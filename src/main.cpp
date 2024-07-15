#include <iostream>
#include "Camera.h"
#include "SceneObject.h"

void computeScreenCoordinates(
    const Camera& camera,           // Contains all the camera settings
    float &top, float &bottom, float &left, float &right    // Boundaries for our image plane
)
{   
    /* Explanation: 
        Top and Right can be computed based off of the geometry of the camera model.
        Essentially, tanx = (filmAH / focalLength) = (right / nearClippingPlane). 
        Thus, there are two similar triangles being made, one with the camera's settings which are its film aperture and focal length,
        and the other with the distance between the rightmost edge of the canvas's width and its centre and the near clipping plane, which is the distance between the eye and the canvas.
        Thus we can use similar triangles to find the rightmost edge's distance from the centre of the canvas.
        Repeat for top. Due to symmetry, bottom and left are just negatives of top and right.
    */ 
    top = ((camera.filmApertureHeight/2) / camera.focalLength) * camera.nearClippingPlane;
    right = ((camera.filmApertureWidth/2) / camera.focalLength) * camera.nearClippingPlane;
    bottom = -top;
    left = -right;
}

void convertToRaster(
    const Vec3f& pWorld,                // Point in the world to transform to raster space
    const Matrix44f& worldToCamera,     // World to camera matrix, needed to transform vector from world space to camera space
    const float& t,                     // Boundaries of the image plane. Used in NDC calculation.
    const float& b,                     
    const float& l,
    const float& r,
    const float& near,                  // Near clipping plane
    const uint32_t& imageWidth,         // Dimensions of final image
    const uint32_t& imageHeight,
    Vec3f &pRaster                      // Point in raster space, which is the only parameter being affected.
)
{
    Vec3f pCamera;
    worldToCamera.multVecMatrix(pWorld, pCamera);

    // Convert to screen space
    Vec2f pScreen;
    pScreen.x = pCamera.x / -pCamera.z * near;
    pScreen.y = pCamera.y / -pCamera.z * near;
    
    // Conversion from screen space to NDC, which has a range of [-1,1]
    Vec2f pNDC;
    pNDC.x = (2*pScreen.x)/(r-l) - (r+l)/(r-l);
    pNDC.y = (2*pScreen.y)/(t-b) - (t+b)/(t-b);

    // Conversion to raster space, which has range [0, imageWidth], [0, imageHeight]
    pRaster.x = (pNDC.x + 1) / (float)(2*imageWidth);
    pRaster.y = (1 - pNDC.y) / (float)(2*imageHeight);  // Recal that y goes from top to bottom, so inverted
    
    pRaster.z = -pCamera.z;     // Opposite direction from camera's perspective
}

const uint32_t imageWidth = 640;
const uint32_t imageHeight = 480;

int main(int argc, char const *argv[])
{
    Camera camera{Vec3f(8.49, 15.41, 6.46), Vec3f(79, 0, 154.4)};
    Cube block1{"Block_1", Colour::BLUE};

    // Compute screen coordinates for the image plane
    float t, b, l, r;
    computeScreenCoordinates(camera, t, b, l, r);

    // Allocate Image Buffer, which will contain colours, and Z-Buffer, which will have all the camera perspective z coords of all the points
    Colour* imgBuffer = new Colour[imageWidth * imageHeight];
    float* zBuffer = new float[imageWidth * imageHeight];

    

    delete [] imgBuffer;
    delete [] zBuffer;
    return 0;
}