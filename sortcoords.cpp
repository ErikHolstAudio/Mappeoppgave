#include "sortcoords.h"

bool SortCoords::compareX(gsml::Vertex V1, gsml::Vertex V2)
{
    if (V1.m_xyz[0] == V2.m_xyz[0])
    {
        if (V1.m_xyz[1] < V2.m_xyz[1])
        {
            return 1;
        }
    }

    return (V1.m_xyz[0] < V2.m_xyz[0]);

}

bool SortCoords::compareY(gsml::Vertex V1, gsml::Vertex V2)
{
    if (V1.m_xyz[1] == V2.m_xyz[1])
    {
        if (V1.m_xyz[0] < V2.m_xyz[0])
        {
            return 1;
        }
    }

    return (V1.m_xyz[1] < V2.m_xyz[1]);
}

void SortCoords::SortX(std::vector<gsml::Vertex> &points)
{
    std::sort(points.begin(), points.end(), compareX);
}

void SortCoords::SortY(std::vector<gsml::Vertex>& points)
{
    std::sort(points.begin(), points.end(), compareY);
}

float SortCoords::FindXMin(std::vector<gsml::Vertex>& points)
{
    if (points.size() < 1)
        return -1.f;

    float minx = points[0].m_xyz[0];

    for (int i = 1; i < points.size(); i++)
    {
        if (minx > points[i].m_xyz[0])
        {
            minx = points[i].m_xyz[0];
        }
    }
    return minx;
}

float SortCoords::FindYMin(std::vector<gsml::Vertex>& points)
{
    if (points.size() < 1)
        return -1.f;

    float miny = points[0].m_xyz[1];
    for (int i = 1; i < points.size(); i++)
    {
        if (miny > points[i].m_xyz[1])
        {
            miny = points[i].m_xyz[1];
        }
    }
    return miny;
}

float SortCoords::FindXMax(std::vector<gsml::Vertex>& points)
{
    if (points.size() < 1)
        return -1.f;

    float maxX = points[0].m_xyz[0];
    for (int i = 1; i < points.size(); i++)
    {
        if (maxX < points[i].m_xyz[0])
        {
            maxX = points[i].m_xyz[0];
        }
    }
    return maxX;
}

float SortCoords::FindYMax(std::vector<gsml::Vertex>& points)
{
    if (points.size() < 1)
        return -1.f;

    float maxY = points[0].m_xyz[1];
    for (int i = 1; i < points.size(); i++)
    {
        if (maxY < points[i].m_xyz[1])
        {
            maxY = points[i].m_xyz[1];
        }
    }
    return maxY;
}
