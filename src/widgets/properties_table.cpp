/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2020 Rhea Rinaldo (Rhea@Odlanir.de).
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

#include "properties_table.h"

#include <QHeaderView>

#define COLUMN_COUNT 1

#define MAKE_ITEM_EDITABLE(item, editable)                      \
    do                                                          \
    {                                                           \
        if (!editable)                                          \
            item->setFlags(item->flags() ^ Qt::ItemIsEditable); \
    } while (0)

#define MAKE_TABLE_INTERACTIVE()                                            \
    do                                                                      \
    {                                                                       \
        setEditTriggers(QAbstractItemView::DoubleClicked);                  \
        setSelectionBehavior(QAbstractItemView::SelectRows);                \
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);      \
        horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive); \
    } while (0)

#define SET_TIP(column, tip)                                 \
    do                                                       \
    {                                                        \
        horizontalHeaderItem(column)->setToolTip(tr(tip));   \
        horizontalHeaderItem(column)->setStatusTip(tr(tip)); \
    } while (0)

#define SET_COLOR(item)                                  \
    do                                                   \
    {                                                    \
        item->setBackgroundColor(QColor(227, 227, 227)); \
    } while (0)

namespace widgets
{
PropertiesTable::PropertiesTable(QWidget* parent) : QTableWidget(parent)
{
    MAKE_TABLE_INTERACTIVE();

    setColumnCount(COLUMN_COUNT);
    setRowCount(0);

    setHorizontalHeaderLabels({"Property"});

    SET_TIP(0, "Property");

    resizeColumnsToContents();

    loadProperties({"hello", "world"});
}
void
PropertiesTable::resetEveryThing()
{
    clearContents();
    setRowCount(0);
    setColumnCount(COLUMN_COUNT);
}
void
PropertiesTable::loadProperties(const QList<QString>& properties)
{
    clearContents();
    setRowCount(properties.length());
    setColumnCount(COLUMN_COUNT);

    int idx = 0;

    for (auto& property : properties)
    {
        auto tableItem = new QTableWidgetItem(property);
        MAKE_ITEM_EDITABLE(tableItem, false);
        // SET_COLOR
        setItem(idx++, 0, tableItem);
    }

    resizeColumnsToContents();
}
}  // namespace widgets