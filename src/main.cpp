#include <iostream>
#include <memory>
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
    const Vertex& pWorld,                // Point in the world coordinate system
    const Camera& camera,               // Camera object contains worldToCamera matrix and near clipping plane data. World to Camera matrix transforms a vector from world space to camera space.
    const float& t,                     // Boundaries of the image plane. Used in NDC calculation.
    const float& b,                     
    const float& l,
    const float& r,
    const uint32_t& imageWidth,         // Dimensions of final image
    const uint32_t& imageHeight,
    Vertex &pRaster                      // Point in raster space, which is the only parameter being affected.
)
{
    const Matrix44f worldToCamera = camera.getWorldToCamera();
    Vec3f pCamera;      // point in camera coordinate system

    worldToCamera.multVecMatrix(pWorld, pCamera);

    // Convert to screen space
    Vec2f pScreen;
    pScreen.x = (pCamera.x / -pCamera.z) * camera.nearClippingPlane;
    pScreen.y = (pCamera.y / -pCamera.z) * camera.nearClippingPlane;
    
    // Conversion from screen space to NDC, which has a range of [-1,1]
    Vec2f pNDC;
    pNDC.x = (2*pScreen.x)/(r-l) - (r+l)/(r-l);
    pNDC.y = (2*pScreen.y)/(t-b) - (t+b)/(t-b);

    // Conversion to raster space, which has range [0, imageWidth], [0, imageHeight]
    pRaster.x = (pNDC.x + 1)/2 * imageWidth;
    pRaster.y = (1 - pNDC.y)/2 * imageHeight;  // Recal that y goes from top to bottom, so inverted
    
    pRaster.z = -pCamera.z;     // Opposite direction from camera's perspective

    // Set colour of the raster point as the same as the world coordinate's
    pRaster.setColour(pWorld.getColour());
}

// Matches the 1.5 aspect ratio of the film aperture of the default camera.
const uint32_t imageWidth = 720;
const uint32_t imageHeight = 480;

int main(int argc, char const *argv[])
{
    Camera camera{Vec3f(8.49, 15.41, 6.46), Vec3f(79, 0, 154.4)};

    std::shared_ptr<Cube> block1 = std::make_shared<Cube>("Block_1", Colour::RED);
    std::shared_ptr<Cube> block2 = std::make_shared<Cube>("Block_2", Colour::GREEN);
    std::shared_ptr<Cube> block3 = std::make_shared<Cube>("Block_3", Colour::BLUE);

    std::vector<std::shared_ptr<SceneObject>> scene{block1, block2, block3};

    // Compute screen coordinates for the image plane
    float t, b, l, r;
    computeScreenCoordinates(camera, t, b, l, r);

    // Allocate Image Buffer, which will contain colours, and Z-Buffer, which will have all the camera perspective z coords of all the points
    Colour* imgBuffer = new Colour[imageWidth * imageHeight];
    for (int i{0}; i < imageWidth * imageHeight; ++i) { imgBuffer[i] = Colour(); }
    
    float* zBuffer = new float[imageWidth * imageHeight];
    for (int i{0}; i < imageWidth * imageHeight; ++i) { zBuffer[i] = camera.farClippingPlane; }

    // Iterate over every object in the scene
    for (const auto& sceneObj : scene)
    {
        // Iterate over every triangle
        for (int i{0}; i < sceneObj->triangles.size(); ++i)
        {
            // Three vertices of the triangle, in world coordinates.
            const Vertex v0 = *(sceneObj->triangles[i][0]);
            const Vertex v1 = *(sceneObj->triangles[i][1]);
            const Vertex v2 = *(sceneObj->triangles[i][2]);

            // Convert to raster coords.
            Vertex v0Raster, v1Raster, v2Raster;
            convertToRaster(v0, camera, t, b, l, r, imageWidth, imageHeight, v0Raster);
            convertToRaster(v1, camera, t, b, l, r, imageWidth, imageHeight, v1Raster);
            convertToRaster(v2, camera, t, b, l, r, imageWidth, imageHeight, v2Raster);

            // Find bounding box, which spans from (xmin, ymix) to (xmax, ymax)
            float xmin = std::min(std::min(v0Raster.x, v1Raster.x), v2Raster.x);
            float ymin = std::min(std::min(v0Raster.y, v1Raster.y), v2Raster.y);
            float xmax = std::max(std::max(v0Raster.x, v1Raster.x), v2Raster.x);
            float ymax = std::max(std::max(v0Raster.y, v1Raster.y), v2Raster.y);

            // Checks if the triangle is out of bounds
            if (xmin > imageWidth - 1 || xmax < 0 || ymin > imageHeight - 1 || ymax < 0) continue;

            // Cast these as integers
            uint32_t x0 = std::max(int32_t(0), (int32_t)(std::floor(xmin)));
            uint32_t x1 = std::min(int32_t(imageWidth) - 1, (int32_t)(std::floor(xmax)));
            uint32_t y0 = std::max(int32_t(0), (int32_t)(std::floor(ymin)));
            uint32_t y1 = std::min(int32_t(imageHeight) - 1, (int32_t)(std::floor(ymax)));

            std::cout << "Bounding box for triangle " << i << " in " << sceneObj->getName() << ":\n" 
            << "(" << x0 << ", " << y0 << ") to (" << x1 << ", " << y1 << ")\n";
            
        }
    }

    delete [] imgBuffer;
    delete [] zBuffer;
    return 0;
}