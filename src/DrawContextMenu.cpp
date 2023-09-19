#include "DrawContextMenu.hpp"

DrawContextMenu::DrawContextMenu(QWidget* t_parent):QMenu(t_parent){
        auto nwAction = new QAction(tr("N-Well"), this);
        connect(nwAction, &QAction::triggered, [this](){this->selectDrawingMaterial(Rect::Material::NW);});

        auto ndAction = new QAction(tr("N+ Diffusion"), this);
        connect(ndAction, &QAction::triggered, [this](){this->selectDrawingMaterial(Rect::Material::DN);});

        auto pdAction = new QAction(tr("P+ Diffusion"), this);
        connect(pdAction, &QAction::triggered, [this](){this->selectDrawingMaterial(Rect::Material::DP);});

        auto pAction = new QAction(tr("Polysilicon"), this);
        connect(pAction, &QAction::triggered, [this](){this->selectDrawingMaterial(Rect::Material::PO);});

        auto p2Action = new QAction(tr("Polysilicon-2"), this);
        connect(p2Action, &QAction::triggered, [this](){this->selectDrawingMaterial(Rect::Material::PO2);});
        
        auto m1Action = new QAction(tr("Metal 1"), this);
        connect(m1Action, &QAction::triggered, [this](){this->selectDrawingMaterial(Rect::Material::ME);});

        auto m2Action = new QAction(tr("Metal 2"), this);
        connect(m2Action, &QAction::triggered, [this](){this->selectDrawingMaterial(Rect::Material::M2);});

        auto m3Action = new QAction(tr("Metal 3"), this);
        connect(m3Action, &QAction::triggered, [this](){this->selectDrawingMaterial(Rect::Material::M3);});

        auto m4Action = new QAction(tr("Metal 4"), this);
        connect(m4Action, &QAction::triggered, [this](){this->selectDrawingMaterial(Rect::Material::M4);});

        auto m5Action = new QAction(tr("Metal 5"), this);
        connect(m5Action, &QAction::triggered, [this](){this->selectDrawingMaterial(Rect::Material::M5);});

        auto m6Action = new QAction(tr("Metal 6"), this);
        connect(m6Action, &QAction::triggered, [this](){this->selectDrawingMaterial(Rect::Material::M6);});

        auto layoutMenu = addMenu(tr("&Draw layout"));

        layoutMenu->addAction(nwAction);
        layoutMenu->addAction(ndAction);
        layoutMenu->addAction(pdAction);
        layoutMenu->addAction(pAction);
        layoutMenu->addAction(p2Action);
        layoutMenu->addAction(m1Action);
        layoutMenu->addAction(m2Action);
        layoutMenu->addAction(m3Action);
        layoutMenu->addAction(m4Action);
        layoutMenu->addAction(m5Action);
        layoutMenu->addAction(m6Action);
}