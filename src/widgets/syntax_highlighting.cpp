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

#include "syntax_highlighting.h"

#include "task.h"

#include <QTextStream>

#define COMMENT 1

namespace widgets
{
HighlighterWidget::HighlighterWidget(QTextDocument* parent) : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    static const QString keywordPatterns[] = {
            QStringLiteral("\\bchar\\b"),      QStringLiteral("\\bclass\\b"),
            QStringLiteral("\\bconst\\b"),     QStringLiteral("\\bdouble\\b"),
            QStringLiteral("\\benum\\b"),      QStringLiteral("\\bexplicit\\b"),
            QStringLiteral("\\bfriend\\b"),    QStringLiteral("\\binline\\b"),
            QStringLiteral("\\bint\\b"),       QStringLiteral("\\blong\\b"),
            QStringLiteral("\\bnamespace\\b"), QStringLiteral("\\boperator\\b"),
            QStringLiteral("\\bprivate\\b"),   QStringLiteral("\\bprotected\\b"),
            QStringLiteral("\\bpublic\\b"),    QStringLiteral("\\bshort\\b"),
            QStringLiteral("\\bsignals\\b"),   QStringLiteral("\\bsigned\\b"),
            QStringLiteral("\\bslots\\b"),     QStringLiteral("\\bstatic\\b"),
            QStringLiteral("\\bstruct\\b"),    QStringLiteral("\\btemplate\\b"),
            QStringLiteral("\\btypedef\\b"),   QStringLiteral("\\btypename\\b"),
            QStringLiteral("\\bunion\\b"),     QStringLiteral("\\bunsigned\\b"),
            QStringLiteral("\\bvirtual\\b"),   QStringLiteral("\\bvoid\\b"),
            QStringLiteral("\\bvolatile\\b"),  QStringLiteral("\\bbool\\b")};

    keywordFormat.setForeground(Qt::darkCyan);
    keywordFormat.setFontWeight(QFont::Bold);

    uncheckedFormat.setForeground(Qt::gray);
    quotationFormat.setForeground(Qt::darkGreen);

    for (const QString& pattern : keywordPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);
}

void
HighlighterWidget::markAsUnchecked(const QString& keyword, bool mark)
{
    int idx = 0;
    bool found = false;
    QString reg = ".*" + keyword + ".*";
    auto iter = highlightingRules.begin();
    while (iter != highlightingRules.end())
    {
        if (iter->pattern.pattern() == reg)
        {
            found = true;
            break;
        }
        iter++;
        idx++;
    }

    if (found && mark)
        return;

    HighlightingRule rule;
    rule.pattern = QRegularExpression(reg);
    rule.format = uncheckedFormat;
    if (mark)
    {
        highlightingRules.append(rule);
    }
    else
    {
        if (found)
        {
            highlightingRules.remove(idx);
        }
    }
}

void
HighlighterWidget::highlightBlock(const QString& text)
{
    bool foundMatch = false;
    foreach (const HighlightingRule& rule, highlightingRules)
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            foundMatch = true;
        }
        if (rule.type == COMMENT && foundMatch)
            break;
    }
}

}  // namespace widgets