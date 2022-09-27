#include "shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "camera.h"
#include "light.h"
//#include "GL/glew.h" - using QOpenGLFunctions instead

#include "logger.h" //For our utility Logger class
#include "scene.h"

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
{
    initializeOpenGLFunctions();    //must do this to get access to OpenGL functions in QOpenGLFunctions

    //Get the instance of the Output logger
    //Have to do this, else program will crash (or you have to put in nullptr tests...)
    mLogger = Logger::getInstance();

    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Open files and check for errors
    vShaderFile.open( vertexPath );
    if(!vShaderFile)
        mLogger->logText("ERROR SHADER FILE " + std::string(vertexPath) + " NOT SUCCESFULLY READ", LogType::REALERROR);
    fShaderFile.open( fragmentPath );
    if(!fShaderFile)
        mLogger->logText("ERROR SHADER FILE " + std::string(fragmentPath) + " NOT SUCCESFULLY READ", LogType::REALERROR);
    std::stringstream vShaderStream, fShaderStream;
    // Read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf( );
    fShaderStream << fShaderFile.rdbuf( );
    // close file handlers
    vShaderFile.close( );
    fShaderFile.close( );
    // Convert stream into string
    vertexCode = vShaderStream.str( );
    fragmentCode = fShaderStream.str( );

    const GLchar *vShaderCode = vertexCode.c_str( );
    const GLchar *fShaderCode = fragmentCode.c_str( );

    // 2. Compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];
    // Vertex Shader
    vertex = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertex, 1, &vShaderCode, nullptr );
    glCompileShader( vertex );
    // Print compile errors if any
    glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( vertex, 512, nullptr, infoLog );
        mLogger->logText("ERROR SHADER VERTEX " + std::string(vertexPath) +
                         " COMPILATION_FAILED\n" + infoLog, LogType::REALERROR);
    }
    // Fragment Shader
    fragment = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment, 1, &fShaderCode, nullptr );
    glCompileShader( fragment );
    // Print compile errors if any
    glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( fragment, 512, nullptr, infoLog );
        mLogger->logText("ERROR SHADER VERTEX " + std::string(fragmentPath) +
                         " COMPILATION_FAILED\n" + infoLog, LogType::REALERROR);
    }
    // Shader Program linking
    this->mProgram = glCreateProgram();
    glAttachShader( this->mProgram, vertex );
    glAttachShader( this->mProgram, fragment );
    glLinkProgram( this->mProgram );
    // Print linking errors if any
    glGetProgramiv( this->mProgram, GL_LINK_STATUS, &success );

    //making nice output name for printing:
    std::string shadername{vertexPath};
    shadername.resize(shadername.size()-5); //deleting ".vert"

    if (!success)
    {
        glGetProgramInfoLog( this->mProgram, 512, nullptr, infoLog );
        mLogger->logText("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" +
                          shadername +  "\n   " + infoLog, LogType::REALERROR);
    }
    else
    {
       mLogger->logText("GLSL shader " + shadername + " was successfully compiled");
    }
    // Delete the shaders as they're linked into our program now and no longer needed
    // The shader program is now on the GPU and we reference it by using the mProgram variable
    glDeleteShader( vertex );
    glDeleteShader( fragment );
}

void Shader::use()
{
    glUseProgram( this->mProgram );
}

GLuint Shader::getProgram() const
{
    return mProgram;
}

void Shader::init(Camera* camera)
{
    mCamera = camera;
}


PlainShader::PlainShader(const GLchar* vertexPath, const GLchar* fragmentPath) : Shader(vertexPath, fragmentPath)
{
    mMatrixUniform = glGetUniformLocation(getProgram(), "matrix");
    vMatrixUniform = glGetUniformLocation(getProgram(), "vmatrix");
    pMatrixUniform = glGetUniformLocation(getProgram(), "pmatrix");

}

void PlainShader::loadShader()
{
    use();
    mCamera->init(pMatrixUniform, vMatrixUniform);
    mCamera->mScene->renderCamera();
}

int PlainShader::getShaderSlot()
{
    initializeOpenGLFunctions();
    return GL_TEXTURE0;
}

