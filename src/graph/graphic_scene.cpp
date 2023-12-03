/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
 *
 * Authors:
 *      - 2020-2023 Rhea Rinaldo (Rhea@Rinaldo.biz)
 *      - 2021-2021 Mohammed Hussam Al Turjman (Mohammed_Hussam.Al_Turjman@dfki.de)
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

#include "graphic_scene.h"
#include "error_handler.h"
#include "node_item.h"
#include "utils.h"
#include "counter.h"
#include "component_type.h"
#include "errors.h"
#include "node_item.h"
#include "edge_item.h"
#include "text_item.h"
#include "logger.h"
#include "transformer.h"
#include "main_window.h"
#include "checks.h"
#include "graphics_view.h"
#include "scene_status.h"
#include "qt_utils.h"
#include "evaluation_settings.h"
#include "prism.h"
#include "tokenizer.h"
#include "qt_utils.h"
#include "model.h"
#include "file_manager.h"
#include "node_settings_validator.h"
#include "main_window_manager.h"

#include "counter.h"
#include "octave.h"
#include <QGraphicsTransform>
#include <QGuiApplication>
#include <QFileDialog>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QInputDialog>
#include <QMessageBox>
#include <QLabel>
#include <QButtonGroup>
#include <QToolButton>
#include <QAction>
#include <QScrollBar>

namespace graph
{
using namespace utils;
using eval::Prism;
using eval::Octave;
using widgets::ErrorHandler;
using widgets::Errors;
using widgets::EvaluationSettingsDialog;
using widgets::GraphicsView;
using widgets::MainWindow;
using widgets::MainWindowButtonsGroupManager;
using widgets::MainWindowToolButtonsManager;

static int
NextId()
{
    static int id = 0;
    return id++;
}

GraphicScene::GraphicScene(QObject* parent, bool passive_submodule) :
        QGraphicsScene(parent), mLine(nullptr), mId(NextId()), 
        mIsPassiveSubmodule(passive_submodule), mTransformer(nullptr),
        mActiveRateInterpretation(graph::RateInterpretation::Unit::hourly)
{
    mMode = ModifyItem;
}

GraphicScene::~GraphicScene()
{
    auto current_items = items();
    for (int i = 0; i < current_items.size(); ++i)
    {
        if (current_items[i]->type() == NodeItem::Type)
        {
            dynamic_cast<NodeItem*>(current_items[i])->InvalidateScene();
        }
    }
}

void
GraphicScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    mOriginPos = mouseEvent->scenePos();

    if (mouseEvent->button() == Qt::LeftButton)
    {
        NodeItem* nodeItem;
        TextItem* textItem;
        bool ok;
        QString text;
        switch (mMode)
        {
            case InsertNormalNode:
                nodeItem = new NodeItem(ComponentType::normalNode, mCounter->getNext(), 
                        mouseEvent->scenePos() - QPointF(NodeItem::Radius, NodeItem::Radius));
                if (!nodeOverlaps(nodeItem))
                {
                    QGraphicsScene::addItem(nodeItem);
                    emit itemInserted();
                }
                else
                {
                    delete nodeItem;
                }
                break;
            case InsertCriticalNode:
                nodeItem = new NodeItem(ComponentType::criticalNode, mCounter->getNext(),
                        mouseEvent->scenePos() - QPointF(NodeItem::Radius, NodeItem::Radius));
                if (!nodeOverlaps(nodeItem))
                {
                    this->addItem(nodeItem);
                    emit itemInserted();
                }
                else
                {
                    delete nodeItem;
                }
                break;
            case InsertEnvironmentNode:
                nodeItem = new NodeItem(ComponentType::environmentNode, mCounter->getNext(), 
                        mouseEvent->scenePos() - QPointF(NodeItem::Radius, NodeItem::Radius));
                if (!nodeOverlaps(nodeItem))
                {
                    this->addItem(nodeItem);
                    emit itemInserted();
                }
                else
                {
                    delete nodeItem;
                }
                break;
            case InsertReachEdge:
                mLine = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), 
                        mouseEvent->scenePos()));
                this->addItem(mLine);
                mLineType = ComponentType::reachEdge;
                emit itemInserted();
                break;
            case InsertFunctionalEdge:
                mLine = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), 
                        mouseEvent->scenePos()));
                this->addItem(mLine);
                mLineType = ComponentType::functionalEdge;
                emit itemInserted();
                break;
            case InsertSecurityEdge:
                mLine = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), 
                        mouseEvent->scenePos()));
                this->addItem(mLine);
                mLineType = ComponentType::securityEdge;
                emit itemInserted();
                break;
            case InsertText:
                ok = false;
                text = QInputDialog::getText(nullptr, "Input dialog", "Text:", 
                        QLineEdit::Normal, "", &ok);
                if (ok)
                {
                    if (text.isEmpty() || isOnlyWhiteSpaces(text.toStdString()))
                    {
                        ErrorHandler::getInstance().setError(Errors::invalidTextItem());
                        ErrorHandler::getInstance().show();
                    }
                    else
                    {
                        textItem = new TextItem(text, mouseEvent->scenePos());
                        this->addItem(textItem);
                    }
                    break;
                }
                case ModifyItem:
                if (isItemAt(mouseEvent->scenePos()))
                { 
                    for (int i = 0; i < GRAPHIC_SCENE_FACTORY()->size(); ++i)
                    { // in case a different item is selected, mode is not changed but selection is
                        emit GRAPHIC_SCENE_FACTORY()->at(i)->itemSelected();
                    }
                }
                else
                {// if there is no item at the spot also deselect on left click, else keep moving
                    for (int i = 0; i < GRAPHIC_SCENE_FACTORY()->size(); ++i)
                    {
                        emit GRAPHIC_SCENE_FACTORY()->at(i)->itemDeselected();
                    }
                }
                break;
            case Navigate:
                if (isItemAt(mouseEvent->scenePos()))
                {
                    for (int i = 0; i < GRAPHIC_SCENE_FACTORY()->size(); ++i)
                    {
                        emit GRAPHIC_SCENE_FACTORY()->at(i)->itemSelected();
                    }
                }
                break;
            default:;
        }
    }
    else if (mouseEvent->button() == Qt::RightButton || mouseEvent->button() == Qt::MiddleButton)
    {
        for (int i = 0; i < GRAPHIC_SCENE_FACTORY()->size(); ++i)
        {
            emit GRAPHIC_SCENE_FACTORY()->at(i)->itemDeselected();
        }
    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}

