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

#ifndef ERIS_GRAPH_INTERNAL_NODE_H
#define ERIS_GRAPH_INTERNAL_NODE_H

#include "component_type.h"
#include "recovery_strategy.h"

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QRectF>
#include <regex>
#include <set>

namespace graphInternal
{
class Edge;

/*
 * This class represents a a node analysis object.
 */
class Node
{
public:
    /**
     * Creates a new node object providing the node counter.
     * @param type, type of node (component)
     * @param counter node counter
     * @param parent QT parent
     */
    Node(graph::ComponentType type,
         unsigned int id,
         bool recoverableDefect = false,
         bool recoverableCorruption = false,
         const std::string& intrusionIndicator = "",
         const std::string& failureIndicator = "",
         const std::string& securityIndicator = "",
         const std::string& defectRecoveryIndicator = "",
         const std::string& corruptionRecoveryIndicator = "",
         const std::string& essentialNodes = "",
         const std::string& customCorrRecoveryStrategy = "",
         graph::Recovery::Strategy corrStrategy = graph::Recovery::Strategy::restricted,
         const std::string& customDefRecoveryStrategy = "",
         graph::Recovery::Strategy defStrategy = graph::Recovery::Strategy::general);

    ~Node();

    /**
     * Get the string representation of the node.
     * @return string of the node
     */
    std::string
    getStringRepresentation();

    /**
     * Adds an Edge to the Node, i.e. inserts it in the list.
     * @param edge, the edge that shall be added
     */
    void
    addEdge(Edge* edge);

    /**
     * States whether the given node is a n environment node or not.
     * @return true if env node, false otherwise
     */
    bool
    isEnvironment();

    /**
     * States whether the node is critical or not.
     * @return true, if node is critical, false otherwise
     */
    bool
    isCritical();

    /**
     * States whether the node is critical or not.
     * @return true, if node is critical, false otherwise
     */
    bool
    isRecoverableFromDefect();

    /**
     * States whether the node is critical or not.
     * @return true, if node is critical, false otherwise
     */
    bool
    isRecoverableFromCorruption();

    /**
     * States whether the node is recoverable or not.
     * @return true, if node is critical, false otherwise
     */
    bool
    isRecoverable();

    /**
     * Checks whether the node is providing security guarantees to another node.
     * @return true if guarantees provided, false otherwise
     */
    bool
    providesSecurityGuarantees();

    /**
     * States whether the node is reachable (directly) from the environment node.
     * @return true if connection to env exists, false otherwise
     */
    bool
    isReachableFromEnv();

    /**
     * Iterates over all reach edges and identifies whether the node has a continues connection
     * starting in an environment node. Therefore the function is called recursively
     * on the nodes that are targetting this node by reach edges. In order to avoid endless loops,
     * the caller has to provide the ID of the original node, which will be carried further and the
     * loop will be broken if the ID is met again and false is returned.
     * @return true if reachable, false otherwise
     */
    bool
    isReachable();

    /**
     * Returns the nodes that this node targets via reach edges.
     * @return
     */
    std::list<Node*>&
    getReachableNodes();

    /**
     * Returns the nodes that point to this node via reach edges.
     * @return
     */
    std::list<Node*>&
    getReachingNodes();

    /**
     * Returns the nodes that this nodes targets via reach edges.
     * @return
     */
    std::vector<Node*>&
    getSecuringNodes();

    /**
     * Returns the number (id) of the node.
     * @return current id
     */
    unsigned int
    getNumber();

    /**
     * Returns the essential nodes of the current node as a string for display.
     * @return
     */
    std::string
    getEssentialNodes();

    /**
     * Indicates whether the node has essential nodes defined (string not empty) or not.
     * @return true if the node has essential nodes, false otherwise
     */
    bool
    hasEssentialNodes();

    /**
     * Checks whether the node can reach other nodes.
     * @return true if the node can reach other nodes, false otherwise
     */
    bool
    hasReachableNodes();

    /**
     * Checks whether the node is receiving security guarantees from other nodes,
     * hence has securing nodes.
     * @return true if the node receives guarantees, false otherwise
     */
    bool
    hasSecuringNodes();

    /**
     * Returns the intrusion probability of the node.
     * @return probability
     */
    std::string
    getIntrusionIndicator();

    /**
     * Checks whether the indicator was set, i.e., is > 0.
     * @return true if > 0, false otherwise
     */
    bool
    hasValidIntrusionIndicator();

    /**
     * Returns the failure probability of the node
     * @return probability
     */
    std::string
    getFailureIndicator();

    /**
     * Checks whether the indicator was set, i.e., is > 0.
     * @return true if > 0, false otherwise
     */
    bool
    hasValidFailureIndicator();

    /**
     * Returns the security probability of the node
     * @return probability
     */
    std::string
    getSecurityIndicator();

    /**
     * Checks whether the indicator was set, i.e., is > 0.
     * @return true if > 0, false otherwise
     */
    bool
    hasValidSecurityIndicator();

    /**
     * Returns the rate/probability of recovering from a defect of the node
     * @return probability
     */
    std::string
    getDefectRecoveryIndicator();

    /**
     * Checks whether the indicator was set, i.e., is > 0.
     * @return true if > 0, false otherwise
     */
    bool
    hasValidDefectRecoveryIndicator();

    /**
     * Returns the rate/probability of recovering from a corruption of the node
     * @return probability
     */
    std::string
    getCorruptionRecoveryIndicator();

