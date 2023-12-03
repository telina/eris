/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
 *      - 2021-2021 Mohammed Hussam Al Turjman (Mohammed_Hussam.Al_Turjman@dfki.de)
 *      - 2023 Rhea Rinaldo (Rhea@Odlanir.de)
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

#ifndef ERIS_WIDGETS_PLOT_H
#define ERIS_WIDGETS_PLOT_H

#include "memory.h"

#include <QMap>

#include <QtCharts/QChart>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

namespace widgets
{
// This is an experimental 2D plot that is similar to matplotlib.pyplot
class Plot : public QObject
{
    Q_OBJECT
public:
    explicit Plot(QString xAxisName="Time", QString yAxisName="Probability", 
            QObject* parent = nullptr, const QString& title = "Plot");
    ~Plot() override;

    QChart*
    operator()()
    {
        return mChart.get();
    }

    void
    showLegend(bool show = true);

    void
    setLegendAlignment(Qt::AlignmentFlag alignment);

    void
    addLineSeries(const QMap<QString, QList<QPointF>> results);

    void
    clear();

    void
    setTitle(const QString& title);

    const QList<QLineSeries*>&
    series() const
    {
        return mSeries;
    }
private slots:
    void
    handleMarkerClicked();

private:
    void
    connectMarkers();

    void
    disconnectMarkers();
    
    void nameAxis();
    
    std::unique_ptr<QChart> mChart;
    
    QList<QLineSeries*> mSeries;
    
    QString mXAxisName;
    
    QString mYAxisName;
    
};

}  // namespace widgets

#endif  // ERIS_WIDGETS_PLOT_H