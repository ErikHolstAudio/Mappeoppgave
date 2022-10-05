#ifndef SORTCOORDS_H
#define SORTCOORDS_H
#include "glm/glm.hpp"
#include <algorithm>
#include "Matrix4x4/vertex.h"
#include <vector>

class SortCoords
{
public:

    static bool compareX(gsml::Vertex V1, gsml::Vertex V2);
    static bool compareY(gsml::Vertex V1, gsml::Vertex V2);

    static void SortX(std::vector<gsml::Vertex>& points);
    static void SortY(std::vector<gsml::Vertex>& points);

    static float FindXMin(std::vector<gsml::Vertex>& points);
    static float FindYMin(std::vector<gsml::Vertex>& points);

    static float FindXMax(std::vector<gsml::Vertex>& points);
    static float FindYMax(std::vector<gsml::Vertex>& points);
};
#endif // SORTCOORDS_H
