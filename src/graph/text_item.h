/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
 *
 * Authors:
 *      - 2020-2021 Rhea Rinaldo (Rhea@Rinaldo.biz)
 *      - 2021-2021 Mohammed Hussam Al Turjman (Mohammed_Hussam.Al_Turjman@dfki.de)
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

#ifndef ERIS_GRAPH_TEXT_ITEM_H
#define ERIS_GRAPH_TEXT_ITEM_H

#include <QFont>
#include <QGraphicsTextItem>
#include <QPen>
#include "eris_config.h"

QT_BEGIN_NAMESPACE
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

namespace graph
{
/**
 * Class represents a text item in the graphics scene and thus inherits a QGraphicsTextItem.
 */
class ERIS_EXPORT TextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    /**
     * Creates a new text item for display in the scene.
     * @param text data that shall be displayed
     * @param position coordinates of the text
     * @param parent
     */
    TextItem(const QString& text, QPointF position, QGraphicsItem* parent = 0);
    ~TextItem() override = default;

    /** Determines what kind of item this GraphicsItem is */
    static int const Type = UserType + 3;

    /**
     *  Returns the kind of type of the Graphics Item, in this case, NodeType.
     */
    int
    type() const override;

protected:
    void
    mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
};

}  // namespace graph

#endif /* ERIS_GRAPH_TEXT_ITEM_H */
