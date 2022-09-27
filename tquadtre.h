#ifndef TQUADTRE_H
#define TQUADTRE_H

#include <iostream>
#include <vector>
#include <list> // std::list er mer egnet til sletting
#include <string>
#include <iterator>
#include <boundingshape.h>
#include "square.h"

#include "glm/glm.hpp"

namespace gsml
{
typedef std::pair<double, double> Point2D; // Erstatter Vector2D klasse

// Template klasse, laget for ID og T
// ID er ikke implementert ennå
// Bruker Point2D - se typedef ovenfor - i stedet for Vector2D
template <class ID, class T>
class QuadTre
{

public:
    // Punkter som definerer hjørnene til objektet
	// i samme rekkefølge som pekerne til subtrær
    Point2D m_a;
    Point2D m_b;
    Point2D m_c;
    Point2D m_d;
    // Pekere til subtrær er nødvendig i en dynamisk
    // quadtre implementering. Vi kunne ha lagt det
    // i array også
    QuadTre* m_sv;
    QuadTre* m_so;
    QuadTre* m_no;
    QuadTre* m_nv;

    AABB* square{nullptr};
    bool isLeaf() const;

private:
   // Hvert quad lagrer sine egne objekter
   // (T kan være objekter, pekere eller referanser)
   // Legger opp til mulig søk på id
   std::list<std::pair<ID,T>> m_objects;


   // Hjelpevariabel for å lagre alle T-ene
   // for senere iterering
   std::vector<T> m_all_objects; // skulle vært static

   // Hjelpefunksjon - preorder traversering av subtre
   void traverse_all(std::vector<T>& all_objects);


public:
    QuadTre();
    QuadTre(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D & v4);
    ~QuadTre();
    void deleteChildren();
    void init(const Point2D &v1, const Point2D &v2, const Point2D &v3, const Point2D & v4);
	void subDivide(int n);
    void print() const;
    void print_all() const;
	QuadTre* insert(const Point2D& point, const ID& id, const T& object);
    QuadTre* insert(BoundingShape* shape, const ID& id, const T& object);

    QuadTre* find(const Point2D& p);
    const QuadTre* find(const Point2D& p) const;
    T& find(const ID& id);
    T& operator[] (ID id);

