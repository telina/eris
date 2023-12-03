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

#ifndef ERIS_RECOVERY_STRATEGY_H
#define ERIS_RECOVERY_STRATEGY_H

#include <string>
#include <QString>

#include "eris_config.h"

namespace graph
{
class Recovery
{
public:
    enum class Strategy : int
    {
        general = 1,
        restricted = 2,
        custom = 3,
        undefined = 4,
    };

    ERIS_DEAD_CLASS(Recovery);

    /**
     * Returns the  string representation of the given recovery strategy.
     * @param strategy recovery strategy
     * @return QString representation
     */
    static QString
    toString(Strategy strategy);

    /**
     * Converts the std::string strategy into the equivalent Strategy type.
     * @param strategyRep
     * @return strategy
     */
    static Strategy
    fromString(const std::string& strategyRep);
    
    /**
     * Converts the QString strategy into the equivalent Strategy type.
     * @param strategyRep
     * @return strategy
     */
    static Strategy
    fromString(const QString& in);
};
}  // namespace graph

#endif /* ERIS_RECOVERY_STRATEGY_H*/
