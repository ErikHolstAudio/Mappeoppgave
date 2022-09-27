#include "camera.h"
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"


Camera::Camera(Scene* scene) : mScene(scene), mEye{0,0,0}
{
    mPmatrix.setToIdentity();
    mVmatrix.setToIdentity();
}

Camera::Camera(Scene *scene, VisualObject *model) : mScene(scene), mModel(model)
{
    mModel->init();
}

void Camera::init(GLint pMatrixUniform, GLint vMatrixUniform)
{
    mPmatrix.setToIdentity();
    mVmatrix.setToIdentity();
    mPmatrixUniform = pMatrixUniform;
    mVmatrixUniform = vMatrixUniform;
}

void Camera::perspective(int degrees, double aspect, double nearplane, double farplane)
{
    mPmatrix.perspective(degrees,aspect,nearplane,farplane);
}

void Camera::lookAt(const QVector3D &eye, const QVector3D &at, const QVector3D &up)
{

	if (!bFreeCamera)
	{
        mPosition = eye;
        mVmatrix.lookAt(eye, at, up);
        if (mModel)
        {
            mModel->move(mPosition.x(), mPosition.y(), mPosition.z());
            QVector3D angle(
                glm::degrees(atan2(
                    sqrt(pow(eye.x() - at.x(), 2) + pow(eye.y() - at.y(), 2)),
                    at.z() - eye.z())) - 90.0f, // the distance between the vectors should not include the Z position distance in it
                glm::degrees(atan2(at.y() - eye.y(), at.x() - eye.x())
                             ),
                0.0f);

            mModel->mMatrix.rotate(angle.x(), QVector3D(1, 0, 0));
        }
	}
	else
	{
        keyInput(mKey, 0.02f);
        mVmatrix.lookAt(mPosition, mPosition + mForward, mUp);
	}
    update();
}

void Camera::update()
{
    initializeOpenGLFunctions();
    glUniformMatrix4fv(mPmatrixUniform, 1, GL_FALSE, mPmatrix.constData()); // Om vi bruker vår egen matrise må vi bruke GL_TRUE
    glUniformMatrix4fv(mVmatrixUniform, 1, GL_FALSE, mVmatrix.constData());
}

void Camera::translate(float dx, float dy, float dz)
{
    mVmatrix.translate(dx,dy,dz);
}

void Camera::drawCamera()
{
    if(mModel)
        mModel->draw();
}

//Oppgave 5
void Camera::keyInput(bool key[5], float speed)
{
	if (key[6])
		speed = speed * 2;

    //float moveX{ 0.0f }, moveY{ 0.0f }, moveZ{0.0f};

	if (key[0]) // W
		mPosition += speed * mForward;

	if (key[1]) // A
		mPosition += -speed * QVector3D::crossProduct(mForward,mUp);

	if (key[2]) // S
		mPosition += -speed * mForward;

	if (key[3]) // D
		mPosition += speed * QVector3D::crossProduct(mForward, mUp);

	if (key[4]) // Q
		mPosition += -speed * mUp;

	if (key[5]) // E
		mPosition += speed * mUp;

	//mPosition += QVector3D(moveX, moveY, 0.f);
}

void Camera::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_W:
		mKey[0] = true;
		break;
	case Qt::Key_A:
		mKey[1] = true;
		break;
	case Qt::Key_S:
		mKey[2] = true;
		break;
	case Qt::Key_D:
		mKey[3] = true;
		break;
	case Qt::Key_Q:
		mKey[4] = true;
		break;
	case Qt::Key_E:
		mKey[5] = true;
		break;
	case Qt::Key_Shift:
		mKey[6] = true;
		break;
	default:
		break;
	}
}

void Camera::mouseMoveEvent(QMouseEvent* event)
{
    float newMouseX(event->position().y());
    float newMouseY(event->position().x());

	float deltaX = mouseX - newMouseX;
	float deltaY = mouseY - newMouseY;

	mouseX = newMouseX;
	mouseY = newMouseY;

	float rotateX(deltaX * 0.5f);
	float rotateY(deltaY * 0.5f);

	glm::vec3 newForward
		= glm::rotate(
			glm::vec3(mForward.x(), mForward.y(), mForward.z()),
			glm::radians(rotateX),
			glm::normalize(
				glm::cross(
					glm::vec3(mForward.x(), mForward.y(), mForward.z()),
					glm::vec3(mUp.x(), mUp.y(), mUp.z()))));

	newForward = glm::rotate(newForward, glm::radians(rotateY), glm::vec3(mUp.x(), mUp.y(), mUp.z()));

	mForward = QVector3D(newForward.x, newForward.y, newForward.z);

}

void Camera::keyReleaseEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_W:
		mKey[0] = false;
		break;
	case Qt::Key_A:
		mKey[1] = false;
		break;
	case Qt::Key_S:
		mKey[2] = false;
		break;
	case Qt::Key_D:
		mKey[3] = false;
		break;

	case Qt::Key_Q:
		mKey[4] = false;
		break;

	case Qt::Key_E:
		mKey[5] = false;
		break;

	case Qt::Key_Shift:
		mKey[6] = false;
		break;
	default:
		break;
	}
}
// slutt på oppgave 5
QVector3D Camera::getPosition()
{
    return mPosition;
}

glm::vec3 Camera::getRight()
{
	return glm::vec3(mRight.x(), mRight.y(), mRight.z());
}

glm::vec3 Camera::getForward()
{
	return glm::vec3(mForward.x(), mForward.y(), mForward.z());
}

glm::vec3 Camera::getUp()
{
	return glm::vec3(mUp.x(), mUp.y(), mUp.z());
}
