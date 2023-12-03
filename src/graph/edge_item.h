/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
 *
 * Authors:
 *      - 2020-2021 Rhea Rinaldo (Rhea@Rinaldo.biz)
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

#ifndef ERIS_GRAPH_EDGE_ITEM_H
#define ERIS_GRAPH_EDGE_ITEM_H

#include "component_type.h"

#include <QGraphicsLineItem>
#include <QPen>

QT_BEGIN_NAMESPACE
class QColor;
class QPainter;
class QPolygonF;
class QRect;
QT_END_NAMESPACE

namespace graph
{
class NodeItem;

/*
 * This class is the visual representation of an edge that is used in the GUI. An edge exists
 * between two node items to visualise communication dependencies.
 */
class EdgeItem : public QGraphicsLineItem
{
public:
    /**
     * Creates a new EdgeItem
     * @param start node
     * @param end node
     * @param type of edge
     * @param parent of the graphics item
     */
    EdgeItem(graph::ComponentType type, NodeItem* start, NodeItem* end, QGraphicsItem* parent = 0);

    /**
     * Dummy Constructor for painting the buttons.
     * @param type
     * @param parent
     */
    EdgeItem(graph::ComponentType type, QGraphicsItem* parent = 0);

    ~EdgeItem();

    /**
     * Initialises the brush for the object in order to draw the edge item.
     */
    void
    initBrush();

    static int const EdgeType = UserType + 2;

    /**
     * Get the bounding rect of the edge item.
     * @return bounding rect
     */
    QRectF
    boundingRect() const override;

    /**
     * Get the shape of the edge item.
     * @return shape
     */
    QPainterPath
    shape() const override;

    /**
     * Returns the node the edge starts in.
     * @return
     */
    NodeItem*
    startItem() const
    {
        return mStartNodeItem;
    }

    /**
     * Returns the node the edge ends in.
     * @return
     */
    NodeItem*
    endItem() const
    {
        return mEndNodeItem;
    }

    /**
     * Creates the image of edge considering its component type used for static usages such as
     * images of buttons.
     * @return the pixmap of the edge image
     */
    QPixmap
    image() const;

    /**
     *  Get the kind of type of the Graphics Item, in this case, EdgeType.
     *  @return QGraphicsItem type
     */
    int
    type() const override;

    /**
     * Get the component (edge) type of the edge item.
     * @return component type
     */
    graph::ComponentType
    getComponentType();

    /**
     * Removes itself from its attached nodes, in order to be removed from the scene!
     */
    void
    removeLinks();

    /**
     * Set the offset of this edge needed to calculate the correct position when other parallel
     * edges exist.
     * @param offset that shall be set
     */
    void
    setOffset(double offset);

    /**
     * Update the position of the edge item.
     */
    void
    updatePosition();

    /**
     * Not implemented yet
     */
    void
    openSettings();

protected:
    void
    paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override;

private:
    /** Start and End node of the Edge*/
    NodeItem* mStartNodeItem;
    NodeItem* mEndNodeItem;

    /** Polygon of the arrow head */
    QPolygonF mArrowHead;

    /** Colour of the edge line */
    QColor mLineColour;

    /** Colour of the arrow head */
    QColor mArrowColour;

    /** Type of Component */
    graph::ComponentType mType;

    /** size of the arrow */
    double mArrowSize;

    /** Pen object for the line */
    QPen mLinePen;

    /** Pen object for the arrow head */
    QPen mArrowHeadPen;

    /** Offset for double edge calculation */
    double mOffset;
};

}  // namespace graph
#endif /* ERIS_GRAPH_EDGE_ITEM_H */