TextureShader::TextureShader(const GLchar* vertexPath, const GLchar* fragmentPath) : Shader(vertexPath, fragmentPath)
{
    mMatrixUniform = glGetUniformLocation(getProgram(), "mMatrix");
    vMatrixUniform = glGetUniformLocation(getProgram(), "vMatrix");
    pMatrixUniform = glGetUniformLocation(getProgram(), "pMatrix");
    mTextureUniform = glGetUniformLocation(getProgram(), "textureSampler");

}

void TextureShader::loadShader()
{
    use();
    mCamera->init(pMatrixUniform, vMatrixUniform);
    mCamera->mScene->renderCamera();
    glUniform1i(mTextureUniform, 1); // Choose shader from texture slots

}

int TextureShader::getShaderSlot()
{
    initializeOpenGLFunctions();
    return GL_TEXTURE1;
}

PhongShader::PhongShader(const GLchar* vertexPath, const GLchar* fragmentPath) : Shader(vertexPath, fragmentPath)
{
    mMatrixUniform = glGetUniformLocation(getProgram(), "mMatrix");
    vMatrixUniform = glGetUniformLocation(getProgram(), "vMatrix");
    pMatrixUniform = glGetUniformLocation(getProgram(), "pMatrix");

    mLightColorUniform = glGetUniformLocation(getProgram(), "lightColor");
    mObjectColorUniform = glGetUniformLocation(getProgram(), "objectColor");
    mAmbientLightStrengthUniform = glGetUniformLocation(getProgram(), "ambientStrengt");
    mLightPositionUniform = glGetUniformLocation(getProgram(), "lightPosition");
    mSpecularStrengthUniform = glGetUniformLocation(getProgram(), "specularStrength");
    mSpecularExponentUniform = glGetUniformLocation(getProgram(), "specularExponent");
    mLightPowerUniform = glGetUniformLocation(getProgram(), "lightPower");
    mCameraPositionUniform = glGetUniformLocation(getProgram(), "cameraPosition");
    mTextureUniform2 = glGetUniformLocation(getProgram(), "textureSampler");

}

void PhongShader::init(Light* light, Camera* camera)
{
    mLight = light;
    mCamera = camera;
}

void PhongShader::loadShader()
{
    use();
    mCamera->init(pMatrixUniform, vMatrixUniform);
    mCamera->mScene->renderCamera();
    //Additional parameters for light shader:
    glUniform3f(mLightPositionUniform, mLight->mMatrix.column(3).x(), mLight->mMatrix.column(3).y(),
        mLight->mMatrix.column(3).z());
    glUniform3f(mCameraPositionUniform, mCamera->mVmatrix.column(3).x(), mCamera->mVmatrix.column(3).y(),
        mCamera->mVmatrix.column(3).z());
    glUniform3f(mLightColorUniform, mLight->mLightColor.x(), mLight->mLightColor.y(), mLight->mLightColor.z());
    glUniform1f(mSpecularStrengthUniform, mLight->mSpecularStrenght);
    //Texture
	glUniform1i(mTextureUniform2, 2);
}

int PhongShader::getShaderSlot()
{
    initializeOpenGLFunctions();
    return GL_TEXTURE2;
}

SkyBoxShader::SkyBoxShader(const GLchar* vertexPath, const GLchar* fragmentPath) : Shader(vertexPath, fragmentPath)
{
    mMatrixUniform = glGetUniformLocation(getProgram(), "mMatrix");
    vMatrixUniform = glGetUniformLocation(getProgram(), "vMatrix");
    pMatrixUniform = glGetUniformLocation(getProgram(), "pMatrix");
    mTextureUniform3 = glGetUniformLocation(getProgram(), "textureSampler");
    rotationUniform = glGetUniformLocation(getProgram(), "rMatrix");

}

void SkyBoxShader::init(Camera* camera)
{
    mCamera = camera;
}

void SkyBoxShader::loadShader()
{
    use();
    mCamera->init(pMatrixUniform, vMatrixUniform);
    mCamera->mScene->renderCamera();
}

int SkyBoxShader::getShaderSlot()
{
    initializeOpenGLFunctions();
    return GL_TEXTURE3;
}




