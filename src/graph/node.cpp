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

#include "node.h"
#include "error_handler.h"
#include "errors.h"
#include "edge.h"
#include "logger.h"

#include <set>

using namespace graph;

namespace graphInternal
{
using widgets::ErrorHandler;
using widgets::Errors;

Node::Node(ComponentType type,
           unsigned int id,
           bool recoverableDefect,
           bool recoverableCorruption,
           const std::string& intrusionIndicator,
           const std::string& failureIndicator,
           const std::string& securityIndicator,
           const std::string& defectRecoveryIndicator,
           const std::string& corruptionRecoveryIndicator,
           const std::string& essentialNodes,
           const std::string& customCorrRecoveryFormula,
           Recovery::Strategy corrStrategy,
           const std::string& customDefRecoveryFormula,
           Recovery::Strategy defStrategy) :
    mCritical(false),
    mEnvironment(false),
    mRecoverableFromDefect(recoverableDefect),
    mRecoverableFromCorruption(recoverableCorruption),
    mProvidesGuarantees(false),
    mStatusChange(""),
    mNumber(id),
    mReachableFromEnv(false),
    mIntrusionIndicator(intrusionIndicator),
    mFailureIndicator(failureIndicator),
    mSecurityIndicator(securityIndicator),
    mDefectRecoveryIndicator(defectRecoveryIndicator),
    mCorruptionRecoveryIndicator(corruptionRecoveryIndicator),
    mEssentialNodes(essentialNodes),
    mCustomCorrRecoveryFormula(customCorrRecoveryFormula),
    mCustomDefRecoveryFormula(customDefRecoveryFormula),
    mCorruptionRecoveryStrategy(corrStrategy),
    mDefectRecoveryStrategy(defStrategy)
{
    switch (type)
    {
        case ComponentType::criticalNode:
            mCritical = true; 
            break;
        case ComponentType::environmentNode:
            mEnvironment = true; 
            break;
        default: 
            break;
    }
}

Node::~Node()
{
    
}

std::string
Node::getStringRepresentation()
{
    return "n" + std::to_string(mNumber);
}

void
Node::addEdge(Edge* edge)
{
    if (edge != NULL)
    {
        Node* origin = edge->startItem();
        Node* target = edge->endItem();
        if (edge->getComponentType() == ComponentType::reachEdge)
        {
            if (target == this)
            {
                if (origin->isEnvironment())
                {
                    mReachableFromEnv = true;
                    mReachingNodes.push_back(origin);
                }
                else
                {
                    mReachingNodes.push_back(origin);
                }
            }
            else
            {
                if (origin->isEnvironment())
                {
                    mReachableFromEnv = true;
                }
                mReachableNodes.push_back(target);
            }
        }
        else if (edge->getComponentType() == ComponentType::securityEdge)
        {
            if (target == this)
            {
                mSecuringNodes.push_back(origin);
            }
            else
            {  // This node is providing guarantees
                mProvidesGuarantees = true;
            }
        }
        else if (edge->getComponentType() == ComponentType::functionalEdge && target == this)
        {
            mFunctionalNodes.push_back(origin);
        }
    }
}

std::list<Node*>&
Node::getReachableNodes()
{
    return mReachableNodes;
}

std::list<Node*>&
Node::getReachingNodes()
{
    return mReachingNodes;
}

std::vector<Node*>&
Node::getSecuringNodes()
{
    return mSecuringNodes;
}

unsigned int
Node::getNumber()
{
    return mNumber;
}

bool
Node::isEnvironment()
{
    return mEnvironment;
}

bool
Node::isCritical()
{
    return mCritical;
}

bool
Node::isRecoverableFromDefect()
{
    return mRecoverableFromDefect;
}

bool
Node::isRecoverableFromCorruption()
{
    return mRecoverableFromCorruption;
}

bool
Node::providesSecurityGuarantees()
{
    return mProvidesGuarantees;
}

bool
Node::isReachableFromEnv()
{
    return mReachableFromEnv;
}

bool
Node::isReachable()
{
    std::list<unsigned int> visited;
    return findReachable(visited);
}

bool
Node::findReachable(std::list<unsigned int> visited)
{
    visited.push_back(mNumber);
    if (mReachableFromEnv)
    {
        return true;
    }
    else
    {
        for (Node* node : mReachingNodes)
        {
            std::list<unsigned int>::iterator iter =
                    std::find(visited.begin(), visited.end(), node->getNumber());
            if (iter != visited.end())
            {              // snake is eating its tail
                continue;  // break endless loop
            }
            else if (node->findReachable(visited))
            {
                return true;
            }
        }
        return false;
    }
}

std::string
Node::getEssentialNodes()
{
    return mEssentialNodes;
}

bool
Node::hasEssentialNodes()
{
    return (!mEssentialNodes.empty());
}

bool
Node::hasReachableNodes()
{
    return (!mReachableNodes.empty());
}

bool
Node::hasSecuringNodes()
{
    return (!mSecuringNodes.empty());
}

std::string
Node::getIntrusionIndicator()
{
    return mIntrusionIndicator;
}

bool
Node::hasValidIntrusionIndicator()
{
    return std::atof(mIntrusionIndicator.c_str()) > 0;
}

std::string
Node::getFailureIndicator()
{
    return mFailureIndicator;
}

bool
Node::hasValidFailureIndicator()
{
    return std::atof(mFailureIndicator.c_str()) > 0;
}

std::string
Node::getSecurityIndicator()
{
    return mSecurityIndicator;
}

bool
Node::hasValidSecurityIndicator()
{
    return std::atof(mSecurityIndicator.c_str()) > 0;
}

std::string
Node::getDefectRecoveryIndicator()
{
    return mDefectRecoveryIndicator;
}

bool
Node::hasValidDefectRecoveryIndicator()
{
    return std::atof(mDefectRecoveryIndicator.c_str()) > 0;
}

Recovery::Strategy
Node::getDefectRecoveryStrategy()
{
    return mDefectRecoveryStrategy;
}

std::string
Node::getCorruptionRecoveryIndicator()
{
    return mCorruptionRecoveryIndicator;
}

bool
Node::hasValidCorruptionRecoveryIndicator()
{
    return std::atof(mCorruptionRecoveryIndicator.c_str()) > 0;
}

Recovery::Strategy
Node::getCorruptionRecoveryStrategy()
{
    return mCorruptionRecoveryStrategy;
}

std::string
Node::getCustomCorrRecoveryFormula()
{
    return mCustomCorrRecoveryFormula;
}

std::string
Node::getCustomDefRecoveryFormula()
{
    return mCustomDefRecoveryFormula;
}


void
Node::addRedundantNode(Node* redundantNode)
{
    if (mRedundantNodes.empty() || std::find(mRedundantNodes.begin(), mRedundantNodes.end(), redundantNode) == mRedundantNodes.end())
    {
        mRedundantNodes.push_back(redundantNode);
    }
}

/*void
Node::addRedundantNode(std::string redundantNode)
{
    if (mRedundantAndEssentialNodes.empty())
    {
        mRedundantAndEssentialNodes += redundantNode;
    }
    else if (mRedundantAndEssentialNodes.find(redundantNode) == std::string::npos)
    {
        mRedundantAndEssentialNodes += " | " + redundantNode;
    }
    std::cout << "Redundant nodes of node " << this->mNumber << " : " << mRedundantAndEssentialNodes
              << std::endl;
}*/

bool
Node::hasRedundantNodes()
{
    return (!mRedundantNodes.empty());
}

std::vector<Node*>
Node::getRedundantNodes()
{
    return mRedundantNodes;
}

std::string
Node::getRedundantNodesAsString(std::string nodeStatus)
{
    std::string ret = "";
    for (Node* redundantNode : mRedundantNodes)
    {
        ret += redundantNode->getStringRepresentation() + "=" + nodeStatus + " & ";
    }
    ret = ret.substr(0, ret.size() - 3);
    return ret;
}

std::string
Node::getRedundantAndEssentialNodesAsString(std::string nodeStatus)
{
    std::string ret = "";
    for (Node* redundantNode : mRedundantNodes)
    {
        ret += redundantNode->getStringRepresentation() + "=" + nodeStatus;
        if (redundantNode->hasEssentialNodes())
        {
            ret += " & (" + redundantNode->collectEssentialNodes(redundantNode->getNumber()) + ")";
        }
        ret += " | ";
    }
    ret = ret.substr(0, ret.size() - 3);

    return ret;
}

bool
Node::findFunctionalNode(unsigned int number, Node*& searchedNode)
{
    for (Node* currentNode : mFunctionalNodes)  // must be a node that is directed to this node
    {
        if (currentNode->getNumber() == number)
        {
            searchedNode = currentNode;
            return true;
        }
    }
    return false;
}

std::string
Node::collectEssentialNodes(unsigned int origin)
{
    std::string totalEssentialNodes;
    totalEssentialNodes.reserve(1000);  // sollte wohl reichen
    totalEssentialNodes.insert(0, mEssentialNodes);

    std::regex nodeElem("(n[0-9][0-9]*)");
    std::smatch match;
    Node* node;
    unsigned int offset = 0;
    std::regex_search(mEssentialNodes, match, nodeElem);
    for (unsigned int i = 1; i < match.size(); ++i)  // skip first match (the whole string)
    {
        std::string elem = match[i];
        std::string number = elem.substr(1);                     // cut off the n
        unsigned int pos = match.position(i);   // point before elem n12=0
        //         ^
        // now check if the node has essential nodes and find the appropriate position to add them
        if (std::stoul(number) == origin)
        {  // loop detected, break it!
            PRINT_INFO("Loop detected while collecting essential nodes");
            continue;
        }
        if (findFunctionalNode(std::stoi(number), node))
        {
            if (node->hasEssentialNodes())
            {  // now place them on the appropriate position and update the offset for further
                // calculations
                std::string curr = totalEssentialNodes.substr(pos, elem.size()+2);
                totalEssentialNodes.erase(pos+offset, elem.size()+2);
                std::string essentials = "(" + curr + " & (" + node->collectEssentialNodes(origin) + "))";

                totalEssentialNodes.insert(pos + offset, essentials);
                offset += essentials.size() - (elem.size() + 2);
            }
        }
        else
        {  // this should never happen
            ErrorHandler::getInstance().setError(
                    Errors::essentialNodeNotInSetOfFunctionalNodes(mNumber, std::stoi(number)));
            ErrorHandler::getInstance().show();
            continue;
        }
    }
    return totalEssentialNodes;
}

std::string
Node::getRestrictedRecoveryFormula()
{
    std::set<std::set<unsigned int>>* pathes = new std::set<std::set<unsigned int>>();
    std::set<unsigned int>* tmp = new std::set<unsigned int>();
    std::string ret = "";

    findPathToEnv(mNumber, pathes, tmp);

    auto pathesIter = pathes->begin();
    for (; pathesIter != pathes->end(); ++pathesIter)
    {
        auto iter = pathesIter->begin();
        for (; iter != pathesIter->end(); ++iter)
        {
            ret += "n" + std::to_string(*iter) + "=0";
            if (std::next(iter) != pathesIter->end())
            {
                ret += " & ";
            }
        }
        if (std::next(pathesIter) != pathes->end())
        {
            ret += " | ";
        }
    }
    return ret;
}

void
Node::findPathToEnv(unsigned int origin,
                    std::set<std::set<unsigned int>>* pathes,
                    std::set<unsigned int>* path)
{
    unsigned int i = 0;
    std::set<unsigned int> content = (*path);
    std::set<unsigned int>* workingCopy = path;

    for (Node* node : mReachingNodes)
    {
        if (node->getNumber() == origin)
        {  // Do not take paths into account that go back to the original node
            continue;
        }
        if (node->isReachable())
        {  // node is connected to the env, keep going!
            if (i > 0)
            {  // more than one reaching node, copy junction path for other nodes
                workingCopy = new std::set<unsigned int>(content);
            }
            // std::string viewElem = "n" + std::to_string(node->getNumber());

            if (workingCopy->find(node->getNumber()) != workingCopy->end())
            {  // Ignore element if its already included in the path
                continue;
            }

            if (node->isEnvironment())
            {
                // only inserts unique elements
                pathes->insert(*workingCopy);
            }
            else
            {
                workingCopy->insert(node->getNumber());
            }

            node->findPathToEnv(origin, pathes, workingCopy);
        }
        ++i;
    }
}

}  // namespace graph