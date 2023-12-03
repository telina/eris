/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
 *
 * Authors:
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
#include "evaluation_tab.h"

#include <QRandomGenerator>
#include <QSplitter>
#include <QHBoxLayout>
#include "chart_view.h"
#include "properties_table.h"

using namespace QtCharts;

namespace widgets
{
    
EvaluationTab::EvaluationTab(QWidget* parent) : QWidget(parent), properties_table_(nullptr)
{
    q_hbox_layout_ = new QHBoxLayout();
    // propertiesTable = new PropertiesTable();
    chart_view_ = new ChartView();
    auto* splitter = new QSplitter(Qt::Horizontal);

    // splitter->addWidget(propertiesTable);
    splitter->addWidget(chart_view_);
    splitter->setSizes({5, 1000});

    q_hbox_layout_->addWidget(splitter);

    setLayout(q_hbox_layout_);
    eval::PrismResultsParser::Get()->RegisterObserver(chart_view_);
}

EvaluationTab*
EvaluationTab::Get()
{
    // Intentionally leaked
    static EvaluationTab* instance = new EvaluationTab(nullptr);
    return instance;
}

}  // namespace widgets