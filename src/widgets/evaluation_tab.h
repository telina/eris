/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
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

#ifndef ERIS_WIDGETS_EVALUATION_TAB_H
#define ERIS_WIDGETS_EVALUATION_TAB_H

#include <QWidget>
#include "eris_config.h"

QT_BEGIN_NAMESPACE
class QHBoxLayout;
QT_END_NAMESPACE

namespace widgets
{
    
class ChartView;
class PropertiesTable;
// This is the tab called evaluation
// TODO view all proprietress in a table instead of putting them
// in the legend of the plot.
class EvaluationTab : public QWidget
{
    Q_OBJECT
public:
    static EvaluationTab*
    Get();
    ERIS_DISALLOW_COPY_AND_ASSIGN(EvaluationTab);
    EvaluationTab() = delete;
    ChartView*
    view()
    {
        return chart_view_;
    };

private:
    explicit EvaluationTab(QWidget* parent);

private:
    PropertiesTable* properties_table_;
    QHBoxLayout* q_hbox_layout_;
    ChartView* chart_view_;
};

}  // namespace widgets

#endif  // ERIS_WIDGETS_EVALUATION_TAB_H
