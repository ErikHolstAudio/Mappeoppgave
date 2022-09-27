#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLFunctions_4_1_Core>
#include <QVector3D>
#include <QKeyEvent>
#include <QMatrix4x4>
#include "visualobject.h"
#include "glm/glm.hpp"

class Scene;
class Shader;
class VisualObject;
class Camera : public QOpenGLFunctions_4_1_Core
{ 
public:
    Camera(Scene* scene);
    Camera(Scene* scene, VisualObject* model);
    ~Camera() { }
    void init(GLint pMatrixUniform, GLint vMatrixUniform);
    void perspective(int degrees, double aspect, double nearplane, double farplane);
    void lookAt(const QVector3D& eye, const QVector3D& at, const QVector3D& up);
    void update();
    void translate(float dx, float dy, float dz);
    void drawCamera();
    void keyInput(bool key[5], float speed);
    void keyPressEvent(QKeyEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    QMatrix4x4 mPmatrix{};         // denne,
    QMatrix4x4 mVmatrix{};         // og denne, skal legges inn i kameraklasse

	Scene* mScene{nullptr};
    VisualObject* mModel{nullptr};

    // new code

    bool bFreeCamera{ false };

    QVector3D getPosition();

    glm::vec3 getRight();
    glm::vec3 getForward();
    glm::vec3 getUp();

    QVector3D mRight{ 1.f, 0.f, 0.f };
    QVector3D mUp{ 0.f, 0.f, 1.f };
    QVector3D mForward{ 0.f, 1.f, 0.f };
    QVector3D mPosition{ 0.f, -10.f, 10.f };

private:
    GLint mPmatrixUniform;        //OpenGL reference to the Uniform in the shader program
    GLint mVmatrixUniform;        //OpenGL reference to the Uniform in the shader program
    QVector3D mEye{0.f,0.f,0.f};

    QMatrix4x4 mYawMatrix{}; // Rotate
    QMatrix4x4 mPitchMatrix{}; // Up/down

    float mPitch{ 0.f };
    float mYaw{ 0.f };

    float mSpeed{ 0.f }; //camera will move by this speed along the mForward vector
    bool mKey[7]{ false };

    float mouseX{ 0.f };
    float mouseY{ 0.f };
};

#endif // CAMERA_H
