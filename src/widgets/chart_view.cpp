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

#include "chart_view.h"

#include "logger.h"
#include "checks.h"

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QLineSeries>
#include <QtCore/QDebug>
#include <QtCore/QtMath>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QMenu>
#include <QPrinter>
#include <QPrintDialog>
#include <QThreadPool>
#include <QFileDialog>
#include <sstream>

namespace widgets
{
ChartView::ChartView(QWidget* parent) : QWidget(parent), mPlot()
{
    // Create chart view with the chart
    mPlot.setTitle("Evaluation Result");

    mChartView = new QChartView(mPlot(), this);
    mChartView->setRenderHint(QPainter::Antialiasing, true);

    // Create layout for grid and detached legend
    mMainLayout = new QGridLayout();
    mMainLayout->addWidget(mChartView, 0, 1, 3, 1);
    setLayout(mMainLayout);

    // createDummySinWaves();

    mChartView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mChartView,
            SIGNAL(customContextMenuRequested(QPoint)),
            this,
            SLOT(customMenuRequested(QPoint)));

    connect(this,
            &ChartView::setPropertyResults,
            this,
            &ChartView::setPropertyResultsSlot,
            Qt::QueuedConnection);

    mContextMenu = new QMenu(this);
    auto act = new QAction("Export as", this);
    connect(act, &QAction::triggered, this, &ChartView::exportTriggered);
    mContextMenu->addAction(act);
    act = new QAction("Export as as CSV", this);
    connect(act, &QAction::triggered, this, &ChartView::exportAsText);
    mContextMenu->addAction(act);
}

void
ChartView::propertyResults(const QMap<QString, QList<QPointF>> results)
{
    PRINT_INFO("Property Results are ready");
    emit setPropertyResults(results);
}

void
ChartView::failedToParse(eval::PrismResultsParser::ParseError error)
{
    PRINT_ERROR("failedToParse error : %s ",
                eval::PrismResultsParser::TranslateParseError(error));
}
void
ChartView::clear()
{
    PRINT_INFO("Clearing the plot");
    mPlot.clear();
}
void
ChartView::customMenuRequested(QPoint pos)
{
    mContextMenu->popup(mChartView->viewport()->mapToGlobal(pos));
}
void
ChartView::exportTriggered(bool /*checked*/)
{
    PRINT_INFO("exportAs triggered");
    std::unique_ptr<QPrinter> printer(new QPrinter());
    printer->setPageOrientation(QPageLayout::Landscape);
    std::unique_ptr<QPrintDialog> dialog(new QPrintDialog(printer.get()));
    dialog->setWindowTitle("Print");

    if (dialog->exec() != QDialog::Accepted)
        return;

    std::unique_ptr<QPainter> painter(new QPainter());
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    painter->begin(printer.get());
    mChartView->render(painter.get());
    painter->end();
}
void
ChartView::operationStarted()
{
    QThreadPool::globalInstance()->start([&] { clear(); });
}
void
ChartView::setPropertyResultsSlot(const QMap<QString, QList<QPointF>> results)
{
    mPlot.addLineSeries(results);
}
void
ChartView::exportAsText(bool)
{
    if (mPlot.series().empty())
    {
        PRINT_INFO("Plot series are empty");
        return;
    }
    QString tmp = QFileDialog::getSaveFileName(
            this, tr("Save evaluation results"), "", tr("All files (*)"));
    if (tmp.isNull())
    {
        return;
    }
    QFile file(tmp);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        PRINT_ERROR("Failed to open the file %s ", tmp.toStdString().c_str());
        return;
    }

    std::stringstream ss;

    auto iter = mPlot.series().begin();
    ss << "# Label,X,Y" << std::endl;

    while (iter != mPlot.series().end())
    {
        ERIS_CHECK(*iter);
        if (!*iter)
            return;
        auto points = (*iter)->points();
        auto point_iter = points.begin();

        while (point_iter != points.end())
        {
            ss << (*iter)->name().toStdString() << "," << point_iter->x() << "," << point_iter->y()
               << std::endl;
            point_iter++;
        }

        iter++;
    }
    ERIS_CHECK(file.write(ss.str().c_str()) == ss.str().size());
    file.close();
}

}  // namespace widgets