   // Jukser litt med disse iteratorene. Det er ikke ekte iteratorfunksjoner
   // for QuadTre klassen, men for en container av T-er
   typename std::vector<T>::iterator begin()
   {
       traverse_all(m_all_objects);
       return m_all_objects.begin();
   }
   typename std::vector<T>::iterator end()
   {
       return m_all_objects.end();
   }
};

template <class ID, class T>
bool QuadTre<ID, T>::isLeaf() const
{
    if (m_sv == nullptr && m_so == nullptr && m_no == nullptr && m_nv == nullptr)
        return true;
    else
        return false;
    
}

template <class ID, class T>
void QuadTre<ID, T>::traverse_all(std::vector<T>& all_objects)
{
    // m_all_objects.clear();
    for (auto it = m_objects.begin(); it != m_objects.end(); it++)
        all_objects.push_back(it->second);
    if (m_no) m_no->traverse_all(all_objects);
    if (m_nv) m_no->traverse_all(all_objects);
    if (m_sv) m_no->traverse_all(all_objects);
    if (m_so) m_no->traverse_all(all_objects);

}

template <class ID, class T>
QuadTre<ID, T>::QuadTre() : m_sv(nullptr), m_so(nullptr), m_no(nullptr), m_nv{ nullptr }
{
    square = new AABB();
}

template <class ID, class T>
QuadTre<ID, T>::QuadTre(const Point2D& v1, const Point2D& v2, const Point2D& v3, const Point2D& v4)
	: m_a{v1}, m_b{v2}, m_c{v3}, m_d{v4}, m_sv(nullptr), m_so(nullptr), m_no(nullptr), m_nv{nullptr}
{
    square = new AABB();
}

template <class ID, class T>
QuadTre<ID, T>::~QuadTre()
{
	delete m_nv;
    delete m_no;
    delete m_sv;
    delete m_so;
}

template <class ID, class T>
void QuadTre<ID, T>::deleteChildren()
{
    delete m_nv;
    delete m_no;
    delete m_sv;
    delete m_so;
}
template <class ID, class T>
void QuadTre<ID, T>::init(const Point2D& v1, const Point2D& v2, const Point2D& v3, const Point2D& v4)
{
    m_a = v1;
    m_b = v2;
    m_c = v3;
    m_d = v4;

}

template <class ID, class T>
void QuadTre<ID, T>::subDivide(int n)
{
	if (n>0)
	{
        Point2D v1 = Point2D{ (m_a.first + m_b.first) / 2, (m_a.second + m_b.second) / 2 };
        Point2D v2 = Point2D{ (m_b.first + m_c.first) / 2, (m_b.second + m_c.second) / 2 };
        Point2D v3 = Point2D{ (m_c.first + m_d.first) / 2, (m_c.second + m_d.second) / 2 };
        Point2D v4 = Point2D{ (m_d.first + m_a.first) / 2, (m_d.second + m_a.second) / 2 };
        Point2D m = Point2D{ (m_a.first + m_c.first) / 2, (m_a.second + m_c.second) / 2 };

        m_sv = new QuadTre(m_a, v1, m, v4);
        m_sv->subDivide(n - 1);
        m_so = new QuadTre(v1, m_b, v2, m);
        m_so->subDivide(n - 1);
        m_no = new QuadTre(m, v2, m_c, v3);
        m_no->subDivide(n - 1);
        m_nv = new QuadTre(v4, m, v3, m_d);
        m_nv->subDivide(n - 1);
	}
}

template <class ID, class T>
void QuadTre<ID, T>::print() const
{
    Point2D c = Point2D{ (m_a.first + m_c.first) / 2, (m_a.second + m_c.second) / 2 };
    std::cout << std::endl
		<< "QuadTre-sentrum = (" << c.first << ", " << c.second << ") " << std::endl;
    for (auto it = m_objects.begin(); it != m_objects.end(); it++)
    {
        std::cout << it->first << ": " << it->second->getPosition2D().first << ", " << it->second->getPosition2D().second << ", bShape pos x: " << it->second->bShape->position.x << ", y: " << it->second->bShape->position.y <<  std::endl;
    }

}

template <class ID, class T>
void QuadTre<ID, T>::print_all() const
{
    print();
    if (m_no) m_no->print_all();
    if (m_nv) m_nv->print_all();
    if (m_sv) m_sv->print_all();
    if (m_so) m_so->print_all();

}

template <class ID, class T>
QuadTre<ID, T>* QuadTre<ID, T>::insert(const Point2D& point, const ID& id, const T& object)
{
    if (isLeaf())
    {
        for (auto it = m_objects.begin(); it != m_objects.end(); it++)
            if ((*it).first == id)
                return this;

        m_objects.push_back(std::pair<ID, T>{id, object});
        return this;
    }
    else
    {
        Point2D m = Point2D{ (m_a.first + m_c.first) / 2, (m_a.second + m_c.second) / 2 };

    	if (point.second < m.second)
	    {
            if (point.first < m.first)
                m_sv->insert(point, id, object);
            else
                m_so->insert(point, id, object);

	    }
        else
        {
            if (point.first < m.first)
                m_nv->insert(point, id, object);
            else
                m_no->insert(point, id, object);

        }
        return nullptr;
    }

}

template <class ID, class T>
QuadTre<ID, T>* QuadTre<ID, T>::insert(BoundingShape* shape, const ID& id, const T& object)
{
    if (isLeaf())
    {
        // Makes sure subtre has no duplicate objects
        for (auto it = m_objects.begin(); it != m_objects.end(); it++)
            if ((*it).first == id)
                return this;

        m_objects.push_back(std::pair<ID, T>{id, object});
        return this;
    }
    else
    {
        if (shape->overlap(m_sv->square))
            m_sv->insert(shape, id, object);

        if (shape->overlap(m_so->square))
            m_so->insert(shape, id, object);

        if (shape->overlap(m_no->square))
            m_no->insert(shape, id, object);

        if (shape->overlap(m_nv->square))
            m_nv->insert(shape, id, object);

        return nullptr;
    }
}

template <class ID, class T>
QuadTre<ID, T>* QuadTre<ID, T>::find(const Point2D& p)
{
    if (isLeaf())
        return this;
    else
    {
        Point2D m = Point2D{ (m_a.first + m_c.first) / 2, (m_a.second + m_c.second) / 2 };

        if (p.second < m.second)
        {
            if (p.first < m.first)
                m_sv->find(p);

            else
                m_so->find(p);
        }
        else
        {
            if (p.first < m.first)
                m_nv->find(p);
	        else
                m_no->find(p);
        }
    }
}

template <class ID, class T>
T& QuadTre<ID, T>::find(const ID& id)
{
    if (isLeaf() && m_objects.size() > 0)
    {
        for (auto it = m_objects.begin(); it != m_objects.end(); it++)
        {
            if (it->first == id)
                return *it->second;
        }
    }
    else if (m_no)
        m_no->find(id);
    else if (m_nv)
        m_nv->find(id);
    else if (m_sv)
        m_sv->find(id);
    else if (m_so)
        m_so->find(id);
}

template <class ID, class T>
T& QuadTre<ID, T>::operator[](ID id)
{
	if (isLeaf())
	{
		for (auto it=m_objects.begin(); it!=m_objects.end(); it++)
		{
            if (it->first = id)
                return it->second;
		}
	}
}
}

#endif // TQUADTRE_H