bool
GraphicScene::nodeOverlaps(NodeItem* newItem)
{
    NodeItem* nodeItem;
    for (QGraphicsItem* item : this->items())
    {
        if (item->type() == NodeItem::Type)
        {
            nodeItem = qgraphicsitem_cast<NodeItem*>(item);
            if ((nodeItem != newItem) && nodeItem->collidesWithItem(newItem))
            {
                ErrorHandler::getInstance().setError(Errors::positionAlreadyOccupied());
                ErrorHandler::getInstance().show();
                return true;
            }
        }
    }
    return false;
}

void
GraphicScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    if (mLine != nullptr && (mMode == InsertReachEdge || mMode == InsertFunctionalEdge
            || mMode == InsertSecurityEdge))
    {
        QLineF newLine(mLine->line().p1(), mouseEvent->scenePos());
        mLine->setLine(newLine);
    }
    else if (mMode == ModifyItem)
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void
GraphicScene::updateCollidingEdges(std::vector<EdgeItem*> collidingEdges)
{
    int count = collidingEdges.size();  // ausgehend von der mittelline abwechselnd + - offset?
    int offset_in = 4;
    int offset_out = 4;
    NodeItem* start = collidingEdges.front()->startItem();
    NodeItem* end = collidingEdges.front()->endItem();

    for (EdgeItem* edgeItem : collidingEdges)
    {
        if (edgeItem->startItem() == start)
        {
            edgeItem->setOffset(offset_out);
            offset_out += 8;
        }
        else if (edgeItem->startItem() == end)
        {
            edgeItem->setOffset(offset_in);
            offset_in += 8;
        }
        else
        {
            PRINT_ERROR("Colliding Edge with incorrect start and end");
        }
        edgeItem->update();
        --count;
    }
}

void
GraphicScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    if (mLine != nullptr && (mMode == InsertReachEdge || mMode == InsertFunctionalEdge
            || mMode == InsertSecurityEdge))
    {
        NodeItem* startNode = getNodeItemAt(mLine->line().p1());
        NodeItem* endNode = getNodeItemAt(mLine->line().p2());

        removeItem(mLine);
        delete mLine;
        mLine = nullptr;

        if (startNode != nullptr && endNode != nullptr)
        {
            // Environment node: is unique, multiple are just syntactic sugar
            // Edges must not exist twice, edges must not exist targeting the env node

            if (startNode == endNode)
            {
                ErrorHandler::getInstance().setError(Errors::selfEdges());
                ErrorHandler::getInstance().show();
                return;
            }
            else if (endNode->getComponentType() == ComponentType::environmentNode)
            {
                ErrorHandler::getInstance().setError(Errors::edgeMustNotDirectToEnvironment());
                ErrorHandler::getInstance().show();
                return;
            }
            else if (startNode->getComponentType() == ComponentType::environmentNode)
            {
                if (edgeToOtherEnvExists(endNode))
                {
                    ErrorHandler::getInstance().setError(Errors::edgeAlreadyExists());
                    ErrorHandler::getInstance().show();
                    return;
                }
                else if (mLineType != ComponentType::reachEdge)
                {
                    ErrorHandler::getInstance().setError(
                            Errors::incorrectEdgeTypeForEnvironmentNode());
                    ErrorHandler::getInstance().show();
                    return;
                }
            }
            else if (startNode->edgeWithTypeExists(endNode, mLineType))
            {
                ErrorHandler::getInstance().show();  // show error message
                return QGraphicsScene::mouseReleaseEvent(mouseEvent);
            }
            EdgeItem* edgeItem = new EdgeItem(mLineType, startNode, endNode);
            startNode->addEdgeItem(edgeItem);
            endNode->addEdgeItem(edgeItem);
            edgeItem->setZValue(-1000.0);
            QGraphicsScene::addItem(edgeItem);
            edgeItem->updatePosition();
            std::vector<EdgeItem*> collidingEdges =
                    startNode->getCollidingEdgeItems(endNode);  // gets edges of both directions!
            if (collidingEdges.size() > 1)                      // collisions were found
            {                                                   // Fix existing edges
                updateCollidingEdges(collidingEdges);
            }
        }
    }
    else if (mMode == ModifyItem || mMode == Navigate) // should never be Navigate
    {
        if (mLine != nullptr)
        {
            removeItem(mLine);
            delete mLine;
            mLine = nullptr;
        }
    
    
        QGraphicsScene::mouseReleaseEvent(mouseEvent);
        int nodeCount = 0;

        for (auto& item : selectedItems())
        {
            if (item->type() == NodeItem::Type)
            {
                nodeCount++;
            }
            else
            {
                nodeCount = 0;
                break;
            }
        }
        emit enableSwapId(nodeCount == 2);
    }
}

