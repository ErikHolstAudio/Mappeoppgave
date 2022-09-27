#pragma once
#include "visualobject.h"
class Scene;

class SkyBox :
    public VisualObject
{
public:
    SkyBox(Scene& scene, Shader* shaderProgram);
    void init() override;
    void draw() override;

    void createCubeMap(
        const char* front,
        const char* back,
        const char* top,
        const char* bottom,
        const char* left,
        const char* right,
        GLuint &tex_cube);

    bool loadCubeMapSide(
        GLuint texture,
        GLenum side_target,
        const char* file_name);

    GLuint mTextureCube;

    GLuint cubemapTexture;

    std::vector<std::string> CubemapDir;

};

