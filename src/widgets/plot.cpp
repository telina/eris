/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
 *
 * Authors:
 *      - 2021-2021 Mohammed Hussam Al Turjman (Mohammed_Hussam.Al_Turjman@dfki.de)
 *      - 2023 Rhea Rinaldo (Rhea@Rinaldo.biz)
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

#include "plot.h"

#include "checks.h"

#include <QtCharts/QLegendMarker>
#include <QtCharts/QXYLegendMarker>

namespace widgets
{
Plot::Plot(QString xAxisName, QString yAxisName, QObject* parent, const QString& title) : 
    QObject(parent), mXAxisName(xAxisName), mYAxisName(yAxisName), mSeries()
{
    mChart = utils::allocateMemoryBlock<QChart>();

    ERIS_CHECK(mChart.get());

    setTitle(title);
    setLegendAlignment(Qt::AlignRight);
    showLegend(true);

    QMap<QString, QList<QPointF>> initializer = {{"Test", {{0,1}}}};
    addLineSeries(initializer);
    clear();
}

Plot::~Plot()
{
    clear();
}

void
Plot::showLegend(bool show)
{
    mChart->legend()->setVisible(show);
}

void
Plot::addLineSeries(const QMap<QString, QList<QPointF>> results)
{
    auto end = results.cend();
    for (auto it = results.cbegin(); it != end; ++it)
    {
        QLineSeries* series = utils::allocateMemoryBlock<QLineSeries>().release();

        if (series)
        {
            series->setName(it.key());
            series->append(it.value());
            mSeries.append(series);
            mChart->addSeries(series);

            // FIXME : find another way to connect the current marker
            connectMarkers();

            mChart->createDefaultAxes();
        }
    }
    nameAxis();    
}

void
Plot::clear()
{
    disconnectMarkers();

    while (!mSeries.empty())
    {
        QLineSeries* series = mSeries.last();
        mChart->removeSeries(series);
        mSeries.removeLast();
        //delete series;
    }
}
void
Plot::nameAxis()
{
    QAbstractAxis* axis = mChart->axes(Qt::Vertical).first();
    if (axis)
    {
        axis->setTitleText(mYAxisName);
    }
    else
    {
        PRINT_WARNING("Chart does not have y axis");
    }
    
    axis = mChart->axes(Qt::Horizontal).first();
    if (axis)
    {
        axis->setTitleText(mXAxisName);
    }
    else
    {
        PRINT_WARNING("Chart does not have x axis");
    }
}

void
Plot::setTitle(const QString& title)
{
    mChart->setTitle(title);
}

void
Plot::setLegendAlignment(Qt::AlignmentFlag alignment)
{
    mChart->legend()->setAlignment(alignment);
}

void
Plot::connectMarkers()
{
    // Connect all markers to handler
    const auto markers = mChart->legend()->markers();

    for (QLegendMarker* marker : markers)
    {
        // Disconnect possible existing connection to avoid multiple connections
        QObject::disconnect(marker, &QLegendMarker::clicked, this, &Plot::handleMarkerClicked);
        QObject::connect(marker, &QLegendMarker::clicked, this, &Plot::handleMarkerClicked);
    }
}

void
Plot::handleMarkerClicked()
{
    QLegendMarker* marker = qobject_cast<QLegendMarker*>(sender());

    ERIS_CHECK(qobject_cast<QLegendMarker*>(sender()));

    switch (marker->type())
    {
        case QLegendMarker::LegendMarkerTypeXY:
        {
            // Toggle visibility of series
            marker->series()->setVisible(!marker->series()->isVisible());

            // Turn legend marker back to visible, since hiding series also hides the marker
            // and we don't want it to happen now.
            marker->setVisible(true);

            // Dim the marker, if series is not visible
            qreal alpha = 1.0;

            if (!marker->series()->isVisible())
            {
                alpha = 0.5;
            }

            QColor color;
            QBrush brush = marker->labelBrush();
            color = brush.color();
            color.setAlphaF(alpha);
            brush.setColor(color);
            marker->setLabelBrush(brush);

            brush = marker->brush();
            color = brush.color();
            color.setAlphaF(alpha);
            brush.setColor(color);
            marker->setBrush(brush);

            QPen pen = marker->pen();
            color = pen.color();
            color.setAlphaF(alpha);
            pen.setColor(color);
            marker->setPen(pen);
            
            break;
        }
        default:
        {
            PRINT_PANIC("Unknown marker type");
            break;
        }
    }
}
void
Plot::disconnectMarkers()
{
    const auto markers = mChart->legend()->markers();
    for (QLegendMarker* marker : markers)
    {
        QObject::disconnect(marker, &QLegendMarker::clicked, this, &Plot::handleMarkerClicked);
    }
}
}  // namespace widgets