#include <iostream>
#include <memory>

#include "document.h"
#include "ui.h"

class Controller
{
public:
    Controller(const DocumentManager &manager, const Ui &ui) :
        m_model{const_cast<DocumentManager&>(manager)},
        m_view{const_cast<Ui&>(ui)}
    {
        m_view.setCreateDocumentCallback([this](){
            bool res;
            std::tie(res, m_activeDoc) = m_model.create();
        });

        m_view.setExportDocumentCallback([this](const std::string &path){
            bool res = m_model.exportToFile(m_activeDoc, path);
        });

        m_view.setImportDocumentCallback([this](const std::string& path){
            bool res;
            std::tie(res, m_activeDoc) = m_model.importFromFile(path);
            m_activeDoc.lock()->draw();
        });

        m_view.setAddPointCallback([this](int x, int y){
            bool res;
            auto activeDoc = m_activeDoc.lock();
            std::tie(res, m_activeShape) = activeDoc->addPoint(x, y);
            activeDoc->draw();
        });

        m_view.setAddTriangleCallback([this](const Point &pt1, const Point &pt2, const Point &pt3){
            bool res;
            auto activeDoc = m_activeDoc.lock();
            std::tie(res, m_activeShape) = activeDoc->addTriangle(pt1, pt2, pt3);
            activeDoc->draw();
        });

        m_view.setDeleteShapeCallback([this](){
            auto activeDoc = m_activeDoc.lock();
            bool res = activeDoc->deleteShape(m_activeShape);
            activeDoc->draw();
        });
    }

private:
    DocumentManager &m_model;
    Ui &m_view;
    std::weak_ptr<DocumentHandler> m_activeDoc;
    std::weak_ptr<IShape> m_activeShape;
};

int main()
{
    DocumentManager docManager;
    Ui ui;
    Controller(docManager, ui);

    /* Simulate gui events. */
    ui.testCreateDocument();
    ui.testAddPoint();
    ui.testAddTriangle();
    ui.testDeleteShape();

    return 0;
}