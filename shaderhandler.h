#pragma once
#include <QOpenGLFunctions_4_1_Core>
#include <shader.h>
class ShaderHandler : protected QOpenGLFunctions_4_1_Core
{
public:
    ShaderHandler();

    void setupPlainShader(int shaderIndex);
    void setupTextureShader(int shaderIndex);
    void setupPhongShader(int shaderIndex);

    //Plain shader
    GLint mMatrixUniform0{ -1 };
    GLint vMatrixUniform0{ -1 };
    GLint pMatrixUniform0{ -1 };

    //Texture Shader
    GLint mMatrixUniform1{ -1 };
    GLint vMatrixUniform1{ -1 };
    GLint pMatrixUniform1{ -1 };
    GLint mTextureUniform{ -1 };

    //Phong Shader
    GLint mMatrixUniform2{ -1 };
    GLint vMatrixUniform2{ -1 };
    GLint pMatrixUniform2{ -1 };

    //other light shader variables
    GLint mLightColorUniform{ -1 };
    GLint mObjectColorUniform{ -1 };
    GLint mAmbientLightStrengthUniform{ -1 };
    GLint mLightPositionUniform{ -1 };
    GLint mCameraPositionUniform{ -1 };
    GLint mSpecularStrengthUniform{ -1 };
    GLint mSpecularExponentUniform{ -1 };
    GLint mLightPowerUniform{ -1 };
    GLint mTextureUniform2{ -1 };

    Shader *mShaderProgram[4]{nullptr};
};