bool
GraphicScene::edgeToOtherEnvExists(NodeItem* targetNode)
{
    for (QGraphicsItem* item : this->items())
    {
        if (item->type() == NodeItem::Type)
        {
            NodeItem* node = qgraphicsitem_cast<NodeItem*>(item);
            if (node->getComponentType() == ComponentType::environmentNode
                && node->edgeExists(targetNode))
            {
                return true;
            }
        }
    }
    return false;
}

QString
GraphicScene::getOutfileName()
{
    return mOutFileName;
}

void
GraphicScene::getSortedSceneItems(std::vector<NodeItem*>& nodes, std::vector<NodeItem*>& envNodes, 
        std::vector<EdgeItem*>& edges)
{
    for (QGraphicsItem* item : this->items())
    {
        if (item->type() == NodeItem::Type)
        {
            NodeItem* nodeItem = qgraphicsitem_cast<NodeItem*>(item);
            switch (nodeItem->getComponentType())
            {
                case ComponentType::criticalNode:
                case ComponentType::normalNode: 
                    nodes.push_back(nodeItem); 
                    break;
                case ComponentType::environmentNode: 
                    envNodes.push_back(nodeItem); 
                    break;
                default: 
                    break;
            }
        }
        else if (item->type() == EdgeItem::Type)
        {
            EdgeItem* edgeItem = qgraphicsitem_cast<EdgeItem*>(item);
            edges.push_back(edgeItem);
        }
    }
}

void
GraphicScene::getModuleNodeItems(std::vector<NodeItem*>& nodes)
{
    for (const auto& item : this->items())
    {
        if (item->type() == NodeItem::Type)
        {
            NodeItem* nodeItem = qgraphicsitem_cast<NodeItem*>(item);
            switch (nodeItem->getComponentType())
            {
                case ComponentType::criticalNode:
                case ComponentType::normalNode: 
                    if (nodeItem->isModule())
                    {
                        nodes.push_back(nodeItem); 
                    }
                    break;
                case ComponentType::environmentNode: 
                default: 
                    break;
            }
        }
    }
}

void
GraphicScene::removeNodeItem(NodeItem* nodeItem)
{
    mCounter->free(nodeItem->getId());
    nodeItem->remove();  // removes all connected edges
    removeItem(nodeItem);
}

void
GraphicScene::removeEdgeItem(EdgeItem* edgeItem)
{
    edgeItem->removeLinks();
    removeItem(edgeItem);
}

void
GraphicScene::removeTextItem(TextItem* textItem)
{
    removeItem(textItem);
    textItem->deleteLater();
}

void
GraphicScene::setMode(Mode mode)
{
    mMode = mode;
    mGraphicsView->enableUserActions((mode == Navigate));
}

NodeItem*
GraphicScene::getNodeItemAt(QPointF point)
{
    for (QGraphicsItem* item : this->items(point))
    {
        if (item->type() == NodeItem::Type)
        {
            return qgraphicsitem_cast<NodeItem*>(item);
        }
    }
    return nullptr;
}

bool
GraphicScene::isItemAt(QPointF point)
{
    return !items(point).empty();
}

bool
GraphicScene::getNodeItemById(unsigned int id, NodeItem** nodeItem, bool silent)
{
    for (QGraphicsItem* item : this->items())
    {
        if (item->type() == NodeItem::Type)
        {
            if (qgraphicsitem_cast<NodeItem*>(item)->getId() == id)
            {
                *nodeItem = qgraphicsitem_cast<NodeItem*>(item);
                if (!silent)
                {
                    ErrorHandler::getInstance().setError(Errors::nodeAlreadyExists(id));
                }
                return true;
            }
        }
    }
    ErrorHandler::getInstance().setError(Errors::noNodeWithId(id));
    return false;
}

