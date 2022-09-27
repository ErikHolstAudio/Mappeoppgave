#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions_4_1_Core>

//#include "GL/glew.h" //We use QOpenGLFunctions instead, so no need for Glew (or GLAD)!

//This class is pretty much a copy of the shader class at
//https://github.com/SonarSystems/Modern-OpenGL-Tutorials/blob/master/%5BLIGHTING%5D/%5B8%5D%20Basic%20Lighting/Shader.h
//which is based on stuff from http://learnopengl.com/ and http://open.gl/.

//must inherit from QOpenGLFunctions_4_1_Core, since we use that instead of glfw/glew/glad
class Camera;
class Light;
class ShaderHandler;
class Shader : protected QOpenGLFunctions_4_1_Core
{
public:
    // Constructor generates the shader on the fly
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath);

    ///Use the current shader
    void use( );

    ///Returns the program number for this shader
    GLuint getProgram() const;

    virtual void loadShader(){};
    void init(Camera* camera);

    virtual int getShaderSlot() { return false; };


	GLint mMatrixUniform{ -1 };
    GLint vMatrixUniform{ -1 };
    GLint pMatrixUniform{ -1 };

protected:
    Camera* mCamera{ nullptr };


private:
    ///The int OpenGL gives as a reference to this shader
    GLuint mProgram;

    ///The logger - Output Log in the application
    class Logger *mLogger{nullptr};

};

class PlainShader : public Shader
{
public:
    PlainShader(const GLchar* vertexPath, const GLchar* fragmentPath);
    void loadShader() override;
    int getShaderSlot() override;
};

class TextureShader : public Shader
{
public:
    TextureShader(const GLchar* vertexPath, const GLchar* fragmentPath);
    void loadShader() override;
    int getShaderSlot() override;
    GLint mTextureUniform{ -1 };

};

class PhongShader : public Shader
{
public:
    PhongShader(const GLchar* vertexPath, const GLchar* fragmentPath);

    void init(Light* light, Camera* camera);
    void loadShader() override;
    int getShaderSlot() override;

    GLint mLightColorUniform{ -1 };
    GLint mObjectColorUniform{ -1 };
    GLint mAmbientLightStrengthUniform{ -1 };
    GLint mLightPositionUniform{ -1 };
    GLint mCameraPositionUniform{ -1 };
    GLint mSpecularStrengthUniform{ -1 };
    GLint mSpecularExponentUniform{ -1 };
    GLint mLightPowerUniform{ -1 };
    GLint mTextureUniform2{ -1 };

protected:
    Light* mLight{nullptr};
};

class SkyBoxShader : public Shader
{
public:
    SkyBoxShader(const GLchar* vertexPath, const GLchar* fragmentPath);

    void init(Camera* camera);
    void loadShader() override;
    GLint mTextureUniform3{ -1 };
    GLint rotationUniform{ -1 };
    int getShaderSlot() override;


};

#endif
