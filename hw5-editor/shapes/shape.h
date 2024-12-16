#pragma once

/**
 * @brief Interface class for different shapes
 */
class IShape
{
public:
    IShape() = default;
    virtual ~IShape() = default;
    
    /**
     * @brief Renders shape on screen.
     */
    virtual void draw() = 0;
};