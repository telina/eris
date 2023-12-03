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

#include "text_item.h"

#include "error_handler.h"
#include "utils.h"

#include <QInputDialog>

using widgets::ErrorHandler;
using widgets::Errors;

namespace graph
{
TextItem::TextItem(const QString& text, QPointF position, QGraphicsItem* parent) :
    QGraphicsTextItem(parent)
{
    this->setPos(position);
    this->setPlainText(text);
    this->setDefaultTextColor(Qt::black);
    // this->setTextInteractionFlags(Qt::TextEditorInteraction);
    QFont font;
    font.setPointSize(12);
    this->setFont(font);
    this->setFlag(QGraphicsItem::ItemIsMovable);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
}

int
TextItem::type() const
{
    return Type;
}

void
TextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    bool ok = false;
    auto res = QInputDialog::getText(nullptr, "Edit dialog", "Text:", QLineEdit::Normal, "", &ok);
    if (ok)
    {
        if (utils::isOnlyWhiteSpaces(res.toStdString()))
        {
            ErrorHandler::getInstance().setError(Errors::invalidTextItem());
            ErrorHandler::getInstance().show();
        }
        else
            setPlainText(res);
    }

    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

}  // namespace graph
