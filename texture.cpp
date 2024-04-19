#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

Texture::Texture() {
  textureId = 0;
  height = 0;
  bitDepth = 0;
  fileLocation = "";
}

Texture::Texture(std::string fileLoc) {
  textureId = 0;
  height = 0;
  bitDepth = 0;
  fileLocation = fileLoc;
}

void Texture::loadTexture() {
  unsigned char *texData =
      stbi_load(fileLocation.c_str(), &width, &height, &bitDepth, 0);
  if (!texData) {
    printf("Failed to find: %s\n", fileLocation.c_str());
  }

  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, texData);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(texData);
}

void Texture::useTexture() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::clearTexture() {
  glDeleteTextures(1, &textureId);
  textureId = 0;
  height = 0;
  bitDepth = 0;
  fileLocation = "";
}

Texture::~Texture() { clearTexture(); }
