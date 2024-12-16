#pragma once

#include <array>

#include "shape.h"
#include "point.h"

/**
 * Represents triangle in x-y 2d coordinates.
 */
class Triangle : public IShape
{
public:
    /**
     * @brief Traingle default ctor.
     */
    Triangle() = default;

    /**
     * @brief Constructs triangle from three 2d points.
     */
    Triangle(const Point& pt1, const Point& pt2, const Point& pt3)
    {
        m_vertices[0] = pt1;
        m_vertices[1] = pt2;
        m_vertices[2] = pt3;
    }

    /**
     * @brief Renders triangle.
     */
    void draw() override
    {
        std::cout << "Triangle has been drawn." << std::endl;
    }

private:
    /**
     * @brief Holds triangle vertices.
     */
    std::array<Point, 3> m_vertices;
};