bool
GraphicScene::nodeExists(unsigned int id)
{
    NodeItem* dummy;
    for (QGraphicsItem* item : this->items())
    {
        if (item->type() == NodeItem::Type)
        {
            dummy = qgraphicsitem_cast<NodeItem*>(item);
            if (dummy->getId() == id
                && dummy->getComponentType() != ComponentType::environmentNode)
            {
                return true;
            }
        }
    }
    return false;
}

void
GraphicScene::onAreaSelection(const QPolygonF& selectionArea)
{
    auto selectedItems = items(selectionArea);
    if (selectedItems.empty())
    {
        return;
    }
    for (int i = 0; i < GRAPHIC_SCENE_FACTORY()->size(); ++i)
    {
        emit GRAPHIC_SCENE_FACTORY()->at(i)->itemSelected();
    }
    
    int nodeCount = 0;
    for (auto item : selectedItems)
    {
        item->setSelected(true);
        
        if (item->type() == NodeItem::Type)
        {
            nodeCount++;
        }
    }

    emit enableSwapId(nodeCount == 2);
}
void
GraphicScene::swapNodeId(bool)
{
    auto selected = selectedItems();
    if (selected.size() != 2)
    {
        PRINT_PANIC("Expected two nodes got %d ", selected.size());
    }
    NodeItem *first = nullptr, *second = nullptr;
    for (auto item : selected)
    {
        if (item->type() != NodeItem::Type)
        {
            PRINT_PANIC("Expected item type to be NodeItem::Type got %d ", item->type());
        }
        if (!first)
        {
            first = qgraphicsitem_cast<NodeItem*>(item);
        }
        else
        {
            second = qgraphicsitem_cast<NodeItem*>(item);
        }
    }

    first->swapId(second);
}
void
GraphicScene::swapNodeCriticality(bool)
{
    QList<QGraphicsItem*> selected = selectedItems();
    NodeItem* nodeItem;
    for (QGraphicsItem* item : qAsConst(selected))
    {
        switch (item->type())
        {
            case NodeItem::Type:
            {
                nodeItem = qgraphicsitem_cast<NodeItem*>(item);
                nodeItem->changeCriticality();
                break;
            }
            default: break;
        }
    }
}
bool
GraphicScene::openItemSettings(bool)
{
    // Open item settings for each selected item
    QList<QGraphicsItem*> _selectedItems = selectedItems();
    NodeItem* nodeItem;
    for (QGraphicsItem* item : qAsConst(_selectedItems))
    {
        switch (item->type())
        {
            case NodeItem::Type:
            {
                nodeItem = qgraphicsitem_cast<NodeItem*>(item);
                nodeItem->openSettings();
                break;
            }
            default: break;
        }
    }
    return true;
}

std::unique_ptr<GraphicScene>
GraphicScene::Create(QObject* parent, bool passive_submodule)
{
    std::unique_ptr<GraphicScene> instance(new (std::nothrow)
                                                   GraphicScene(parent, passive_submodule));

    if (instance.get() && instance->init())
        return instance;

    return nullptr;
}

bool
GraphicScene::init()
{
    mGraphicsView.reset(new (std::nothrow) GraphicsView(this));
    mTransformer.reset(new (std::nothrow) graphInternal::Transformer(this));
    mCounter = Counter::Create();

    if (mGraphicsView && mTransformer && mCounter)
    {
        this->setMode(Navigate);

        QObject::connect(mGraphicsView.get(), &GraphicsView::areaSelected, this, 
                &GraphicScene::onAreaSelection);
        setSceneRect(0, 0, 5000, 5000);
        auto h_bar = graphics_view()->horizontalScrollBar();
        auto v_bar = graphics_view()->verticalScrollBar();
        graphics_view()->horizontalScrollBar()->setValue(
                (h_bar->maximum() + h_bar->minimum() + h_bar->pageStep()) / 2);
        graphics_view()->verticalScrollBar()->setValue(
                (v_bar->maximum() + v_bar->minimum() + v_bar->pageStep()) / 2);

        connect(this, &GraphicScene::itemInserted, this, &GraphicScene::onItemInserted);
        connect(this, &GraphicScene::itemSelected, this, &GraphicScene::onItemSelected);
        connect(this, &GraphicScene::itemDeselected, this, &GraphicScene::onItemDeselected);
        connect(this, &GraphicScene::enableSwapId, this, &GraphicScene::onEnableSwapId);

        mFileName = QDir::currentPath().append("/scene_data.xml");
        mOutFileName = QDir::currentPath().append("/scenario.pm");
        
        return true;
    }
    return false;
}

