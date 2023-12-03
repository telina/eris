/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
 *
 * Authors:
 *      - 2020-2021 Rhea Rinaldo (Rhea@Rinaldo.biz)
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

#include "model.h"

namespace graph
{
void
Model::setType(Type type)
{
    mType = type;

    emit modelTypeUpdated();
}

Model::Type
Model::getType()
{
    return mType;
}

QString
Model::getTypeAsString(Type type)
{
    QString ret = "";
    switch (type)
    {
        case Type::CTMC: 
            ret = "CTMC";
            break;
        case Type::MDP: 
            ret = "MDP"; 
            break;
        default: 
            break;
    }
    return ret;
}
}  // namespace graph
