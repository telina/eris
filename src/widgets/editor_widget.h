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

#ifndef ERIS_WIDGETS_EDITOR_WIDGET_H
#define ERIS_WIDGETS_EDITOR_WIDGET_H

#include <QObject>
#include <QPlainTextEdit>

#include "syntax_highlighting.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

namespace widgets
{
class HighlighterWidget;
class LineNumberArea;
//
// Evaluation settings editor (properties editor).
// From this widget a user can edit the later used by prism "experiment.pctl" file.
//
class EditorWidget : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit EditorWidget(QWidget* parent = nullptr);

    // These functions are to view line numbers alongside
    // the content of the editor.
    void
    lineNumberAreaPaintEvent(QPaintEvent* event);
    int
    lineNumberAreaWidth();

    // This function is to mark a predefined property.
    // To add or remove properties you should go to the
    // highlighterWidget.
    // Notice that specifying random properties has no effect.
    void
    markAsUnchecked(const QString&, bool mark = true);

public slots:
    // Can be used to clean the editor widget.
    void
    clean();

protected:
    void
    resizeEvent(QResizeEvent* event) override;

private slots:
    void
    updateLineNumberAreaWidth(int newBlockCount);
    void
    highlightCurrentLine();
    void
    updateLineNumberArea(const QRect&, int);

private:
    QWidget* lineNumberArea;
    int currentLine = 0;
    HighlighterWidget* highlighterWidget;
};

// Very basic numbering area for the editor
class LineNumberArea : public QWidget
{
    Q_OBJECT

public:
    explicit LineNumberArea(EditorWidget* editor) : QWidget(editor)
    {
        codeEditor = editor;
    }

    QSize
    sizeHint() const override
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void
    paintEvent(QPaintEvent* event) override
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    EditorWidget* codeEditor;
};

}  // namespace widgets

#endif  // ERIS_WIDGETS_EDITOR_WIDGET_H