void
GraphicScene::onItemInserted()
{
    has_changed_ = true;
    // DeprecatedSceneStatus::getInstance().setChanged(true);
    // Wird nur gebraucht wenn man den Knopf wieder unchecken will
    // pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    // mScene->setMode(GraphicScene::Mode(pointerTypeGroup->checkedId()));
    // mButtonGroup->button(int(item->getType()))->setChecked(false);
}

void
GraphicScene::onItemSelected()
{
    // Falls Eigenschaften vom ausgewählten Item geändert werden sollen
    setMode(GraphicScene::ModifyItem);
}

void
GraphicScene::onItemDeselected()
{
    widgets::qt_utils::deselectButtons(
            MainWindowButtonsGroupManager::getInstance()->mButtonGroup->buttons());
    widgets::qt_utils::deselectButtons(
            MainWindowButtonsGroupManager::getInstance()->mAnnotationGroup->buttons());

    for (QGraphicsItem* item : selectedItems())
    {
        item->setSelected(false);
    }

    setMode(GraphicScene::Navigate);
}
void
GraphicScene::onEnableSwapId(bool enable)
{
    MainWindowToolButtonsManager::getInstance()->enableNodeSwapIdButton();
}
/*-------------------------------------------*/
// Tool Buttons from MainWindowToolButtonsManager

bool
GraphicScene::exportAs(FileManager::Extension extension, const QString& path)
{
    FileManager fileManager(this);
    fileManager.exportAs(extension, path);
    return true;
}

bool
GraphicScene::load(const QString& path, bool passive)
{
    // DeprecatedSceneStatus::getInstance().setChanged(true);
    has_changed_ = true;
    mFileName = path;

    FileManager fileManager(this);
    if (fileManager.read(mFileName, passive))
    {
        widgets::MainWindowManager::getInstance()->setModelType(Model::getInstance().getType());

        mRedundancy = fileManager.getRedundancyDefinition();

        ErrorHandler::getInstance().clearCollection();
        // set default PRISM file

        QFileInfo fileInfo(mFileName);
        // QFileInfo.absolutePath() ignores hidden directories

        std::string absolutePath;
        std::vector<std::string> out;

        split(&out, mFileName.toStdString(), pathSeparator);
#if IS_WIN
        absolutePath = base::join(out, out.size() - 1, base::pathSeparator);
#else
        absolutePath = pathSeparator + join(out, out.size() - 1, pathSeparator);
#endif
        mOutFileName =
                tr((absolutePath + fileInfo.baseName().toStdString() + tr(".pm").toStdString())
                           .c_str());
    }
    else
    {  // an error occurred
        ErrorHandler::getInstance().show();
        return false;
    }
    return true;
}

bool
GraphicScene::saveAs(const QString& path)
{
    QString tmp = path;
    QFileInfo info(tmp);
    if (info.suffix().isEmpty())
    {
        tmp += ".xml";
    }
    mFileName = tmp;
    FileManager fileManager(this);

    if (fileManager.write(mFileName, QString::fromUtf8(mRedundancy.c_str())))
    {  // successfully stored scene in file with name filename
        MainWindow::getInstance()->mInformationLabel->setText(" Success: Saved Scene Data to XML File: "
                                                      + mFileName);
        mOutFileName = mFileName.split(".", Qt::SkipEmptyParts).at(0);
        mOutFileName += ".pm";
    }
    else
    {
        ErrorHandler::getInstance().show();
    }
    return true;
}

bool
GraphicScene::buildAndStartXPrism()
{
  //  MainWindowActionsManager::getInstance()->mStartVerificationAct->setEnabled(false);
    mPrismMode = true;  // Start PRISM GUI once transformation is performed
    if (hasChanged())
    {  // try to transform once
        MainWindow::getInstance()->mInformationLabel->setText(" Building PRISM Model...");
        MainWindow::getInstance()->disableGuiElements(true);
        qApp->processEvents();

        if (mTransformer->start(mRedundancy, mOutFileName.toStdString()))
        {
            
        }
    }
    else
    {  // No transformation needed
        mTransformer->DeprecatedTransformationFinished(true);
    }
    return false;
}

bool
GraphicScene::transform()
{
   /* if (!save())
    {
        PRINT_ERROR("Failed to save current module %s ", mFileName.toStdString().c_str());
        return false;
    } */
    
    if (!mIsPassiveSubmodule) // todo use something like this to identify the main module?
    {
        MainWindow::getInstance()->mInformationLabel->setText(" Building PRISM Model...");
        MainWindow::getInstance()->disableGuiElements(true);
    }
    return mTransformer->start(mRedundancy, mOutFileName.toStdString());
}

