#include "vertex.h"

Vertex::Vertex()
{    
}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float s, float t) // float u, float v
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;
    m_normal[0] = r;
    m_normal[1] = g;
    m_normal[2] = b;
    m_uv[0] = s;
    m_uv[1] = t;

}

Vertex::Vertex(QVector3D a, QVector3D b, QVector2D c)
{
    m_xyz[0] = a.x();
    m_xyz[1] = a.y();
    m_xyz[2] = a.z();
    m_normal[0] = b.x();
    m_normal[1] = b.y();
    m_normal[2] = b.z();
    m_uv[0] = c.x();
    m_uv[1] = c.y();
}

Vertex::~Vertex()
{

}

float Vertex::x()
{
    return m_xyz[0];
}

float Vertex::y()
{
    return m_xyz[1];
}

float Vertex::z()
{
    return m_xyz[2];
}

QVector3D Vertex::getXYZ()
{
    return QVector3D(x(), y(), z());
}

void Vertex::setNormals(QVector3D normals)
{
    m_normal[0] = normals.x();
    m_normal[1] = normals.y();
    m_normal[2] = normals.z();

}

void Vertex::setUVs(QVector2D UVs)
{
    m_uv[0] = UVs.x();
    m_uv[1] = UVs.y();
}

std::ostream& operator<< (std::ostream& os, const Vertex& v)
{
  os << std::fixed;
  os << "(" << v.m_xyz[0] << ", " << v.m_xyz[1] << ", " << v.m_xyz[2] << ") ";
  os << "(" << v.m_normal[0] << ", " << v.m_normal[1] << ", " << v.m_normal[2] << ") ";
  os << "(" << v.m_uv[0] << ", " << v.m_uv[1] << ") ";
  return os;
}

std::istream& operator>> (std::istream& is, Vertex& v)
{
// Trenger fire temporære variabler som kun skal lese inn parenteser og komma
  char dum, dum2, dum3, dum4;
  is >> dum >> v.m_xyz[0] >> dum2 >> v.m_xyz[1] >> dum3 >> v.m_xyz[2] >> dum4;
  is >> dum >> v.m_normal[0] >> dum2 >> v.m_normal[1] >> dum3 >> v.m_normal[2] >> dum4;
  is >> dum >> v.m_uv[0] >> dum2 >> v.m_uv[1] >> dum3;
  return is;
}
