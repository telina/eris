/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
 *      - 2021-2021 Mohammed Hussam Al Turjman (Mohammed_Hussam.Al_Turjman@dfki.de)
 *      - 2022-2023 Rhea Rinaldo (Rhea@Odlanir.de)
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

#include "graphics_view.h"

#include <QApplication>
#include <QMouseEvent>

namespace widgets
{
    
GraphicsView::GraphicsView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent),
        mActiveArea(false), mUserActionsEnabled(false), mMouseLeftPressed(false), 
        mMouseMiddlePressed(false)
{
    mRubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    // Uncomment to change the color of the selection area
    // QPalette pal;
    // pal.setBrush(QPalette::Highlight, QBrush(Qt::white));
    // rubberBand->setPalette(pal);
    setTransformationAnchor(QGraphicsView::NoAnchor); // required for middle mouse moving
}

void
GraphicsView::mousePressEvent(QMouseEvent* event)
{
    if (mUserActionsEnabled && event->button() == Qt::MouseButton::LeftButton 
            && !mMouseMiddlePressed)
    {
        if (this->underMouse())
        {
            mMouseLeftPressed = true;
            mOrigin = event->pos();
            mRubberBand->setGeometry(QRect(mOrigin, QSize()));
            mRubberBand->show();
        }
    }
    else if (event->button() == Qt::MouseButton::MiddleButton && !mMouseLeftPressed)
    {
        mMouseMiddlePressed = true;
        mOrigin = event->pos();
    }
    
    QGraphicsView::mousePressEvent(event);

}

void
GraphicsView::mouseMoveEvent(QMouseEvent* event)
{

    if (mUserActionsEnabled && mMouseLeftPressed)
    {
        if ((event->buttons() & ~Qt::MiddleButton))
        {
            mRubberBand->setGeometry(QRect(mOrigin, event->pos()).normalized());
            mActiveArea = true;
        }
    }
    else if (mMouseMiddlePressed)
    {
        QPointF translation = event->pos() - mOrigin;
        translate(translation.x(), translation.y());

        mOrigin = event->pos(); 
    }

    
    QGraphicsView::mouseMoveEvent(event);
}

void
GraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    if (mUserActionsEnabled && mMouseLeftPressed)
    {
        QPolygonF selectionArea = mapToScene(QRect(mOrigin, event->pos()).normalized());
        emit areaSelected(selectionArea);  // mRubberBand->geometry());

        mOrigin = mRubberBand->mapToParent(QPoint(0, 0));
        mEndPoint = mRubberBand->mapToParent(mRubberBand->rect().bottomRight());
        mActiveArea = !(abs(mOrigin.x() - mEndPoint.x()) <= 2 && abs(mOrigin.y() - mEndPoint.y()) <= 2);
        mRubberBand->hide();
    }
    mMouseLeftPressed = false;
    mMouseMiddlePressed = false;

    QGraphicsView::mouseReleaseEvent(event);
}

void
GraphicsView::wheelEvent(QWheelEvent* event)
{
    if (QApplication::keyboardModifiers() & Qt::ControlModifier)
    {
        const QGraphicsView::ViewportAnchor anchor = transformationAnchor();
        this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        qreal factor;
        if (event->angleDelta().y() > 0)
        {
            factor = 1.1;
        }
        else
        {
            factor = 0.9;
        }
        scale(factor, factor);
        setTransformationAnchor(anchor);
    }
    else
    {// If not zooming, enable default (scrolling)
        QGraphicsView::wheelEvent(event);
    }
}

void
GraphicsView::enableUserActions(bool flag)
{
    mUserActionsEnabled = flag;
}


}  // namespace widgets