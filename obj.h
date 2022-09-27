#pragma once
#include <fstream>
#include <sstream>
#include <algorithm>
#include "visualobject.h"

class Texture;
class OBJ :
    public VisualObject
{
public:
    OBJ(Scene& scene, Shader* shaderProgram, const std::string objdir, std::string texturedir);
    ~OBJ();

    void draw() override;

    void loadOBJ(std::string file_dir);

private:
    Texture* mTexture;
    bool bTexture{false};
};

