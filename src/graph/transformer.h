/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
 *      - 2020-2023  Rhea Rinaldo (Rhea@Odlanir.de)
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


#ifndef ERIS_GRAPH_TRANSFORMER_H
#define ERIS_GRAPH_TRANSFORMER_H

#include "eris_config.h"

#include <QObject>

#include <memory>

QT_BEGIN_NAMESPACE
class QProcess;
QT_END_NAMESPACE

namespace graph
{
class NodeItem;
class GraphicScene;
}

namespace graphInternal
{
class Node;
class Edge;

class ERIS_EXPORT Transformer : public QObject
{
    Q_OBJECT

public:
    explicit Transformer(graph::GraphicScene* graphicScene);
    ~Transformer() override;

    // TODO Move to another thread
    bool
    start(const std::string& redundancyDefinition, const std::string& outFileName);

    /**
     * Attempts a graceful shutdown of the transformation process by killing the Simulation Process
     * (if active) and stopping the transformation.
     */
    void
    stop();
    
    /**
     * Listens to the finish signal of the transformer thread. The GUI elements will be unblocked.
     * If the transformation was successful, the program is ready to call PRISM afterwards.
     * If an error occurred, the collection of errors will be displayed.
     */
    void
    DeprecatedTransformationFinished(bool success);

signals:

    void
    transformationFinished(bool success);

private:
    /**
     * Starts the transformation by transforming NodeItems and EdgeItems to Nodes and Edges.
     * Then the analyser is called to perform the analysis, create the string representation and
     * write it to the given output file.
     * If NodeItems turn out to be simulated, the transformation is paused while the
     * simulation is run to receive the simulation results.
     */
    bool
    transform(std::string& outfile);

    /**
     * Parses all scene items and converts them to analysis nodes and edges. Sets redundancy
     * definition and essential nodes etc.
     * Fills the given vectors with the nodes and edge and sets the error handler if necessary.
     * @param envNodes vector to push the env nodes to
     * @param otherNodes vector to push other nodes to
     * @return true if generation was successful, false otherwise
     */
    bool
    generateLogicRepresentation(std::vector<Node*>& envNodes, std::vector<Node*>& otherNodes);

    /**
     * Sets the redundant nodes of the parsed nodes given by the globally set 
     * redundancy definition. Thereby a pointer to the twin node object is added
     * in the viewed node.
     * @param nodes
     */
    void
    processRedundancy(const std::vector<Node*>& nodes);

    /**
     * Iterates overall nodes and searches for the node with the provided ID. If found, the node is
     * stored in the provided reference and true is returned.
     * @param nodes list of nodes
     * @param id of searched node
     * @param node reference to store the found node in
     * @return true if found, false otherwise
     */
    bool
    getNodeById(const std::vector<Node*>& nodes, unsigned int id, Node*& node);

    /**
     * Starts the simulation of the subsystem using octave. The returned results
     * are retrieved from the command line and set in the particular subsystem
     * node.
     * @param nodeItem
     * @return 
     */
    bool
    startSimulationProcess(graph::NodeItem* nodeItem);
    
    /** Pointer to the graphic scene */
    graph::GraphicScene* mScene = nullptr;

    /** Global redundancy definition */
    std::string mRedundancy;

    std::string mOutFileName;

    std::unique_ptr<QProcess> mProcess;

    bool mRunning = false;
};

}  // namespace graph

#endif /* ERIS_GRAPH_TRANSFORMER_H */
