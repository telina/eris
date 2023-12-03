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

#ifndef ERIS_GRAPH_INTERNAL_COMPONENT_TYPE_H
#define ERIS_GRAPH_INTERNAL_COMPONENT_TYPE_H

#include <QString>

#include "eris_config.h"

namespace graph
{

    enum ComponentType : int
    {
        normalNode = 1,
        criticalNode = 2,
        environmentNode = 3,
        reachEdge = 4,
        functionalEdge = 5,
        securityEdge = 6,
    };

    static inline QString
    componentTypeAsString(ComponentType type)
    {
        switch(type)
        {
            case ComponentType::functionalEdge:
                return "Functional Edge";
            case ComponentType::reachEdge:
                return "Reach Edge";
            case ComponentType::securityEdge:
                return "Security Edge";
            case ComponentType::normalNode:
                return "Normal Node";
            case ComponentType::criticalNode:
                return "Critical Node";
            case ComponentType::environmentNode:
                return "Environment Node";
            default:
                return "";
        }
    }

}  // namespace graph

#endif /* ERIS_GRAPH_INTERNAL_COMPONENT_TYPE_H */
