/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
 *      - 2020-2023 Rhea Rinaldo (Rhea@Odlanir.de)
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

#ifndef ERIS_GRAPH_INTERNAL_TRANSCRIBER_H
#define ERIS_GRAPH_INTERNAL_TRANSCRIBER_H

#include <fstream>
#include <list>
#include <string>
#include <vector>

namespace graphInternal
{
class Node;
class Edge;

class Transcriber
{
public:

    /**
     * Creates an transcriber object that can be used to start the analysis with the given inputs.
     * @param envNodes set of environment nodes
     * @param otherNodes set of other nodes excluding the environment nodes
     * @param mode the mode of operation
     * @param redundancy definition
     * @param outFileName name of the output file
     */
    Transcriber(const std::vector<Node*>& envNodes,
                const std::vector<Node*>& otherNodes,
                std::string redundancy,
                std::string outFileName);
    ~Transcriber();

    /**
     * Starts the transformation to the given model.
     */
    void
    buildModel();

    /**
     * Sets the name of the outpit (.pm) file.
     * @param name of file
     */
    void
    setOutfileName(std::string name);

private:
    /**
     * Creates a file for the PRISM Code and writes the generated variables, transitions etc in it.
     */
    void
    generateFile();

    /**
     * Parses the dependency graph and generates the automaton. Thereby the
     * global Mode is checked to determine whether a CTMC or an MDP has to
     * be build.
     */
    void
    buildAutomaton();

    /**
     * Assembles the safety transition from the ok state to the defective state
     * (n=0 -> n=1).
     *
     * @param init contains the initial guard setup
     * @param node is a pointer to the current node
     */
    void
    assembleSafetyTransition(const std::string& start, const std::string& end, Node* node);

    /**
     * Assembles the security transition and collects the security guarantees
     * if present. In the end, one or multiple transitions, depending on the
     * provided security guarantees, from the ok state to the corrupted state
     * of the node (n=0 -> n=2) are written.
     *
     * @param init contains the initial guard setup
     * @param node is a pointer to the current node
     */
    void
    assembleSecurityTransition(const std::string& init, Node* node);

    /**
     * Assembles the recovery transition from a corruptive state of the current node and adds the
     * transition and required constants to the global lists.
     */
    void
    assembleCorRecoveryTransition(Node* node);

    /**
     * Assembles the recovery transition from a defective state of the current node and adds the
     * transition and required constants to the global lists.
     */
    void
    assembleDefRecoveryTransition(Node* node, const std::string& init, const std::string& end);

    /**
     * Generates all permutations (0,1) by the given number of nodes (nodeCount)
     * and stores them in provided list as a string.
     * This is needed to generate transitions including every health
     * -constellation of the securing nodes of the target node.
     * @param nodeCount number of securing nodes, hence permutation base
     * @param permutations reference to the string list containing the generated
     * permutations
     */
    void
    generatePermutations(unsigned int nodeCount, std::list<std::string>* permutations);

    /**
     * Collects the security guarantees for the node, if securing nodes are
     * present.
     */
    void
    collectSecurityGuarantees(Node* node, std::string* guard, std::string* consequence);

    /**
     * Clears the containers. This is needed after each transformation in order to ensure a
     * that no data from previous runs is used.
     */
    void
    clear();

    std::string mVarDecl;

    std::string mVarUsg;

    std::string mModelAsString;

    std::string mRedundancy;

    std::string mOutfileName;

    std::vector<std::string> mConstants;
    std::vector<std::string> mVariables;
    std::vector<std::string> mTransitions;
    std::vector<std::string> mLabels;

    /** List of environment nodes */
    const std::vector<Node*>& mEnvNodes;

    /** List of all nodes excluding environment nodes */
    const std::vector<Node*>& mNodes;
};

}  // namespace graph

#endif /* ERIS_GRAPH_INTERNAL_TRANSCRIBER_H */