bool
GraphicScene::evaluteErisModule(NodeItem* submoduleNode, eval::ExperimentInterval interval)
{
    PRINT_INFO("Submodule found, will evaluate it...");
    
    QString submodulePath = submoduleNode->getSubmodulePath();
    auto submodule = Create(nullptr, true);
    PRINT_INFO("Loading submodule %s ", submodulePath.toStdString().c_str());
    if (submodule->load(submodulePath, true))
    {
        if (submodule->transform()) //  transform the found submodule, recursive call
        {
            submoduleNode->getIntrusionIndicatorPtr()->clear();
            submoduleNode->getFailureIndicatorPtr()->clear();
            if (eval::Prism::getInstance()->extractSubmoduleFailureRates(
                    submodule->getOutfileName(), interval,
                    submoduleNode->getFailureIndicatorPtr(),
                    submoduleNode->getIntrusionIndicatorPtr()))
            {
                return true;
            }
            else
            {
                PRINT_ERROR("Failed to evaluate submodule %s .. ", submoduleNode->getSubmodulePath());
                return false;
            }
            
        }
        else
        {
            PRINT_ERROR("Failed to transform submodule %s ", submodulePath);
            //ErrorHandler::getInstance().setError(Errors::transformationError());
            return false;
        }
    }
    else
    {
        PRINT_ERROR("Failed to load submodule %s ", submodulePath);
        //ErrorHandler::getInstance().setError(Errors::submoduleFailedToLoad(nodeItem->getSubmodulePath(), nodeItem->getId()));
        return false;
    }
    PRINT_INFO("Successfully evaluated Submodule %s ", submoduleNode->getSubmodulePath().toStdString().c_str());
    //nodeItem->setFailureIndicator(tr(std::to_string(safetyFailure).c_str()));
    //nodeItem->setIntrusionIndicator(tr(std::to_string(securityFailure).c_str()));
    return true;
}

bool
GraphicScene::evaluteSimulationModule(NodeItem* submoduleNode, eval::ExperimentInterval interval)
{
    submoduleNode->getIntrusionIndicatorPtr()->clear();
    submoduleNode->getFailureIndicatorPtr()->clear();
    if (eval::Octave::getInstance()->extractSimulationFailureRates(
            submoduleNode->getSimulationPath(), interval, 
            submoduleNode->getFailureIndicatorPtr(),
            submoduleNode->getIntrusionIndicatorPtr()))
    {
        return true;
    }
    else
    {
        PRINT_ERROR("Failed to simulate submodule %s .. ", submoduleNode->getSubmodulePath());
        return false;
    }
    
    PRINT_INFO("Successfully simulated submodule %s ", submoduleNode->getSubmodulePath().toStdString().c_str());
    //nodeItem->setFailureIndicator(tr(std::to_string(safetyFailure).c_str()));
    //nodeItem->setIntrusionIndicator(tr(std::to_string(securityFailure).c_str()));
    return true;
}

bool
GraphicScene::evaluateExperiment()
{
    mPrismMode = false;

    eval::ExperimentInterval interval = eval::ExperimentInterval();
    EvaluationSettingsDialog::Get()->experimentDocument(
        Prism::getInstance()->experimentDoc, &interval);
    // Check if submodules exist
    std::vector<NodeItem*> submoduleNodes;
    getModuleNodeItems(submoduleNodes);

    for (const auto& moduleNode : submoduleNodes)
    {  
        if (moduleNode->isErisModule())
        {
            this->evaluteErisModule(moduleNode, interval);
        }
        else if (moduleNode->isSimulationModule())
        {
            this->evaluteSimulationModule(moduleNode, interval);
        }
    }

    if (!submoduleNodes.empty())
    {
        for (int i=interval.from; i < interval.to; i+=interval.steps)
        {
            for (const auto& submodule : submoduleNodes)
            {
                // Set intrusion/failure indicator for current time step
                double step = i+interval.steps; // for some reason module step is 0
                submodule->setIntrusionIndicator(submodule->getIntrusionIndicator(step));
                submodule->setFailureIndicator(submodule->getFailureIndicator(step));
                PRINT_INFO("set failure rate %s", submodule->getFailureIndicator().toStdString().c_str());
                PRINT_INFO("set intrusion rate %s", submodule->getIntrusionIndicator().toStdString().c_str());
            }
            // Evaluate top module for current time step
            transform();
            if (!Prism::getInstance()->isRunning())
            {
                Prism::getInstance()->reset();
                auto currentInterval = Prism::getInstance()->mExperimentInterval.get();
                currentInterval->from = i;
                currentInterval->to = i+interval.steps;
                currentInterval->steps = interval.steps;

                Prism::getInstance()->addArgument(mOutFileName);
                if (!Prism::getInstance()->execute(true, true, (currentInterval->to>=interval.to)))
                {
                    ErrorHandler::getInstance().setError(Errors::programExecutionError("prism"));
                    ErrorHandler::getInstance().show();
                }

                while (Prism::getInstance()->isRunning())
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));

                    qApp->processEvents();
                }
            }
        }
    }
    else
    {
        if (hasChanged())
        {
            transform();
            mTransformer->DeprecatedTransformationFinished(true);
        }

        MainWindow::getInstance()->mInformationLabel->setText(" Running PRISM Experiment...");
        // Experiment Code (PRISM in background) goes here!
        if (!Prism::getInstance()->isRunning())
        {
            // NOTE : Always reset prism command line
            Prism::getInstance()->reset();

            EvaluationSettingsDialog::Get()->experimentDocument(Prism::getInstance()->experimentDoc,
                                                                Prism::getInstance()->mExperimentInterval.get());

            Prism::getInstance()->addArgument(mOutFileName);

            if (!Prism::getInstance()->execute(true))
            {
                ErrorHandler::getInstance().setError(Errors::programExecutionError("prism"));
                ErrorHandler::getInstance().show();
            }
        }
    }

    return true;
}

