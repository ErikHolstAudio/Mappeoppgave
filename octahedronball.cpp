#include <iostream>
#include <sstream>
#include "octahedronball.h"

//! \param n - the recursion level (default is 0 which makes the original Octahedron)
//!
//! The number of (duplicated) vertices are calculated based on the parameter n - the recursion level.
//! Initially we have 8 triangles, each with 3 vertices.
//! A subdivision consists of
//! - compute the midpoint of each edge in a triangel
//! - add these midpoint as vertices
//! - make three new edges by connecting new vertices
//! - preserve orientation
//! - move the three new points on the unit ball's surface by normalizing. Remember the vertex vector
//! is also a normal to the ball's surface.
//! The formula for the number of vertices is
//! m_nVertices = 3 * 8 * 4^n
//! where
//! - 3 comes from the number of vertices in a triangle,
//! - 8 comes from the original number of triangles in a regular Octahedron
//! - n is the recursion level (number of repeated subdivisions)
//!

OctahedronBall::OctahedronBall(Scene& scene, Shader* shaderProgram, int n) : VisualObject(scene, shaderProgram), m_rekursjoner(n), m_indeks(0)
{
	mVertices.reserve(3 * 8 * pow(4, m_rekursjoner));
	oktaederUnitBall();
	bShape = new AABB();

}

//!//! \brief OctahedronBall::~OctahedronBall() virtual destructor
//!
OctahedronBall::~OctahedronBall()
{
	//
}

//!
//! \brief OctahedronBall::initVertexBufferObjects() calls glGenBuffers(), glBindBuffer() and glBufferdata()
//! for using later use of glDrawArrays()
//!


//!
//! \brief OctahedronBall::draw() draws a ball using glDrawArrays()
//! \param positionAttribute    vertex shader variable for position
//! \param normalAttribute      vertex shader variable for normal or color
//! \param textureAttribute     vertex shader variable for texture coordinates (optional)
//!
//! draw()
//! - glBindBuffer()
//! - glVertexAttribPointer()
//! - glBindTexture()
//! - glVertexAttribPointer()
//! - glDrawArrays() with GL_TRIANGLES
//!


void OctahedronBall::move(float x, float y, float z)
{
	mPosition.setToIdentity(); // hvis nødvendig
	QVector4D pos{ x,y,z,1.0f };
	mPosition.setColumn(3, pos);
	//mScale.scale(QVector3D(0.25, 0.25, 0.25));
	mMatrix = mPosition; // * mScale * mRotation etter behov, pass på rekkefølgen
}

// Parametre inn: xyz koordinatene til et triangle v1, v2, v3 ccw
// Bevarer orienteringen av hjørnene
//!
//! \brief OctahedronBall::lagTriangel()
//! \param v1 - position on the unit ball for vertex 1
//! \param v2 - position on the unit ball for vertex 2
//! \param v3 - position on the unit ball for vertex 3
//!
//! lagTriangel() creates vertex data for a triangle's 3 vertices. This is done in the
//! final step of recursion.
//!
void OctahedronBall::lagTriangel(const QVector3D& v1, const QVector3D& v2, const QVector3D& v3)
{
    QVector3D color{1.f,1.f,0.f};
    gsml::Vertex v{ v1.x(), v1.y(), v1.z(), color.x(), color.y(), color.z() };
	mVertices.push_back(v);
    v = gsml::Vertex{ v2.x(), v2.y(), v2.z(), color.x(), color.y(), color.z() };
	mVertices.push_back(v);
    v = gsml::Vertex{ v3.x(), v3.y(), v3.z(), color.x(), color.y(), color.z() };
	mVertices.push_back(v);
}

// Rekursiv subdivisjon av triangel
//!
//! \brief OctahedronBall::subDivide() recursive subdivision of a triangel
//! \param a coordinates for vertex a
//! \param b coordinates for vertex b
//! \param c coordinates for vertex c
//! \param n level of recursion
//!
//! The function tests
//! (if n>0)
//! - three new edge midpoints are computed and normalized,
//! - four new calls to subDivide() is done - one for each new triangle, preserving orientation
//! and with decreased parameter n
//! else
//! - call lagTriangel(a, b, c)
//!
void OctahedronBall::subDivide(const QVector3D& a, const QVector3D& b, const QVector3D& c, int n)
{
	if (n > 0) {
		QVector3D v1 = a + b; v1.normalize();
		QVector3D v2 = a + c; v2.normalize();
		QVector3D v3 = c + b; v3.normalize();
		subDivide(a, v1, v2, n - 1);
		subDivide(c, v2, v3, n - 1);
		subDivide(b, v3, v1, n - 1);
		subDivide(v3, v2, v1, n - 1);
	}
	else {
		lagTriangel(a, b, c);
	}
}

//!
//! \brief OctahedronBall::oktaederUnitBall() creates 8 unit ball vertices and call subDivide()
//!
//! If the parameter n of the constructor OctahedronBall() is zero, the result will be the
//! original octahedron consisting of 8 triangles with duplicated vertices.
//!
void OctahedronBall::oktaederUnitBall()
{
	QVector3D v0{ 0, 0, 1 };
	QVector3D v1{ 1, 0, 0 };
	QVector3D v2{ 0, 1, 0 };
	QVector3D v3{ -1, 0, 0 };
	QVector3D v4{ 0, -1, 0 };
	QVector3D v5{ 0, 0, -1 };

	subDivide(v0, v1, v2, m_rekursjoner);
	subDivide(v0, v2, v3, m_rekursjoner);
	subDivide(v0, v3, v4, m_rekursjoner);
	subDivide(v0, v4, v1, m_rekursjoner);
	subDivide(v5, v2, v1, m_rekursjoner);
	subDivide(v5, v3, v2, m_rekursjoner);
	subDivide(v5, v4, v3, m_rekursjoner);
	subDivide(v5, v1, v4, m_rekursjoner);
}
