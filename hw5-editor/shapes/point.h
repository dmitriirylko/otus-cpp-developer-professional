#pragma once

#include <iostream>

#include "shape.h"

/**
 * Represents point in x-y 2d coordinates.
 */
class Point : public IShape
{
public:
    /**
     * @brief Creates point with zero x, y coordinates.
     */
    Point() : m_x{0}, m_y{0}
    {}
    /**
     * @brief Constructs point from x and y coordinates.
     */
    Point(int x, int y) : m_x{x}, m_y{y}
    {}

    /**
     * @brief Renders point.
     */
    void draw() override
    {
        std::cout << "Point has been drawn." << std::endl;
    }

    /**
     * @brief X coordinate getter.
     * @return X coordinate of the point.
     */
    int getX() const
    {
        return m_x;
    }

    /**
     * @brief Y coordinate getter.
     * @return Y coordinate of the point.
     */
    int getY() const
    {
        return m_y;
    }

private:
    /**
     * @brief X screen coordinate of point.
     */
    int m_x;
    /**
     * @brief Y screen coordinate of point.
     */
    int m_y;
};