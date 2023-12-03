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

#ifndef ERIS_WIDGETS_OUTPUT_WIDGET_H
#define ERIS_WIDGETS_OUTPUT_WIDGET_H

#include <QPlainTextEdit>

namespace widgets
{
// A simple text area that is read only
// and can put a colored line at the end of each input.
//
// Possible usage scenario:
// just call writeOutput("bla",Qt::green);
//
class OutputWidget : public QPlainTextEdit
{
    Q_OBJECT

public:
    OutputWidget();

public slots:
    void
    writeOutput(const QString& output, Qt::GlobalColor color);
};
}  // namespace widgets

#endif  // ERIS_WIDGETS_OUTPUT_WIDGET_H