/*bool
GraphicScene::stopTransformer()
{
    mTransformer->stop();
    return true;
}*/

bool
GraphicScene::optionAddRedundancy()
{
    bool ok = false;
    QString displayRedundancy = QString::fromUtf8(mRedundancy.c_str());
    std::string input =
            QInputDialog::getText(
                    nullptr, "Input dialog", "Text:", QLineEdit::Normal, displayRedundancy, &ok)
                    .toStdString();
    if (ok)
    {
        // Check whether input is correct
        if (!input.empty())
        {
            if (isValidRedundancyDefinition(input, this))
            {
                mRedundancy = input;
            }
        }
        else
        {  // empty redundancy!
            mRedundancy.clear();
        }
    }
    // else cancel was pressed
    return true;
}

bool
GraphicScene::setOutfileName()
{
    QString tmp = QFileDialog::getSaveFileName(MainWindow::getInstance(),
                                               tr("Set/Choose PRISM Output File"),
                                               "",
                                               tr("Graphic Scene (*.pm);;All Files (*)"));
    if (!tmp.isNull())
    {
        mOutFileName = tmp;
        return true;
    }
    return false;
}

bool
GraphicScene::save()
{
    FileManager fileManager(this);

    if (fileManager.write(mFileName, QString::fromUtf8(mRedundancy.c_str())))
    {  // successfully stored scene in file with name filename
        MainWindow::getInstance()->mInformationLabel->setText(" Success: Saved Scene Data to XML File: "
                                                      + mFileName);
        mOutFileName = mFileName.split(".", Qt::SkipEmptyParts).at(0);
        mOutFileName += ".pm";
    }
    else
    {
        ErrorHandler::getInstance().show();
        return false;
    }
    return true;
}

bool
GraphicScene::deleteItem()
{
    QList<QGraphicsItem*> selectedItems = this->selectedItems();
    EdgeItem* edgeItem;
    NodeItem* nodeItem;
    TextItem* textItem;
    bool change = false;
    
    for (QGraphicsItem* item : qAsConst(selectedItems))
    {
        switch (item->type())
        {
            case EdgeItem::Type:
                edgeItem = qgraphicsitem_cast<EdgeItem*>(item);
                this->removeEdgeItem(edgeItem);
                change = true;
                break;
        }
    }
    selectedItems = this->selectedItems();
    for (QGraphicsItem* item : qAsConst(selectedItems))
    {
        switch (item->type())
        {
            case NodeItem::Type:
                nodeItem = qgraphicsitem_cast<NodeItem*>(item);
                this->removeNodeItem(nodeItem);  // node will be deleted
                change = true;
                break;
            case TextItem::Type:
                textItem = qgraphicsitem_cast<TextItem*>(item);
                this->removeTextItem(textItem);
                change = true;
                break;
            default:;
        }
    } 
    
    if (change)
    {
        // DeprecatedSceneStatus::getInstance().setChanged(true);
        has_changed_ = true;
        this->clearSelection();
    }
    return true;
}
bool
GraphicScene::setNodeMode(int id)
{
    const QList<QAbstractButton*> buttons =
            MainWindowButtonsGroupManager::getInstance()->mButtonGroup->buttons();
    const QList<QAbstractButton*> otherButtons =
            MainWindowButtonsGroupManager::getInstance()->mAnnotationGroup->buttons();
    QAbstractButton* current = MainWindowButtonsGroupManager::getInstance()->mButtonGroup->button(id);

    if (!current->isChecked())
    {  // previously checked button was clicked again to be unchecked, just set a neutral mode
        if (selectedItems().empty())
        {
            setMode(GraphicScene::Navigate);
        }
        else
        {
            setMode(GraphicScene::ModifyItem);
        }
    }
    else
    {  // In all other cases uncheck all buttons that are not current button and set the mode
        for (QAbstractButton* button : buttons)
        {
            if (current != button)
            {
                button->setChecked(false);
            }
        }
        widgets::qt_utils::deselectButtons(otherButtons);
        switch (id)
        {
            case ComponentType::normalNode: 
                setMode(GraphicScene::InsertNormalNode); 
                break;              
            case ComponentType::criticalNode: 
                setMode(GraphicScene::InsertCriticalNode); 
                break;
            case ComponentType::environmentNode:
                setMode(GraphicScene::InsertEnvironmentNode);
                break;
            case ComponentType::reachEdge: 
                setMode(GraphicScene::InsertReachEdge); 
                break;
            case ComponentType::functionalEdge:
                setMode(GraphicScene::InsertFunctionalEdge);
                break;
            case ComponentType::securityEdge: 
                setMode(GraphicScene::InsertSecurityEdge); 
                break;
            default: 
                break;
        }
    }
    return true;
}

