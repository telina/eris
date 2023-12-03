/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
 *
 * Authors:
 *      - 2021-2021 Mohammed Hussam Al Turjman (Mohammed_Hussam.Al_Turjman@dfki.de)
 *      - 2022-2023 Rhea Rinaldo (Rhea@Rinaldo.biz)
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

// source : https://github.com/MagedMilad/Image-Processing

#ifndef ERIS_WIDGETS_GRAPHIC_SCENE_GRAPHICS_VIEW_H
#define ERIS_WIDGETS_GRAPHIC_SCENE_GRAPHICS_VIEW_H

#include <QGraphicsView>
#include <QRubberBand>
#include <atomic>

namespace widgets
{
// Extend some functionalities in the QGraphicsView to
// support area selection
class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QGraphicsScene* scene, QWidget* parent = nullptr);
    
    /**
     * Is called whenever the GraphicScene switches into the Navigate mode. Only in this mode
     * GraphicView actions are available!
     * @param flag true enables actions, false disables actions
     */
    void
    enableUserActions(bool flag);

protected:
    
    /**
     * Overwritten to enable selection via rubberband area.
     * Is only possible if user actions are available (GraphicScene Navigate mode)
     * @param event
     */
    void
    mousePressEvent(QMouseEvent* event) override;

    /**
     * Overwritten to enable selection via rubberband area.
     * Is only possible if user actions are available (GraphicScene Navigate mode).
     * This method adjusts the selection area (rect) while the mouse is moving
     * @param event
     */
    void
    mouseMoveEvent(QMouseEvent* event) override;

    /**
     * Overwritten to enable selection via rubberband area.
     * Is only possible if user actions are available (GraphicScene Navigate mode).
     * This method emits a signal when the selection is completed and cleans up the selection area.
     * @param event
     */
    void
    mouseReleaseEvent(QMouseEvent*) override;
    
    /**
     * Overwritten function to enable zoom in and zoom out actions.
     * Is only possible if user actions are available (GraphicScene Navigate mode) and ctrl button
     * is held down!
     * @param event
     */
    void
    wheelEvent(QWheelEvent* event) override;
    
signals:

    void
    areaSelected(const QPolygonF&);

private:

    
    /** Selection Area*/
    QRubberBand* mRubberBand;
    
    bool mActiveArea;
    
    /** Flag to indicate that certain user actions are available*/
    bool mUserActionsEnabled;
    
    QPoint mOrigin, mEndPoint;
        
    bool mMouseLeftPressed;
    
    bool mMouseMiddlePressed;
};
}  // namespace widgets

#endif  // ERIS_WIDGETS_GRAPHIC_SCENE_GRAPHICS_VIEW_H
