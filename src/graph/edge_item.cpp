/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
 *      - 2020-2021 Rhea Rinaldo (Rhea@Odlanir.de)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "edge_item.h"
#include "node_item.h"
#include "edge.h"

#include "logger.h"

#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QPolygonF>
#include <QRect>
#include <QtMath>

namespace graph
{
    
EdgeItem::EdgeItem(ComponentType type, NodeItem* start, NodeItem* end, QGraphicsItem* parent) :
    QGraphicsLineItem(parent),
    mStartNodeItem(start),
    mEndNodeItem(end),
    mLineColour(Qt::black),
    mType(type),
    mOffset(0.0),
    mArrowSize(20)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    // initToDefaultPosition(); // calculates the position between the given start and end node
    initBrush();
}

EdgeItem::EdgeItem(ComponentType type, QGraphicsItem* parent) :
    QGraphicsLineItem(parent), mLineColour(Qt::black), mType(type), mArrowSize(20)
{
    initBrush();
}

EdgeItem::~EdgeItem()
{
}

void
EdgeItem::initBrush()
{
    switch (mType)
    {
        case ComponentType::reachEdge:
            mArrowColour = Qt::white;
            mLinePen = (QPen(mLineColour, 2, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));
            mArrowHeadPen = (QPen(mLineColour, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            break;
        case ComponentType::functionalEdge:
            mArrowColour = Qt::black;
            mLinePen = (QPen(mLineColour, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            mArrowHeadPen = (QPen(mLineColour, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            break;
        case ComponentType::securityEdge:
            mArrowColour = Qt::black;
            mLinePen = (QPen(mLineColour, 2, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
            mArrowHeadPen = (QPen(mLineColour, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            break;
        default: break;
    }
}

QRectF
EdgeItem::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(),
                  QSizeF(line().p2().x() - line().p1().x(), line().p2().y() - line().p1().y()))
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

QPainterPath
EdgeItem::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(mArrowHead);
    return path;
}

int
EdgeItem::type() const
{
    return EdgeItem::Type;
}

ComponentType
EdgeItem::getComponentType()
{
    return mType;
}

void
EdgeItem::removeLinks()
{
    mStartNodeItem->removeEdgeItem(this);
    mEndNodeItem->removeEdgeItem(this);
}

void
EdgeItem::updatePosition()
{
    QLineF line(mapFromItem(mStartNodeItem, 0, 0), mapFromItem(mEndNodeItem, 0, 0));
    setLine(line);
}

void
EdgeItem::setOffset(double offset)
{
    mOffset = offset;
}

void
EdgeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    if (mStartNodeItem->collidesWithItem(mEndNodeItem))
    {
        return;  // TODO error ? Forbid setting two nodes overlapping
    }

    // setLine(QLineF(mStartPoint, mEndPoint));
    QPointF startPoint = mStartNodeItem->pos();
    QPointF endPoint = mEndNodeItem->pos();
    double angle = std::atan2((endPoint - startPoint).y(), (endPoint - startPoint).x());

    double radius = NodeItem::Radius;
    QPointF radiusPoint {radius, radius};
    startPoint = mStartNodeItem->pos() + radiusPoint
                 + QPointF((cos(angle) * radius), sin(angle) * radius);
    endPoint =
            mEndNodeItem->pos() + radiusPoint + QPointF(-cos(angle) * radius, -sin(angle) * radius);

    QPointF arrowP1 = endPoint
                      + QPointF(sin(-angle + M_PI / 2.7 - M_PI) * mArrowSize,
                                cos(-angle + M_PI / 2.7 - M_PI) * mArrowSize);
    QPointF arrowP2 =
            endPoint
            + QPointF(sin(-angle - M_PI / 2.7) * mArrowSize, cos(-angle - M_PI / 2.7) * mArrowSize);
    mArrowHead.clear();
    mArrowHead << endPoint << arrowP1 << arrowP2;

    QLineF drawingLine = QLineF(startPoint, endPoint);
    QLineF normalVector = line().unitVector().normalVector();
    normalVector.setLength(mOffset);
    drawingLine.translate(normalVector.dx(), normalVector.dy());
    mArrowHead.translate(normalVector.dx(), normalVector.dy());
    //    moveBy(normalVector.dx(), normalVector.dy());

    setLine(drawingLine);

    // Draw Line with Line properties
    painter->setPen(mLinePen);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawLine(line());
    painter->setPen(mArrowHeadPen);
    painter->setBrush(mArrowColour);
    painter->drawPolygon(mArrowHead);

    if (isSelected())
    {
        painter->setPen(QPen(mLineColour, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0, -8.0);
        painter->drawLine(myLine);
    }
}

QPixmap
EdgeItem::image() const
{
    QPixmap pixmap(220, 220);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPointF p1 = QPointF(5, 120);    // line start
    QPointF p2 = QPointF(170, 120);  // line end
    QPointF p3 = QPointF(170, 160);  // arrowhead point top line
    QPointF p4 = QPointF(170, 80);   // arrowhead point bot line
    QPointF p5 = QPointF(220, 120);  // arrowhead point end
    QPointF p6 = QPointF(220, 120);  // arrowhead point end
    QPainterPath path;

    if (mType == ComponentType::reachEdge)
    {
        // Draw icon hacky AF
        painter.setPen(QPen(mLineColour, 8, Qt::DashLine));
        painter.drawLine(p1, p2);
        path.moveTo(p3);
        path.lineTo(p4);
        path.lineTo(p5);
        path.lineTo(p6);
        painter.fillPath(path, QBrush(mArrowColour));
        painter.setPen(QPen(mLineColour, 8, Qt::SolidLine));
        painter.drawLine(p3, p4);
        painter.drawLine(p3, p6);
        painter.drawLine(p4, p5);
    }
    else if (mType == ComponentType::functionalEdge)
    {
        painter.setPen(QPen(mLineColour, 8, Qt::SolidLine));
        painter.drawLine(p1, p2);
        path.moveTo(p3);
        path.lineTo(p4);
        path.lineTo(p5);
        path.lineTo(p6);
        painter.fillPath(path, QBrush(mArrowColour));
    }
    else if (mType == ComponentType::securityEdge)
    {
        painter.setPen(QPen(mLineColour, 8, Qt::DotLine));
        painter.drawLine(p1, p2);
        path.moveTo(p3);
        path.lineTo(p4);
        path.lineTo(p5);
        path.lineTo(p6);
        painter.fillPath(path, QBrush(mArrowColour));
    }
    return pixmap;
}

void
EdgeItem::openSettings()
{
}
}  // namespace graph
