#ifndef TEXTUREGEOTIFF_H
#define TEXTUREGEOTIFF_H


//#include <GL/glew.h> // GLEW fournit les extensions OpenGL modernes
#include <GL/gl.h>

#include <vector>

class TextureGeoTiff {
public:
    GLuint createTextureFromRaster(const std::vector<unsigned char>& rasterData, int width, int height);
};

#endif // TEXTUREGEOTIFF_H
