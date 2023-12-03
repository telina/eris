/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
 *
 * Authors:
 *      - 2020-2023 Rhea Rinaldo (Rhea@Rinaldo.biz)
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

#include "transcriber.h"

#include "model.h"
#include "node.h"
#include "edge.h"
#include "error_handler.h"
#include "operational.h"

#include <QStyleOption>

#include <cmath> /* pow */

#include <bitset>
#include <utility>

using namespace graph;
namespace graphInternal
{
using widgets::ErrorHandler;
using widgets::Errors;

Transcriber::Transcriber(const std::vector<Node*>& envNodes,
                         const std::vector<Node*>& otherNodes,
                         std::string redundancy,
                         std::string outFileName) :
    mRedundancy(std::move(redundancy)),
    mOutfileName(std::move(outFileName)),
    mEnvNodes(envNodes),
    mNodes(otherNodes)
{
    if (Model::getInstance().getType() == Model::CTMC)
    {
        mModelAsString = "ctmc";
        mVarDecl = "const double rn";
        mVarUsg = "r";
    }
    else
    {
        mModelAsString = "mdp";
        mVarDecl = "const double pn";
        mVarUsg = "p";
    }
}

Transcriber::~Transcriber()
{
}

void
Transcriber::setOutfileName(std::string name)
{
    mOutfileName = std::move(name);
}

void
Transcriber::generatePermutations(unsigned int nodeCount, std::list<std::string>* permutations)
{
    unsigned int permutationCount = pow(2, nodeCount);
    unsigned int binaryPermutation = 0;
    for (; binaryPermutation < permutationCount; ++binaryPermutation)
    {
        std::string currentPermutation;
        for (unsigned int i = 0; i < nodeCount; ++i)
        {
            currentPermutation = std::to_string((binaryPermutation >> i) & 1) + currentPermutation;
        }

        permutations->push_back(currentPermutation);
    }
}

void
Transcriber::assembleSafetyTransition(const std::string& start, const std::string& end, Node* node)
{
    std::string restProb = "";
    std::string currTransition = "[] (" + node->getStringRepresentation() + "=" + start
                                 + ") & (operational) -> " + mVarUsg
                                 + node->getStringRepresentation() + "SAFE : ("
                                 + node->getStringRepresentation() + "'=" + end + ")";

    if (node->hasEssentialNodes() && start == "0")
    {
        // set internalfailure flag to identify the cause of the defect
        // (only needed to recover nodes that have essentialnodes)
        // optimization: may be left out for critical nodes that are not redundant
        if (node->isRecoverableFromDefect())
        {
            currTransition += " & (" + node->getStringRepresentation() + "internalfailure'=true)";
        }

        // if node has essential nodes, it turns defective if essential nodes fail
        std::string essentialConst = "formula " + node->getStringRepresentation()
                                     + "essentials = " + node->getEssentialNodes() + ";";
        std::string essentialTransition = "[] (" + node->getStringRepresentation() + "=0) & (!"
                                          + node->getStringRepresentation() + "essentials"
                                          + ") & (operational)-> ("
                                          + node->getStringRepresentation() + "'=1);";
        mTransitions.push_back(essentialTransition);
        mConstants.push_back(essentialConst);
    }

    if (Model::getInstance().getType() == Model::MDP)
    {
        restProb = " + 1-" + mVarUsg + node->getStringRepresentation() + "SAFE" + " : ("
                   + node->getStringRepresentation() + "'=" + start + ")";
    }
    currTransition += restProb + ";";
    mTransitions.push_back(currTransition);
}

void
Transcriber::assembleSecurityTransition(const std::string& init, Node* node)
{
    if (node->hasSecuringNodes())
    {
        std::list<std::string> permutations;
        generatePermutations(node->getSecuringNodes().size(), &permutations);

        for (std::string permutation : permutations)
        {
            // check whether a permutation evaluates to zero or below and skip it if so!
            double indicator = std::stod(node->getIntrusionIndicator());
            for (unsigned int i = 0; i < permutation.size(); ++i)
            {
                if (permutation[i] == '0')
                {  // Node is ok
                    indicator -= std::stod(node->getSecuringNodes()[i]->getSecurityIndicator());
                }
            }
            if (indicator <= 0.0)
            {  // TODO: do we need to differentiate between 0 and below 0 ?
                ErrorHandler::getInstance().setError(
                        Errors::invalidTransitionsRemoved(node->getStringRepresentation(), "SEC"));
                continue;
            }
            std::string currTransition = "[] " + init;
            std::string guard = "(";
            std::string consequence;
            std::string rest;
            for (unsigned int i = 0; i < permutation.size(); ++i)
            {
                std::string currSecNode = node->getSecuringNodes()[i]->getStringRepresentation();
                if (permutation[i] == '0')
                {  // node is ok
                    guard += currSecNode + "=0 & ";
                    consequence += "-" + mVarUsg + currSecNode + "GUAR";
                }
                else
                {
                    guard += currSecNode + "!=0 & ";
                }
            }
            guard = guard.substr(0, guard.size() - 3);
            guard += ")";
            currTransition += " & " + guard + " & (operational) -> " + mVarUsg
                              + node->getStringRepresentation() + "SEC" + consequence + " : ("
                              + node->getStringRepresentation() + "'=2)";
            if (Model::getInstance().getType() == Model::MDP)
            {
                rest = "+ 1-(" + mVarUsg + node->getStringRepresentation() + "SEC" + consequence
                       + ") : (" + node->getStringRepresentation() + "'=2)";
            }
            currTransition += rest + ";";
            mTransitions.push_back(currTransition);
        }
    }
    else
    {
        std::string currTransition = "[] " + init + " & (operational) -> " + mVarUsg
                                     + node->getStringRepresentation() + "SEC" + " : ("
                                     + node->getStringRepresentation() + "'=2)";
        std::string rest;
        if (Model::getInstance().getType() == Model::MDP)
        {
            rest = "+ 1-" + mVarUsg + node->getStringRepresentation() + "SEC" + " : ("
                   + node->getStringRepresentation() + "'=2)";
        }
        currTransition += rest + ";";

        mTransitions.push_back(currTransition);
    }
}

void
Transcriber::assembleCorRecoveryTransition(Node* node)
{
    std::string transition;
    std::string formula = "pathes" + node->getStringRepresentation() + "CORREC";
    switch (node->getCorruptionRecoveryStrategy())
    {
        case Recovery::Strategy::general:
        {  // Node can always recover
            transition = "[] (" + node->getStringRepresentation() + "=2) & (operational) -> "
                         + mVarUsg + node->getStringRepresentation() + "CORREC : ("
                         + node->getStringRepresentation() + "'=0)";
            if (Model::getInstance().getType() == Model::MDP)
            {
                std::string rest = "1-" + mVarUsg + node->getStringRepresentation() + "CORREC"
                                   + " : (" + node->getStringRepresentation() + "'=2)";
                transition += " + " + rest;
            }
            transition += ";";
            mTransitions.push_back(transition);
            break;
        }
        case Recovery::Strategy::restricted:
        {  // Node can recover if non-corrupted reach-path to
            // env exists
            /* TODO
            if (node->isReachableFromEnv())
            {
                ErrorHandler::getInstance().setError(
                    Errors::restrictedRecoveryReachableFromEnv(node->getStringRepresentation()));
                node->setCorrStrategy
                assembleCorRecoveryTransition(node);
            }*/
            std::string restrictedFormula = node->getRestrictedRecoveryFormula();
            if (restrictedFormula.empty())
            {
                ErrorHandler::getInstance().setError(
                        Errors::restrictedRecoveryTransitionEmpty(node->getStringRepresentation()));
            }
            else
            {
                mConstants.push_back("formula " + formula + " = "+ restrictedFormula + ";");
                transition = "[] (" + node->getStringRepresentation() + "=2) & (operational) & ("
                             + formula + ") -> " + mVarUsg + node->getStringRepresentation()
                             + "CORREC : (" + node->getStringRepresentation() + "'=0)";
                if (Model::getInstance().getType() == Model::MDP)
                {
                    std::string rest = "1-" + mVarUsg + node->getStringRepresentation() + "CORREC"
                                       + " : (" + node->getStringRepresentation() + "'=2)";
                    transition += " + " + rest;
                }
                transition += ";";
                mTransitions.push_back(transition);
            }

            break;
        }
        case Recovery::Strategy::custom:
        {  // Node can recover by given user definition
            mConstants.push_back("formula "+ formula + " = "
                                 + node->getCustomCorrRecoveryFormula() + ";");
            transition = "[] (" + node->getStringRepresentation() + "=2) & (operational) & ("
                         + formula + ") -> " + mVarUsg + node->getStringRepresentation()
                         + "CORREC : (" + node->getStringRepresentation() + "'=0)";
            if (Model::getInstance().getType() == Model::MDP)
            {
                std::string rest = "1-" + mVarUsg + node->getStringRepresentation() + "CORREC"
                                   + " : (" + node->getStringRepresentation() + "'=2)";
                transition += " + " + rest;
            }
            transition += ";";
            mTransitions.push_back(transition);
            break;
        }
        default:
        {
            break;
        }
    }
}

void
Transcriber::assembleDefRecoveryTransition(Node* node,
                                           const std::string& init,
                                           const std::string& end)
{
    std::string transition = init;
    std::string formula = "pathes" + node->getStringRepresentation() + "DEFREC";

    switch (node->getDefectRecoveryStrategy())
    {
            //  [] (n1=1 & n1internalfailure=true) & (n1essentials) -> (n1'=0) &
            //  (n1internalfailure'=false);'

        case Recovery::Strategy::general:
        {  // Node can always recover
            transition += "-> " + mVarUsg + node->getStringRepresentation() + "DEFREC : ("
                          + node->getStringRepresentation() + "'=0)" + end;
            if (Model::getInstance().getType() == Model::MDP)
            {
                std::string rest = "1-" + mVarUsg + node->getStringRepresentation() + "DEFREC"
                                   + " : (" + node->getStringRepresentation() + "'=1)";
                transition += " + " + rest;
            }
            transition += ";";
            mTransitions.push_back(transition);
            break;
        }
        case Recovery::Strategy::restricted:
        {  // Node can recover if non-corrupted reach-path to
            // env exists
            std::string restrictedFormula = node->getRestrictedRecoveryFormula();
            if (restrictedFormula.empty())
            {
                ErrorHandler::getInstance().setError(
                        Errors::restrictedRecoveryTransitionEmpty(node->getStringRepresentation()));
                break;
            }
            else
            {
                mConstants.push_back("formula "+ formula + " = " + restrictedFormula + ";");
                transition += " & (" + formula + ") -> " + mVarUsg + node->getStringRepresentation()
                              + "DEFREC : (" + node->getStringRepresentation() + "'=0)" + end;
                if (Model::getInstance().getType() == Model::MDP)
                {
                    std::string rest = "1-" + mVarUsg + node->getStringRepresentation() + "DEFREC"
                                       + " : (" + node->getStringRepresentation() + "'=1)";
                    transition += " + " + rest;
                }
                transition += ";";
                mTransitions.push_back(transition);
                break;
            }
        }
        case Recovery::Strategy::custom:
        {  // Node can recover by given user definition
            // TODO show warning if mode is custom but no nodes are provided
            mConstants.push_back("formula " + formula + " = "
                                 + node->getCustomDefRecoveryFormula() + ";");
            transition += " & (" + formula + ") -> " + mVarUsg + node->getStringRepresentation()
                          + "DEFREC : (" + node->getStringRepresentation() + "'=0)" + end;
            if (Model::getInstance().getType() == Model::MDP)
            {
                std::string rest = "1-" + mVarUsg + node->getStringRepresentation() + "DEFREC"
                                   + " : (" + node->getStringRepresentation() + "'=1)";
                transition += " + " + rest;
            }
            transition += ";";
            mTransitions.push_back(transition);
            break;
        }
        default:
        {
            break;
        }
    }
}

void
Transcriber::buildAutomaton()
{
    std::string currNode;
    std::string operational;      // correct functionality; system is working!
    std::string normalDefective;  // In case no critical nodes are given
    std::string normalCorrupted;  // In case no critical nodes are given
    std::string corrupted = "";
    std::string systemfailure = "label \"systemfailure\" = !operational;";
    std::string defective = "";
    bool crit = false;
    for (Node* node : mNodes)
    {
        currNode = node->getStringRepresentation();
        // Add variable for node
        mVariables.push_back(std::string(currNode + ": [0..2] init 0;"));

        if (node->hasValidFailureIndicator())
        {
            // ----- n=0 -> n=1 -----
            // Safety transition
            assembleSafetyTransition("0", "1", node);
            // ----- n=1 -> n=0 -----
            // Recovery Transition from defective state to ok stat
            if (node->isRecoverableFromDefect() && node->hasValidDefectRecoveryIndicator())
            {
                std::string init =
                        "[] (" + node->getStringRepresentation() + "=1) & (operational) ";
                std::string end = "";
                if (node->hasEssentialNodes())
                {
                    std::string defState = node->getStringRepresentation() + "internalfailure";
                    init += "& (" + defState + ")";
                    end += " & (" + node->getStringRepresentation() + "internalfailure'=false)";
                    defState += ": bool init false;";
                    mVariables.push_back(defState);
                }
                assembleDefRecoveryTransition(node, init, end);
            }
        }

        if (node->isReachable()) 
        {  // Node is reachable when continuous reach edges from the env node to it exist
            // ----- n=0 -> n=2 -----
            // Security transition
            if (node->isReachableFromEnv() && node->hasValidIntrusionIndicator())
            {  // Node is directly attached to Env!
                std::string init = "(" + currNode + "=0)";
                assembleSecurityTransition(init, node);
            }

            // ----- n=2 -> n=1 -----
            if (node->hasValidFailureIndicator())
            {
                assembleSafetyTransition("2", "1", node);
            }

            // Corruption of node can be used to attack other nodes
            // ----- n=2 -> n'=2 -----
            if (node->hasReachableNodes())
            {
                for (Node* reachNode : node->getReachableNodes())
                {
                    std::string init = "(" + currNode + "=2 & "
                                       + reachNode->getStringRepresentation() + "=0)";
                    assembleSecurityTransition(init, reachNode);
                }
            }
            // ----- n=2 -> n=0 -----
            // Recovery Transition from corrupted state to ok state, if applicable
            if (node->isRecoverableFromCorruption() && node->hasValidCorruptionRecoveryIndicator())
            {
                assembleCorRecoveryTransition(node);
            }
        }

        // ----- critical essential nodes add up to the mode of operation -----
        if (node->isCritical())
        {
            if (Operational::getInstance().getMode() == Operational::Mode::optimized)
            {
                operational += "(" + currNode + "=0";
                if (node->hasEssentialNodes())
                {
                    operational +=
                            " & (" + node->collectEssentialNodes(node->getNumber()) + ")";
                }
                if (node->hasRedundantNodes())
                {
                    operational += " | " + node->getRedundantAndEssentialNodesAsString(); 
                }
            }
            else
            {
                operational += "(" + currNode + "=0";
                if (node->hasRedundantNodes())
                {
                    operational += " | " + node->getRedundantNodesAsString(); 
                }
            }
            operational += ") & ";

            if (node->hasRedundantNodes())
            {
                corrupted += "(" + currNode + "=2 & " + node->getRedundantNodesAsString("2") + ") | "; // returns "nx=2" or "nx=2 & nz=2 ..."
            }
            else
            {
                corrupted += currNode + "=2 | ";
            }
            crit = true;
        }
        // ----- n=2 + normal essential nodes -----
        else
        {  // Safety transition: a corrupted node may turn defective (critical nodes can't break
            // more)
            normalDefective += currNode + "=0";
            normalCorrupted += "(" + currNode + "=2) & ";// required to always print corrupted label
            // If the node is corrupted the attacker may use it to corrupt further nodes:
            if (node->hasEssentialNodes())
            {
                normalDefective += " | " + node->getEssentialNodes();
            }
            normalDefective += " | ";
        }
    }

    if (crit)  // at least one critical node given
    {          // otherwise corrupted label has not been set and should not be printed
        if (!corrupted.empty())
        {
            corrupted.insert(0, "label \"corrupted\" = ");
            corrupted.replace(corrupted.size() - 2, 2, ";");
        }
        // remove last logic operator and space; cannot be empty as critical nodes exist
        operational = operational.substr(0, operational.size() - 2);
    }
    else
    {  // no critical nodes given, default mode of operation has to be set!
        // remove last & and space
        normalDefective = normalDefective.substr(0, normalDefective.size() - 2);
        normalCorrupted = normalCorrupted.substr(0, normalCorrupted.size() - 2);
        operational += normalDefective;

        // default mode even though reach links may not exist, nodes are included
        // This is a workaround, as the corrupted label might be required for an experiment
        corrupted.insert(0, "label \"corrupted\" = ");
        corrupted += normalCorrupted + ";";
    }
    operational += ";";

    defective = operational;
    char prev = ' ';
    for (char& c : defective)
    {
        if (c == ' ')
        {
            continue;
        }
        else if (c == '0' && prev == '=')
        {
            c = '1';
        }
        else if (c == '&')
        {
            c = '|';
        }
        else if (c == '|')
        {
            c = '&';
        }
        prev = c;
    }
    operational.insert(0, "formula operational = ");
    defective.insert(0, "label \"defective\" = ");

    mConstants.push_back(operational);
    mConstants.emplace_back("");  // newline
    mVariables.emplace_back("");
    mLabels.push_back(systemfailure);
    mLabels.push_back(defective);
    mLabels.push_back(corrupted);
}

void
Transcriber::buildModel()
{
    for (Node* node : mNodes)
    {  // Collect Variables (Component Probabilities)
        mConstants.push_back(std::string(mVarDecl + std::to_string(node->getNumber())
                                         + "SEC = " + node->getIntrusionIndicator() + ";"));
        mConstants.push_back(std::string(mVarDecl + std::to_string(node->getNumber())
                                         + "SAFE = " + node->getFailureIndicator() + ";"));
        mConstants.push_back(std::string(mVarDecl + std::to_string(node->getNumber())
                                         + "GUAR = " + node->getSecurityIndicator() + ";"));
        if (node->isRecoverableFromDefect())
        {
            mConstants.push_back(std::string(mVarDecl + std::to_string(node->getNumber())
                                             + "DEFREC = " + node->getDefectRecoveryIndicator()
                                             + ";"));
        }
        if (node->isRecoverableFromCorruption())
        {
            mConstants.push_back(std::string(mVarDecl + std::to_string(node->getNumber())
                                             + "CORREC = " + node->getCorruptionRecoveryIndicator()
                                             + ";"));
        }
    }
    mConstants.emplace_back("");  // newline

    buildAutomaton();
    generateFile();
}

void
Transcriber::generateFile()
{
    // open file
    std::ofstream outfile;
    outfile.open(mOutfileName, std::ios::out | std::ios::trunc);
    if (outfile.is_open())
    {
        outfile << mModelAsString << "\n" << std::endl;

        // write constant variables
        for (auto it = mConstants.begin(); it != mConstants.end(); ++it)
        {
            outfile << *it << std::endl;
        }

        // begin module
        outfile << "module generatedScenario\n" << std::endl;

        // write variable and state declarations
        for (auto it = mVariables.begin(); it != mVariables.end(); ++it)
        {
            outfile << *it << std::endl;
        }
        // outfile << "s : [0.." << mTransitions.size() << "] init 0\n" << std::endl;

        // write state transitions
        for (auto it = mTransitions.begin(); it != mTransitions.end(); ++it)
        {
            outfile << *it << std::endl;
        }
        // end module
        outfile << "\n" << std::endl;
        outfile << "endmodule" << std::endl;

        for (auto it = mLabels.begin(); it != mLabels.end(); ++it)
        {
            outfile << *it << std::endl;
        }
    }
    outfile.close();
    clear();
}

void
Transcriber::clear()
{
    mConstants.clear();
    mVariables.clear();
    mTransitions.clear();
    mLabels.clear();
}

}  // namespace graph