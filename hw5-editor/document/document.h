#pragma once

#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <fstream>

#include "shape.h"
#include "point.h"
#include "triangle.h"

/**
 * Manages graphical editor file.
 */
class DocumentHandler
{
public:
    /**
     * Creates document handler.
     */
    DocumentHandler() = default;
    
    /**
     * @brief Saves document to file.
     * @param fs Output file stream.
     */
    void exportToFile(std::ofstream& fs)
    {
        std::cout << "Exporting document to file." << std::endl;
    }

    /**
     * @brief Loads document from file.
     * @param fs Input file stream.
     */
    void importFromFile(std::ifstream& fs)
    {
        std::cout << "Importing document from file." << std::endl;
    }

    /**
     * @brief Renders all shapes of file.
     */
    void draw()
    {
        for(const auto& shape : m_shapes)
        {
            shape->draw();
        }
    }

    /**
     * @brief Deletes shape.
     * @param ptr Shape to delete from document.
     * @return true - shape has been deleted, false - shape has not been deleted.
     */
    bool deleteShape(const std::weak_ptr<IShape> &ptr)
    {
        auto p = ptr.lock();
        if(p)
        {
            auto iter = m_shapes.find(p);
            if(iter != m_shapes.end())
            {
                std::cout << "Delete shape." << std::endl;
                m_shapes.erase(iter);
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
        return true;
    }

    /**
     * @brief Adds point to document.
     * @param x X coordinate of point.
     * @param y Y coordinate of point.
     * @return Tuple with first element: result of point addition (true - point
     *         has been added, false - point has noot been added); second
     *         element - pointer to point object.
     */
    std::tuple<bool, std::weak_ptr<IShape>> addPoint(int x, int y)
    {
        auto [iter, isEmplaced] = m_shapes.emplace(std::make_shared<Point>(x, y));
        if(isEmplaced)
        {
            std::cout << "Add point (" << x << ", " << y << ")." << std::endl;
            return {true, *iter};
        }
        return {false, *iter};
    }

    /**
     * @brief Adds triangle to document.
     * @param pt1 First point of triangle.
     * @param pt2 Second point of triangle.
     * @param pt3 Third point of triangle.
     * @return Tuple with first element: result of triangle addition (true - triangle
     *         has been added, false - triangle has noot been added); second
     *         element - pointer to traingle object.
     */
    std::tuple<bool, std::weak_ptr<IShape>> addTriangle(const Point &pt1,
                                                        const Point &pt2,
                                                        const Point &pt3)
    {
        auto [iter, isEmplaced] = m_shapes.emplace(std::make_shared<Triangle>(pt1, pt2, pt3));
        if(isEmplaced)
        {
            std::cout << "Add triangle [" <<
                         "(" << pt1.getX() << ", " << pt1.getY() << ")," <<
                         "(" << pt2.getX() << ", " << pt2.getY() << ")," <<
                         "(" << pt3.getX() << ", " << pt3.getY() << ")" <<
                         "]." << std::endl;
            return {true, *iter};
        }
        return {false, *iter};
    }

private:
    /**
     * @brief Holds all shapes present in graphical document;
     */
    std::set<std::shared_ptr<IShape>> m_shapes;
};

/**
 * @brief Manages operations with documents: creates, deletes, openes
 *        documents and so on.
 */
class DocumentManager
{
public:
    DocumentManager() = default;

    /**
     * @brief Creates documents with specified path.
     * @return Tuple with first element: result of file creation (true - document
     *         has been created, false - document has noot been created); second
     *         element - pointer to document's handler.
     */
    std::tuple<bool, std::weak_ptr<DocumentHandler>> create()
    {
        std::cout << "Create file." << std::endl;
        auto [iter, isEmplaced] = m_docs.emplace(std::make_shared<DocumentHandler>());
        if(isEmplaced) return {true, *iter};
        return {false, *iter};
    }

    /**
     * @brief Saves graphical document to file.
     * @param ptr Pointer to document handler.
     * @param path Path to file.
     * @return Result of file exporting (true - document has been saved, false -
     *         document has noot been saved).
     */
    bool exportToFile(const std::weak_ptr<DocumentHandler> &ptr, const std::string &path)
    {
        auto p = ptr.lock();
        if(p)
        {
            if(m_docs.find(p) != m_docs.end())
            {
                std::ofstream fs;
                fs.open(path);
                if(fs.is_open())
                {
                    p->exportToFile(fs);
                }
                else
                {
                    return false;
                }
                if(fs.is_open()) fs.close();
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
        return true;
    }

    /**
     * @brief Loads document from previously saved file.
     * @param path Path to file.
     * @return Result of file importing (true - document has been loaded, false -
     *         document has noot been loaded).
     */
    std::tuple<bool, std::weak_ptr<DocumentHandler>> importFromFile(const std::string &path)
    {
        bool res;
        std::weak_ptr<DocumentHandler> ptr;
        std::ifstream fs;
        fs.open(path);
        if(fs.is_open())
        {
            std::tie(res, ptr) = create();
            auto p = ptr.lock();
            p->importFromFile(fs);
        }
        else
        {
            return {false, *(m_docs.end())};
        }
        if(fs.is_open()) fs.close();
        return {true, std::move(ptr)};
    }

private:
    /**
     * @brief Holds document handler instances proprietary.
     */
    std::set<std::shared_ptr<DocumentHandler>> m_docs;
};