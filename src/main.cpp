#include <iostream>
#include <fstream>
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

// a, b, and c are the vertices of a triangle. We can find the determinant (or area of triangle) by using vectors ab and ac.
// E(P) > 0 if P is to the right of the edge made by v1 and v2
// E(P) = 0 if it is on the edge
// E(P) < 0 if it is to the left of the edge
float edgeFunction(const Vec3f& v1, const Vec3f& v2, const Vec3f& pixel)
{
    // return ((b.x-a.x) * (c.y-a.y) - (c.x-a.x) * (b.y-a.y))/2;
    float determinant = (pixel.x - v1.x) * (v2.y - v1.y) - (pixel.y - v1.y) * (v2.x - v1.x);
    return determinant/2;
}


// Matches the 1.5 aspect ratio of the film aperture of the default camera.
const uint32_t imageWidth = 720;
const uint32_t imageHeight = 480;

int main(int argc, char const *argv[])
{
    Camera camera{Vec3f(8.53, 23.35, 3.25), Vec3f(90, 0, 161)};
    
    std::shared_ptr<Cube> block1 = std::make_shared<Cube>("Block_1", Colour::RED);
    // std::shared_ptr<Cube> block2 = std::make_shared<Cube>("Block_2", Colour::GREEN);
    // std::shared_ptr<Cube> block3 = std::make_shared<Cube>("Block_3", Colour::BLUE);
    std::vector<std::shared_ptr<SceneObject>> scene{block1};

    // Compute screen coordinates for the image plane
    float t, b, l, r;
    computeScreenCoordinates(camera, t, b, l, r);

    // Colour buffer, which stores the colour data of all the pixels in the image
    // Colour* colourBuffer = new Colour[imageWidth * imageHeight];
    // for (int i{0}; i < imageWidth * imageHeight; ++i) { colourBuffer[i] = Colour(); }

    unsigned char* frameBuffer = new unsigned char[imageWidth * imageHeight * 3];
    for (int i{0}; i < imageWidth * imageHeight * 3; i++) { frameBuffer[i] = 255; }
    
    float* zBuffer = new float[imageWidth * imageHeight];
    for (int i{0}; i < imageWidth * imageHeight; ++i) { zBuffer[i] = camera.farClippingPlane; }

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

            float area = abs(edgeFunction(v0Raster, v2Raster, v1Raster));
            // Iterate through the bounding box in the image buffer
            for (uint32_t y{y0}; y <= y1; y++)
            {
                for (uint32_t x{x0}; x <= x1; x++)
                {
                    // Sample point in the middle of the pixel being targetted
                    Vec3f pixelSample(x + 0.5, y + 0.5, 0);

                    // Check if it lies within our triangle using the determinant. Area will be positive if the triangle is within 
                    // w's represent the proportion of the effect of each vertex attirbute at a location in the triangle. Used for linear interpolation.
                    float w0 = edgeFunction(v1Raster, v2Raster, pixelSample);
                    float w1 = edgeFunction(v0Raster, v2Raster, pixelSample);    
                    float w2 = edgeFunction(v0Raster, v1Raster, pixelSample);    

                    if (w0 >= 0 && w1 >= 0 && w2 >= 0)
                    {
                        // pixel sample lies within the triangle

                        // Get proportions for linear interpolation of vertex data
                        w0 /= area;
                        w1 /= area;
                        w2 /= area;

                        // Z interpolation for colour attribute
                        // v0Raster.zInterpolate();
                        // v1Raster.zInterpolate();
                        // v2Raster.zInterpolate();

                        // Z coordinate interpolation
                        float oneOverZ = (1/v0Raster.z) * w0 + (1/v1Raster.z) * w1 + (1/v2Raster.z) * w2;
                        float z = 1/oneOverZ;

                        // Check if z is closer than what is stored in z buffer
                        if (z < zBuffer[y*imageWidth + x])
                        {
                            // Update the z buffer
                            zBuffer[y*imageWidth + x] = z;

                            // Update the image buffer with attributes

                            // Get colour of the point
                            float r = w0 * v0Raster.getColour().R + w1 * v1Raster.getColour().R + w2 * v2Raster.getColour().R;
                            float g = w0 * v0Raster.getColour().G + w1 * v1Raster.getColour().G + w2 * v2Raster.getColour().G;
                            float b = w0 * v0Raster.getColour().B + w1 * v1Raster.getColour().B + w2 * v2Raster.getColour().B;

                            // Perspective correct colour interpolation
                            // r *= z;
                            // g *= z;
                            // b *= z;

                            // Bind between 0 and 255
                            r = std::max(std::min((int32_t)r, 255), 0);
                            g = std::max(std::min((int32_t)g, 255), 0);
                            b = std::max(std::min((int32_t)b, 255), 0);


                            frameBuffer[3 * (y*imageWidth + x)] = (unsigned char)(r);
                            frameBuffer[3 * (y*imageWidth + x) + 1] = (unsigned char)(g);
                            frameBuffer[3 * (y*imageWidth + x) + 2] = (unsigned char)(b);
                        }                        
                    }
                }
            }   
        }
    }

    std::ofstream ofs;
    ofs.open("../output.ppm");
    ofs << "P6\n" << imageWidth << " " << imageHeight << "\n255\n";
    ofs.write((char*)frameBuffer, imageWidth * imageHeight * 3);
    ofs.close();

    delete [] frameBuffer;
    delete [] zBuffer;
    return 0;
}