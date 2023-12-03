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

#ifndef ERIS_WIDGETS_CHART_VIEW_H
#define ERIS_WIDGETS_CHART_VIEW_H

#include "plot.h"
#include "prism_results_parser.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class QGridLayout;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
QT_CHARTS_END_NAMESPACE

namespace widgets
{
/** 
 * This widget controls and views the evaluation plot.
 * It does that by observing the signals from parser::PrismResultsParser
 * and updating the plot accordingly.
 */
class ChartView : public QWidget, public eval::PrismResultsParser::Observer
{
    Q_OBJECT
public:
    explicit ChartView(QWidget* parent = nullptr);
    void
    clear();

protected:
    
    void
    propertyResults(const QMap<QString, QList<QPointF>> results) override;
    
    void
    failedToParse(eval::PrismResultsParser::ParseError error) override;
    
    void
    operationStarted() override;

private:
signals:
    
    void
    setPropertyResults(const QMap<QString, QList<QPointF>> results);

private slots:
    void
    customMenuRequested(QPoint pos);
    
    void
    exportTriggered(bool checked = false);
    
    void
    exportAsText(bool checked = false);
    
    void
    setPropertyResultsSlot(const QMap<QString, QList<QPointF>> results);

private:
    Plot mPlot;
    QMenu* mContextMenu;
    QChartView* mChartView;
    QGridLayout* mMainLayout;
};

}  // namespace widgets

#endif  // ERIS_WIDGETS_CHART_VIEW_H