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

#include "transformer.h"
#include "graphic_scene.h"
#include "task.h"
#include "node_settings_validator.h"
#include "error_handler.h"
#include "errors.h"
#include "logger.h"
#include "node.h"
#include "edge.h"
#include "edge_item.h"
#include "node_item.h"
#include "transcriber.h"
#include "utils.h"
#include "scene_status.h"
#include "main_window.h"
#include "xprism.h"
#include "checks.h"

#include <utility>
#include <QProcess>
#include <QLabel>

namespace graphInternal
{
using eval::XPrism;
using widgets::ErrorHandler;
using widgets::Errors;
using widgets::MainWindow;

Transformer::Transformer(GraphicScene* graphicScene) : mScene(graphicScene), mProcess(nullptr)
{
    
}

Transformer::~Transformer() = default;

bool
Transformer::start(const std::string& redundancyDefinition, const std::string& outFileName)
{
    ERIS_CHECK(!outFileName.empty());
    mRedundancy = redundancyDefinition;
    mOutFileName = outFileName;
    mRunning = true;

    mProcess.reset(new (std::nothrow) QProcess());
    const bool success = transform(mOutFileName);

    // emit transformationFinished(success);

    DeprecatedTransformationFinished(success);
    return success;
}

void
Transformer::stop()
{
    if (mProcess != nullptr)
    {  // be sure mProcess was initialised...s
        mProcess->kill();
    }
    mRunning = false;
}

bool
Transformer::transform(std::string& outfile)
{
    std::vector<Node*> envNodes;
    std::vector<Node*> nodes;
    if (generateLogicRepresentation(envNodes, nodes))
    {

        std::sort(nodes.begin(), nodes.end(),[] (Node* const& n1, Node* const& n2) 
        {return n1->getNumber() < n2->getNumber(); });


        Transcriber transcriber(envNodes, nodes, mRedundancy, outfile);
        transcriber.buildModel();

        mScene->setStateChanged(false);
        // DeprecatedSceneStatus::getInstance().setChanged(false);
        return true;
    }
    else
    {  // something went wrong
        return false;
    }
}

bool
Transformer::getNodeById(const std::vector<Node*>& nodes, unsigned int id, Node*& node)
{
    for (Node* elem : nodes)
    {
        if (elem->getNumber() == id)
        {
            node = elem;
            return true;
        }
    }
    ErrorHandler::getInstance().setError(Errors::noNodeWithId(id));
    return false;
}

bool
Transformer::startSimulationProcess(NodeItem* nodeItem)
{
    // Check if octave is available
    if (!utils::commandExists("octave"))
    {
        ErrorHandler::getInstance().setError(Errors::octaveCommandNotAvailable());
        return false;
    }
    QString command = "octave ";  //+ nodeItem->getSimulationPath();
    QStringList arguments;
    arguments.append(nodeItem->getSimulationPath());
    // arguments.append(">> /home/telina/Development/simResults.txt");
    mProcess->setArguments(arguments);
    mProcess->setProgram(command);
    mProcess->start();
    if (!mProcess->waitForStarted())
    {
        ErrorHandler::getInstance().setError(
                Errors::simulationStartError("octave", mProcess->readAllStandardError()));
        return false;
    }
    mProcess->waitForFinished(-1);  // wait until the simulation is done

    QString errorStr = QString::fromLatin1(mProcess->readAllStandardError());
    if (!errorStr.isEmpty())
    {
        ErrorHandler::getInstance().setError(Errors::simulationError("octave", errorStr));
        return false;
    }
    std::string output = mProcess->readAllStandardOutput().toStdString();
    std::regex lambdaSafe("Lambda_Safe\\s*=\\s*([0-9]+(\\.[0-9]+)?(e-?[0-9]+)?)");
    std::regex lambdaSec("Lambda_Sec\\s*=\\s*([0-9]+(\\.[0-9]+)?(e-?[0-9]+)?)");
    std::smatch safeMatch;
    if (std::regex_search(output, safeMatch, lambdaSafe))
    {
        if (utils::isValidInputValue(
                    safeMatch[1].str(), "Failure", Model::getInstance().getType()))
        {
            nodeItem->setFailureIndicator(QString::fromStdString(safeMatch[1].str()));
        }
        else
        {
            return false;
        }
    }
    else
    {  // Error
        ErrorHandler::getInstance().setError(Errors::simulationOutputError("Lambda_Safe"));
        return false;
    }
    if (std::regex_search(output, safeMatch, lambdaSec))
    {
        if (utils::isValidInputValue(
                    safeMatch[1].str(), "Intrusion", Model::getInstance().getType()))
        {
            nodeItem->setIntrusionIndicator(QString::fromStdString(safeMatch[1].str()));
        }
        else
        {
            return false;
        }
    }
    else
    {
        ErrorHandler::getInstance().setError(Errors::simulationOutputError("Lambda_Sec"));
        return false;
    }

    return true;
}

bool
Transformer::generateLogicRepresentation(std::vector<Node*>& envNodes,
                                         std::vector<Node*>& otherNodes)
{
    Node* node;
    Edge* edge;
    std::vector<NodeItem*> envNodeItems;
    std::vector<NodeItem*> nodeItems;
    std::vector<EdgeItem*> edgeItems;
    mScene->getSortedSceneItems(nodeItems, envNodeItems, edgeItems);

    if (nodeItems.empty())
    {
        ErrorHandler::getInstance().setError(Errors::missingNodes());
        return false;
    }
    for (NodeItem* envNodeItem : envNodeItems)
    {
        node = new Node(envNodeItem->getComponentType(), envNodeItem->getId());
        envNodes.push_back(node);
    }
    for (NodeItem* nodeItem : nodeItems)
    {
        if (!mRunning)
        {  // Stop was requested
            ErrorHandler::getInstance().clearCollection();
            ErrorHandler::getInstance().setError(Errors::transformationStopped());
            return false;
        }

        node = new Node(nodeItem->getComponentType(),
                        nodeItem->getId(),
                        nodeItem->isRecoverableFromDefect(),
                        nodeItem->isRecoverableFromCorruption(),
                        nodeItem->getIntrusionIndicator().toStdString(),
                        nodeItem->getFailureIndicator().toStdString(),
                        nodeItem->getSecurityIndicator().toStdString(),
                        nodeItem->getDefectRecoveryIndicator().toStdString(),
                        nodeItem->getCorruptionRecoveryIndicator().toStdString(),
                        nodeItem->getEssentialNodes().toStdString(),
                        nodeItem->getCustomCorruptionRecoveryFormula().toStdString(),
                        nodeItem->getCorruptionRecoveryStrategy(),
                        nodeItem->getCustomDefectRecoveryFormula().toStdString(),
                        nodeItem->getDefectRecoveryStrategy());

    
        otherNodes.push_back(node);
    }

    std::vector<Node*> totalNodes = envNodes;
    totalNodes.insert(totalNodes.end(), otherNodes.begin(), otherNodes.end());

    for (EdgeItem* edgeItem : edgeItems)
    {
        Node* start;
        Node* end;
        if (!getNodeById(totalNodes, edgeItem->startItem()->getId(), start)
            || !getNodeById(totalNodes, edgeItem->endItem()->getId(), end))
        {  // Somehow a node got lost (Should not happen), Error was set and function terminates
            // unsuccessfully!
            return false;
        }
        edge = new Edge(start, end, edgeItem->getComponentType());
        start->addEdge(edge);
        end->addEdge(edge);
    }
    processRedundancy(otherNodes);
    return true;
}

void
Transformer::processRedundancy(const std::vector<Node*>& nodes)
{
    std::regex nodeElem("(n[0-9][0-9]*)");
    // Iterate over redundancy string. Seperate by "," delimiter
    std::string token;
    std::stringstream ss(mRedundancy);
    Node* nodeA;
    Node* nodeB;
    std::string redundantStr;
    while (getline(ss, token, ','))
    {  // node redundancies are separated, collect nodes and add the logic!
        std::regex elem("(n[0-9][0-9]*)");
        std::sregex_iterator iter(token.begin(), token.end(), elem);
        unsigned int idA = std::stoi(std::string((*iter)[1]).substr(1));
        iter++;
        unsigned int idB = std::stoi(std::string((*iter)[1]).substr(1));

        getNodeById(nodes, idA, nodeA);
        getNodeById(nodes, idB, nodeB);
        
        nodeA->addRedundantNode(nodeB);
        nodeB->addRedundantNode(nodeA);
    }

    ErrorHandler::getInstance().clear();
}
void
Transformer::DeprecatedTransformationFinished(bool success)
{
    MainWindow::getInstance()->mInformationLabel->clear();
    MainWindow::getInstance()->disableGuiElements(false);
    // Display Warnings that occurred during the
    // transformation and transcription
    ErrorHandler::getInstance().showErrorCollection();  

    if (success)
    {
        if (mScene->isPrismMode() && !XPrism::getInstance()->isRunning())
        {
            XPrism::getInstance()->setArguments(QStringList() << tr(mOutFileName.c_str()));

            if (!XPrism::getInstance()->runDetached(nullptr))
            {
                ErrorHandler::getInstance().setError(Errors::programExecutionError("xprism"));
                ErrorHandler::getInstance().show();
            }
            mScene->setPrismMode(false);
        }
    }
}

}  // namespace graph
