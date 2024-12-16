#pragma once

#include <functional>

/**
 * @brief Template for gui class. Only contains gui callbacks for handling different events.
 */
class Ui
{
public:
    /**
     * @brief Sets document creation ui event handler.
     * @param callback Callback function passed by controller.
     */
    void setCreateDocumentCallback(const std::function<void()>& callback)
    {
        m_createDocumentCallback = callback;
    }

    /**
     * @brief Sets importing document from file ui event handler.
     * @param callback Callback function passed by controller.
     */
    void setImportDocumentCallback(const std::function<void(const std::string &path)>& callback)
    {
        m_importDocumentCallback = callback;
    }

    /**
     * @brief Sets exporting document to file ui event handler.
     * @param callback Callback function passed by controller.
     */
    void setExportDocumentCallback(const std::function<void(const std::string &path)>& callback)
    {
        m_exportDocumentCallback = callback;
    }

    /**
     * @brief Sets point addition ui event handler.
     * @param callback Callback function passed by controller.
     */
    void setAddPointCallback(const std::function<void(int x, int y)>& callback)
    {
        m_addPointCallback = callback;
    }

    /**
     * @brief Sets triangle addition ui event handler.
     * @param callback Callback function passed by controller.
     */
    void setAddTriangleCallback(const std::function<void(const Point &pt1,
                                                         const Point &pt2,
                                                         const Point &pt3)>& callback)
    {
        m_addTriangleCallback = callback;
    }

    /**
     * @brief Sets geometrical shape deletion ui event handler.
     * @param callback Callback function passed by controller.
     */
    void setDeleteShapeCallback(const std::function<void()>& callback)
    {
        m_deleteShapeCallback = callback;
    }

    /**
     * @brief Simulates document creation from gui.
     */
    void testCreateDocument()
    {
        if(m_createDocumentCallback) m_createDocumentCallback();
    }

    /**
     * @brief Simulates point addition from gui.
     */
    void testAddPoint()
    {
        if(m_addPointCallback) m_addPointCallback(10, 10);
    }

    /**
     * @brief Simulates triangle addition from gui.
     */
    void testAddTriangle()
    {
        if(m_addTriangleCallback) m_addTriangleCallback(Point(20, 20),
                                                        Point(40, 20),
                                                        Point(20, 40));
    }

    /**
     * @brief Simulates shape deletion from gui.
     */
    void testDeleteShape()
    {
        if(m_deleteShapeCallback) m_deleteShapeCallback();
    }

private:
    /**
     * @brief Handles document creation ui event.
     */
    std::function<void()> m_createDocumentCallback;
    
    /**
     * @brief Handles importing document from file ui event.
     */
    std::function<void(const std::string &path)> m_importDocumentCallback;
    
    /**
     * @brief Handles exporting document to file ui event.
     */
    std::function<void(const std::string &path)> m_exportDocumentCallback;
    
    /**
     * @brief Handles point addition ui event.
     */
    std::function<void(int x, int y)> m_addPointCallback;
    
    /**
     * @brief Handles triangle addition ui event.
     */
    std::function<void(const Point &pt1, const Point &pt2, const Point &pt3)> m_addTriangleCallback;
    
    /**
     * @brief Handles geometrical shape deletion ui event.
     */
    std::function<void()> m_deleteShapeCallback;
};