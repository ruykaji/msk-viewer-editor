#include "DrawData.hpp"

void selectPenAndBrush(const Rect::Material t_material, const double& t_lineWidth, QPainter* t_painter, const uint8_t t_opacity, const bool t_penOpacity)
{
    switch (t_material) {
    case Rect::Material::NW: {
        t_painter->setPen(QPen(QColor(55, 146, 145, t_penOpacity ? t_opacity : 255), t_lineWidth, Qt::SolidLine, Qt::SquareCap));
        t_painter->setBrush(QBrush(QColor(55, 146, 55, t_opacity), Qt::Dense4Pattern));
        break;
    }
    case Rect::Material::DN: {
        t_painter->setPen(QPen(QColor(0, 128, 0, t_penOpacity ? t_opacity : 255), t_lineWidth, Qt::SolidLine, Qt::SquareCap));
        t_painter->setBrush(QBrush(QColor(0, 128, 0, t_opacity)));
        break;
    }
    case Rect::Material::DP: {
        t_painter->setPen(QPen(QColor(128, 128, 0, t_penOpacity ? t_opacity : 255), t_lineWidth, Qt::SolidLine, Qt::SquareCap));
        t_painter->setBrush(QBrush(QColor(128, 128, 0, t_opacity)));
        break;
    }
    case Rect::Material::PO: {
        t_painter->setPen(QPen(QColor(255, 0, 0, t_penOpacity ? t_opacity : 255), t_lineWidth, Qt::SolidLine, Qt::SquareCap));
        t_painter->setBrush(QBrush(QColor(255, 0, 0, t_opacity)));
        break;
    }
    case Rect::Material::PO2: {
        t_painter->setPen(QPen(QColor(255, 0, 0, t_penOpacity ? t_opacity : 255), t_lineWidth, Qt::SolidLine, Qt::SquareCap));
        t_painter->setBrush(QBrush(QColor(255, 0, 0, t_opacity)));
        break;
    }
    case Rect::Material::ME: {
        t_painter->setPen(QPen(QColor(0, 0, 255, t_penOpacity ? t_opacity : 255), t_lineWidth, Qt::SolidLine, Qt::SquareCap));
        t_painter->setBrush(QBrush(QColor(0, 0, 255, t_opacity)));
        break;
    }
    case Rect::Material::CO: {
        t_painter->setPen(QPen(QColor(247, 247, 247, t_penOpacity ? t_opacity : 255), t_lineWidth, Qt::SolidLine, Qt::SquareCap));
        t_painter->setBrush(QBrush(QColor(247, 247, 247, t_opacity)));
        break;
    }
    case Rect::Material::M2: {
        t_painter->setPen(QPen(QColor(0, 0, 129, t_penOpacity ? t_opacity : 255), t_lineWidth, Qt::SolidLine, Qt::SquareCap));
        t_painter->setBrush(QBrush(QColor(0, 0, 129, t_opacity)));
        break;
    }
    case Rect::Material::VI: {
        t_painter->setPen(QPen(QColor(247, 247, 247, t_penOpacity ? t_opacity : 255), t_lineWidth, Qt::SolidLine, Qt::SquareCap));
        t_painter->setBrush(QBrush(QColor(247, 247, 247, t_opacity)));
        break;
    }
    case Rect::Material::M3: {
        t_painter->setPen(QPen(QColor(0, 0, 255, t_penOpacity ? t_opacity : 255), t_lineWidth, Qt::SolidLine, Qt::SquareCap));
        t_painter->setBrush(QBrush(QColor(0, 0, 255, t_opacity), Qt::BDiagPattern));
        break;
    }
    case Rect::Material::V2: {
        t_painter->setPen(QPen(QColor(247, 247, 247, t_penOpacity ? t_opacity : 255), t_lineWidth, Qt::SolidLine, Qt::SquareCap));
        t_painter->setBrush(QBrush(QColor(247, 247, 247, t_opacity)));
        break;
    }
    case Rect::Material::M4: {
        t_painter->setPen(QPen(QColor(52, 252, 52, t_penOpacity ? t_opacity : 255), t_lineWidth, Qt::SolidLine, Qt::SquareCap));
        t_painter->setBrush(QBrush(QColor(52, 252, 52, t_opacity), Qt::FDiagPattern));
        break;
    }
    case Rect::Material::V3: {
        t_painter->setPen(QPen(QColor(247, 247, 247, t_penOpacity ? t_opacity : 255), t_lineWidth, Qt::SolidLine, Qt::SquareCap));
        t_painter->setBrush(QBrush(QColor(247, 247, 247, t_opacity)));
        break;
    }
    case Rect::Material::M5: {
        t_painter->setPen(QPen(QColor(0, 0, 255, t_penOpacity ? t_opacity : 255), t_lineWidth, Qt::SolidLine, Qt::SquareCap));
        t_painter->setBrush(QBrush(QColor(0, 0, 255, t_opacity), Qt::HorPattern));
        break;
    }
    case Rect::Material::V4: {
        t_painter->setPen(QPen(QColor(247, 247, 247, t_penOpacity ? t_opacity : 255), t_lineWidth, Qt::SolidLine, Qt::SquareCap));
        t_painter->setBrush(QBrush(QColor(247, 247, 247, t_opacity)));
        break;
    }
    case Rect::Material::M6: {
        t_painter->setPen(QPen(QColor(128, 128, 128, t_penOpacity ? t_opacity : 255), t_lineWidth, Qt::SolidLine, Qt::SquareCap));
        t_painter->setBrush(QBrush(QColor(128, 128, 128, t_opacity), Qt::Dense3Pattern));
        break;
    }
    case Rect::Material::V5: {
        t_painter->setPen(QPen(QColor(247, 247, 247, t_penOpacity ? t_opacity : 255), t_lineWidth, Qt::SolidLine, Qt::SquareCap));
        t_painter->setBrush(QBrush(QColor(247, 247, 247, t_opacity)));
        break;
    }
    default: {
        t_painter->setPen(QPen(QColor(Qt::white), t_lineWidth, Qt::SolidLine, Qt::SquareCap));
        t_painter->setBrush(QBrush(QColor(Qt::white)));
        break;
    }
    }
};