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

#ifndef ERIS_GRAPH_INTERNAL_EDGE_H
#define ERIS_GRAPH_INTERNAL_EDGE_H

#include "component_type.h"

namespace graphInternal
{
class Node;
/**
 * The internal representation of an edge between two nodes. It indicates that  messages are
 * exchanged between these nodes, and certain data dependencies exist. This object is solely used
 * for the analysis (transformation) to a probabilistic automaton.
 */
class Edge
{
public:
    /**
     * Creates a new Edge object providing a start and end node and the type of the edge.
     * @param start node
     * @param end node
     * @param type of edge
     */
    Edge(Node* start, Node* end, graph::ComponentType type);
    ~Edge();

    /**
     * Returns the node the edge starts in.
     * @return
     */
    Node*
    startItem() const
    {
        return mStartNode;
    }

    /**
     * Returns the node the edge ends in.
     * @return
     */
    Node*
    endItem() const
    {
        return mEndNode;
    }

    graph::ComponentType
    getComponentType();

private:
    /** Start and End node of the Edge*/
    Node* mStartNode;
    Node* mEndNode;

    /** Type of Csomponent*/
    graph::ComponentType mType;
};

}  // namespace graph

#endif /* ERIS_GRAPH_INTERNAL_EDGE_H */
