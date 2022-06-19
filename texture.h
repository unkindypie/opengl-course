#ifndef TEXTURE_H
#define TEXTURE_H

#include "stb/stb_image.h"
#include <GL/glew.h>
#include <iostream>

class Texture {
public:
  Texture();
  Texture(std::string fileLoc);

  void loadTexture();
  void useTexture();
  void clearTexture();

  ~Texture();

private:
  GLuint textureId;
  int width, height, bitDepth;
  std::string fileLocation;
};

#endif