bool
GraphicScene::setAnnotationMode(int id)
{
    const QList<QAbstractButton*> buttons =
            MainWindowButtonsGroupManager::getInstance()->mAnnotationGroup->buttons();
    const QList<QAbstractButton*> otherButtons =
            MainWindowButtonsGroupManager::getInstance()->mButtonGroup->buttons();
    QAbstractButton* current = MainWindowButtonsGroupManager::getInstance()->mAnnotationGroup->button(id);

    if (!current->isChecked())
    {  // previously checked button was clicked again to be unchecked, just set a neutral mode
        setMode(GraphicScene::ModifyItem);
    }
    else
    {  // In all other cases uncheck all buttons that are not current button and set the mode
        for (QAbstractButton* button : buttons)
        {
            if (current != button)
            {
                button->setChecked(false);
            }
        }
        widgets::qt_utils::deselectButtons(otherButtons);
        // Currently support only one button in button group, this has to be changed if more buttons
        // are added
        setMode(GraphicScene::InsertText);
    }
    return true;
}

void
GraphicScene::setRateInterpretation(graph::RateInterpretation rateInterpretation)
{
    mActiveRateInterpretation = rateInterpretation;
    MainWindowActionsManager::getInstance()->toggleRateInterpretation(rateInterpretation);
}

void
GraphicScene::updateRateInterpretation(graph::RateInterpretation newRateInterpretation)
{
    if (mActiveRateInterpretation != newRateInterpretation)
    {// Interpretation has changed, attempt to change rates for all existing nodes
        PRINT_INFO("Rate Interpretation was changed by user. Attempting to change rates of all nodes of the active scene.");
        
        double factor = graph::RateInterpretation::RateInterpretationFactor(mActiveRateInterpretation, newRateInterpretation);
        
        for (auto item : items())
        {// Todo maybe check if the reinterpretation works before actually doing it
            if (item->type() == NodeItem::Type)
            {
                NodeItem* nodeItem = qgraphicsitem_cast<NodeItem*>(item);
                double tmpIndicator = nodeItem->getFailureIndicator().toDouble() * factor;
                nodeItem->setFailureIndicator(QString::number(tmpIndicator, 'g', 15));
                tmpIndicator = nodeItem->getIntrusionIndicator().toDouble() * factor;
                nodeItem->setIntrusionIndicator(QString::number(tmpIndicator, 'g', 15));
                tmpIndicator = nodeItem->getSecurityIndicator().toDouble() * factor;
                nodeItem->setSecurityIndicator(QString::number(tmpIndicator, 'g', 15));
                tmpIndicator = nodeItem->getDefectRecoveryIndicator().toDouble() * factor;
                nodeItem->setDefectRecoveryIndicator(QString::number(tmpIndicator, 'g', 15));
                tmpIndicator = nodeItem->getCorruptionRecoveryIndicator().toDouble() * factor;
                nodeItem->setCorruptionRecoveryIndicator(QString::number(tmpIndicator, 'g', 15));
            }
        }
        mActiveRateInterpretation = newRateInterpretation;
    }
}

graph::RateInterpretation
GraphicScene::getCurrentRateInterpretation()
{
    return mActiveRateInterpretation;
}
    

bool
GraphicScene::addSubmodule(const QString& submodule_path, NodeItem* node)
{
    ERIS_CHECK(!submodule_path.isEmpty());
    ERIS_CHECK(node);

    if (!QFile::exists(submodule_path))
    {
        PRINT_ERROR("Cannot add submodule. Path does not exists %s ",
                    submodule_path.toStdString().c_str());
        return false;
    }

    if (mSubmodules.find(submodule_path) != mSubmodules.end())
    {
        PRINT_WARNING("Submodule %s already exist in this module %s ",
                      submodule_path.toStdString().c_str(),
                      path().toStdString().c_str());
        return true;
    }

    mSubmodules[submodule_path] = node;

    return true;
}

bool
GraphicScene::removeSubmodule(const QString& submodule_path, NodeItem* node)
{
    ERIS_CHECK(node);
    ERIS_CHECK(!submodule_path.isEmpty());

    if (mSubmodules.find(submodule_path) == mSubmodules.end())
    {
        PRINT_WARNING("This module %s is not a submodule of the current module %s ",
                    submodule_path.toStdString().c_str(),
                    path().toStdString().c_str());
        PRINT_WARNING("Given node id = %lu ", node->getId());
        return false;
    }

    ERIS_CHECK(mSubmodules[submodule_path] == node);
    ERIS_CHECK(mSubmodules.erase(submodule_path) == 1);

    return true;
}


/*-------------------------------------------*/

}  // namespace graph
