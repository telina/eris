/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
 *      - 2020-2021 Rhea Rinaldo (Rhea@Odlanir.de)
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

#include "recovery_strategy.h"

namespace graph
{
QString
Recovery::toString(Strategy strategy)
{
    switch (strategy)
    {
        case Strategy::general: 
            return "General";
        case Strategy::restricted: 
            return "Restricted";
        case Strategy::custom: 
            return "Custom";
        default: 
            return "";
    }
}
Recovery::Strategy
Recovery::fromString(const std::string& in)
{// Switch with string does not really work (integer values required)
    if (in == "General")
    {
        return Strategy::general;
    }
    else if (in == "Restricted")
    {
        return Strategy::restricted;
    }
    else if (in == "Custom")
    {
        return Strategy::custom;
    }
    else
    {  // Error case
        return Strategy::undefined;
    }
}
Recovery::Strategy
Recovery::fromString(const QString& in)
{
    return fromString(in.toStdString());
}

}  // namespace graph