    /**
     * Checks whether the indicator was set, i.e., is > 0.
     * @return true if > 0, false otherwise
     */
    bool
    hasValidCorruptionRecoveryIndicator();
    

    /**
     * Returns the recovery strategy that was chosen for this node in case it gets corrupted.
     * @return
     */
    graph::Recovery::Strategy
    getCorruptionRecoveryStrategy();

    /**
     * Returns the recovery strategy that was chosen for this node in case it gets defect.
     * @return
     */
    graph::Recovery::Strategy
    getDefectRecoveryStrategy();

    /**
     * Adds the node identified by the given ID as a redundant node to this node.
     * @param nodeId
     */
    void
    addRedundantNode(Node* redundantNode);
    
    void
    addRedundantAndEssentialNodes(std::string nodeId);

    /**
     * Indicates whether this node has redundant nodes or not.
     * @return true if the list of redundant node is not empty, false otherwise
     */
    bool
    hasRedundantNodes();

    /**
     * Returns the vector containing unique redundant node elements.
     * @return 
     */
    std::vector<Node*>
    getRedundantNodes();
    
    /**
     * Returns the redundant nodes as a logic string representation for PRISM,
     * including essential nodes.
     * @return redundant node string
     */
    std::string
    getRedundantNodesAsString(std::string nodeStatus="0");
    
    /**
     * Returns the redundant nodes as a logic string representation for PRISM.
     * @return redundant node string
     */
    std::string
    getRedundantAndEssentialNodesAsString(std::string nodeStatus="0");
    
    /**
     * Returns redundant nodes of this node in corrupted mode (=2) and without 
     * essential nodes.
     * @return 
     */
    std::string
    getReundantCorrNodes();

    /**
     * Iterates over the node's essential node definition (user input) and identifies single
     * node elems, in order to recursively call this function on them. The goal is to receive
     * a hierarchically built string that contains the firstly called nodes essential nodes,
     * extended by their essential nodes, extended by their essential nodes and so on.
     * @return, the extended essential node string
     */
    std::string
    collectEssentialNodes(unsigned int startNode);

    /**
     * Parses the reach connection from the given node to the environment node and identifies
     * whether a non-corrupted path to it exists.
     * @return
     */
    std::string
    getRestrictedRecoveryFormula();

    /**
     * Returns the corruption recovery strategy that was retrieved by user input.
     * @return
     */
    std::string
    getCustomCorrRecoveryFormula();

    /**
     * Returns the defect recovery strategy that was retrieved by user input.
     * @return
     */
    std::string
    getCustomDefRecoveryFormula();

private:
    /**
     * Helper function that iterates over the nodes functional dependencies and finds the node
     * associated to the given number.
     * @param number, searched node number
     * @param searchedNode, reference to store the searched node in
     * @return true if node was found, false otherwise
     */
    bool
    findFunctionalNode(unsigned int number, Node*& searchedNode);

    /**
     * Helper function for recursive call to identify whether a path to the environment node exists,
     * that is
     * @return
     */
    void
    findPathToEnv(unsigned int origin,
                  std::set<std::set<unsigned int>>* pathes,
                  std::set<unsigned int>* path);

    bool
    findReachable(std::list<unsigned int> visited);

    /** List of nodes that are reaching to this node */
    std::list<Node*> mReachingNodes;

    /** List of nodes that are reachable from this node */
    std::list<Node*> mReachableNodes;

    /** List of nodes that target this node via security edges */
    std::vector<Node*> mSecuringNodes;

    /** List of nodes that target this node via functional edges */
    std::list<Node*> mFunctionalNodes;
    
    std::vector<Node*> mRedundantNodes;

    std::string mRedundantAndEssentialNodes;

    
    /** Flag indicating the criticality of the node */
    bool mCritical;

    bool mEnvironment;

    /** Flag indicating that the node can recover from defects */
    bool mRecoverableFromDefect;

    /** Flag indicating that the node can recover from corruptions */
    bool mRecoverableFromCorruption;

    bool mProvidesGuarantees;

    /** Flag indicating that this node provides security guarantees to another node */
    std::string mStatusChange;

    /** Number/ID of the node */
    unsigned int mNumber;

    /** Flag indicating whether this node is reachable from the environment node*/
    bool mReachableFromEnv;

    /** The probability that the node runs into a security failure*/
    std::string mIntrusionIndicator;

    /** The probability that the node runs into a safety failure*/
    std::string mFailureIndicator;

    /** The security guarantees offered by this node*/
    std::string mSecurityIndicator;

    /** The recovery indicator in case of a defect of this node*/
    std::string mDefectRecoveryIndicator;

    /** The recovery indicator in case of a corruption of this node*/
    std::string mCorruptionRecoveryIndicator;

    /** String containing the node dependencies of this node */
    std::string mEssentialNodes;

    /** String containing the corruption custom recovery strategy */
    std::string mCustomCorrRecoveryFormula;

    /** String containing the corruption custom recovery strategy */
    std::string mCustomDefRecoveryFormula;

    /** Flag indicating which recovery strategy in case of corruption is applied*/
    graph::Recovery::Strategy mCorruptionRecoveryStrategy;

    /** Flag indicating which recovery strategy in case of defect is applied*/
    graph::Recovery::Strategy mDefectRecoveryStrategy;
};

}  // namespace graph

#endif /* ERIS_GRAPH_INTERNAL_NODE_H */
