#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "octahedronball.h"
#include "trianglesurface.h"

class Line;
class LAZSurface;

class RollingBall : public OctahedronBall
{
public:
    RollingBall(Scene& scene, Shader* shaderProgram, int n);
    ~RollingBall() override;
    void init() override;
    void draw() override;
    void move(float dt) override;
    void setSurface(LAZSurface* surface);
    QVector3D getPosition();
    QVector3D findNormal(QVector3D v0, QVector3D v1, QVector3D v2);
    QVector3D abs(QVector3D vector); // Finds the absolute value
protected:
    VisualObject* mSurface{nullptr};
private:
    int oldTriangleIndex{-1};
    QVector3D oldNormal{0.0, 0.0, 1.0};
    QVector3D mVelocity{0.00,0,0}; // Her gir vi den en litt ekstra start fart, for at den skal trille over kanten
    QVector3D mAcceleration{0,0,0};
    QVector3D mGravity{0,0,-9.80665};
    QVector3D mFrictionForce{0,0,0};


    float mGroundFriction = 0.1f; // drag is not affected by surface area, but instead a constant force
    float mMass {0.0175f}; //kg
    //float mRadius {0.0175f}; // m
    float mRadius {1.0f};

    float timeSlowDown {1};

    //Debug
    Line* mVelocityLine {nullptr};
    Line* mAccelerationLine {nullptr};
};
#endif // ROLLINGBALL_H
