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

#include "node_item.h"

#include "edge_item.h"
#include "list.h"
#include "node_settings.h"
#include "counter.h"
#include "error_handler.h"
#include "transcriber.h"
#include "logger.h"
#include "checks.h"
#include "main_window_manager.h"
#include "graphic_scene.h"

#include <algorithm>

#include <QGraphicsScene>

#include <QGraphicsTextItem>
#include <QRectF>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneHoverEvent>

#include <QMenu>
#include <QAction>

namespace graph
{
using widgets::ErrorHandler;
using widgets::Errors;

NodeItem::NodeItem(ComponentType type, QGraphicsEllipseItem* parent) :
    QGraphicsEllipseItem(parent), mType(type), mObserver(this), 
    mIntrusionIndicators(nullptr), mFailureIndicators(nullptr)
{
    init();
}

NodeItem::NodeItem(ComponentType type,
                   unsigned int id,
                   QPointF position,
                   QGraphicsEllipseItem* parent,
                   GraphicScene* maybe_graph) :
    QGraphicsEllipseItem(parent),
    mType(type),
    mId(id),
    mCritical(false),
    mRecoverableCorruption(false),
    mRecoverableDefect(false),
    mIsSimulationModule(false),
    mIsErisModule(false),
    mNodeSettingsActive(false),
    mIntrusionIndicator("0"),
    mIntrusionIndicators(new std::map<qreal, QString>()),
    mFailureIndicators(new std::map<qreal, QString>()),
    mFailureIndicator("0"),
    mSecurityIndicator("0"),
    mDefectRecoveryIndicator("0"),
    mCorruptionRecoveryIndicator("0"),
    mSimulationPath(""),
    mCorruptionRecoveryStrategy(Recovery::Strategy::restricted),
    mDefectRecoveryStrategy(Recovery::Strategy::restricted),
    mEssentialNodes(""),
    mObserver(this),
    mScene(maybe_graph)
{
    init();
    mPreviousPosition = position;
    this->setPos(position);
}

NodeItem::~NodeItem()
{
    if (!mSceneIsInvalid)
    {
        if (!mSubmodulePath.isEmpty())
        {
            ERIS_CHECK(getCurrentGraphicScene()->removeSubmodule(mSubmodulePath, this));
        }
    }
    
    delete this->mIntrusionIndicators;
    delete this->mFailureIndicators;
}

void
NodeItem::init()
{
    mContextMenu.reset(new (std::nothrow) QMenu());
    mInspectAct.reset(new (std::nothrow) QAction("Inspect"));
    ERIS_CHECK(mContextMenu.get());
    ERIS_CHECK(mInspectAct.get());

    QObject::connect(mInspectAct.get(),
                     &QAction::triggered,
                     &mObserver,
                     &InspectActionObserver::actionTriggered);

    mContextMenu->addAction(mInspectAct.get());

    mNodeText = new QGraphicsTextItem(this);
    QFont font;
    font.setPointSize(15);
    mNodeText->setFont(font);
    mCriticalRed.setRgb(222, 135, 135); //222, 135, 135)

    mRect = QRect(0.0, 0.0, ObjectSize, ObjectSize);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    this->setPen(QPen(Qt::black, 2));

    initBrush();
    setToolTip(mNodeText->toPlainText());
}

void
NodeItem::initBrush()
{
    if (mType == ComponentType::normalNode)
    {
        mCritical = false;
        this->setBrush(Qt::white);
        mTextColor = Qt::black;
        mNodeText->setPlainText("n" + QString::number(mId));
        mNodeText->setDefaultTextColor(mTextColor);
    }
    else if (mType == ComponentType::criticalNode)
    {
        mCritical = true;
        this->setBrush(mCriticalRed);
        mTextColor = Qt::black;
        mNodeText->setPlainText("n" + QString::number(mId));
        mNodeText->setDefaultTextColor(mTextColor);
    }
    else if (mType == ComponentType::environmentNode)
    {
        this->setBrush(Qt::black);
        mTextColor = Qt::white;
        mNodeText->setPlainText("Env");
        mNodeText->setDefaultTextColor(mTextColor);
    }
    this->setRect(mRect);
    updateTextItemPosition();
    mOriginalPen = pen();
}

QRectF
NodeItem::boundingRect() const
{
    return QRectF(0, 0, NodeItem::ObjectSize, NodeItem::ObjectSize);
}

void
NodeItem::updateTextItemPosition()
{
    QPointF point = QPointF(mRect.x() + boundingRect().width() / 2,
                            mRect.y() + boundingRect().height() / 2);
    mNodeText->setPos(point
                      - QPointF(mNodeText->boundingRect().width() / 2,
                                mNodeText->boundingRect().height() / 2));
}

void
NodeItem::addEdgeItem(EdgeItem* edgeItem)
{
    if (edgeItem->startItem() == this)
    {  // Item does only need one direction
        mOutgoingEdgeItems.push_back(edgeItem);
    }
    else if (edgeItem->endItem() == this)
    {
        mIncomingEdgeItems.push_back(edgeItem);
    }
}

void
NodeItem::removeEdgeItem(EdgeItem* edgeItem)
{
    if (edgeItem->startItem() == this)
    {
        mOutgoingEdgeItems.remove(edgeItem);
    }
    else
    {
        mIncomingEdgeItems.remove(edgeItem);
    }
}

void
NodeItem::remove()
{
    removeAllEdgeItems();
}

void
NodeItem::removeAllEdgeItems()
{
    EdgeItem* curr;
    while (!mOutgoingEdgeItems.empty())
    {
        curr = mOutgoingEdgeItems.front();
        curr->endItem()->removeEdgeItem(curr);  // call end nodeitem to remove this edge

        mOutgoingEdgeItems.remove(curr);
        scene()->removeItem(curr);
    }

    while (!mIncomingEdgeItems.empty())
    {
        curr = mIncomingEdgeItems.front();
        curr->startItem()->removeEdgeItem(curr);  // call end nodeitem to remove this edge

        mIncomingEdgeItems.remove(curr);
        scene()->removeItem(curr);
    }
}

ComponentType
NodeItem::getComponentType()
{
    return mType;
}

bool
NodeItem::isCritical()
{
    return mCritical;
}

void
NodeItem::recoverableFromDefect(bool recoverable)
{
    mRecoverableDefect = recoverable;
}

bool
NodeItem::isRecoverableFromDefect()
{
    return mRecoverableDefect;
}

void
NodeItem::recoverableFromCorruption(bool recoverable)
{
    mRecoverableCorruption = recoverable;
}

bool
NodeItem::isRecoverableFromCorruption()
{
    return mRecoverableCorruption;
}

void
NodeItem::setSimulated(bool simulated, const QString& pathToSimulation)
{
    if (simulated)
    {
        if (pathToSimulation != mSimulationPath) 
        {
            PRINT_INFO("New Simulation model given, overwriting previous");
            mSimulationPath = pathToSimulation; // store path
        }
        mIsSimulationModule = simulated;

        // set pen to indicate node is simulated
        setPen(QPen(Qt::black, 2, Qt::DashDotLine));
        setToolTip("Simulation : " + mSimulationPath);
    }
    else
    {
        if (!pathToSimulation.isEmpty())
        {
            PRINT_INFO("New Simulation model given but turned off. Overwriting previous path");
            mSimulationPath = pathToSimulation;
        }
        mIsSimulationModule = simulated;

        // set pen to indicate normal node mode
        // path is empty, cant be a submodule even though the flag is set
        if (!this->isErisModule())
        {
            setPen(mOriginalPen);
            setToolTip(mNodeText->toPlainText()); 
        }          
    }
}

QString
NodeItem::getSimulationPath()
{
    return mSimulationPath;
}

bool
NodeItem::isSimulationModule()
{
    return mIsSimulationModule;
}

unsigned int
NodeItem::getId()
{
    return mId;
}

void
NodeItem::setId(unsigned int id)
{
    mId = id;
    mNodeText->setPlainText("n" + QString::number(mId));
    updateTextItemPosition();
}

int
NodeItem::type() const
{
    return NodeItem::Type;
}

bool
NodeItem::edgeExists(NodeItem* nodeItem)
{
    for (EdgeItem* item : mOutgoingEdgeItems)
    {
        if (item->endItem() == nodeItem)
        {
            return true;
        }
    }
    return false;
}

bool
NodeItem::edgeWithTypeExists(NodeItem* nodeItem, ComponentType edgeType)
{
    for (EdgeItem* item : mOutgoingEdgeItems)
    {
        if (item->endItem() == nodeItem && item->getComponentType() == edgeType)
        {
            ErrorHandler::getInstance().setError(
                    Errors::edgeAlreadyExists(nodeItem->getId(), graph::componentTypeAsString(edgeType)));
            return true;
        }
    }
    return false;
}

std::vector<EdgeItem*>
NodeItem::getCollidingEdgeItems(NodeItem* nodeItem)
{
    std::vector<EdgeItem*> collidingEdges;
    for (EdgeItem* item : mOutgoingEdgeItems)
    {
        if (item->endItem() == nodeItem)
        {
            collidingEdges.push_back(item);
        }
    }
    for (EdgeItem* item : mIncomingEdgeItems)
    {
        if (item->startItem() == nodeItem)
        {
            collidingEdges.push_back(item);
        }
    }
    return collidingEdges;
}

QString
NodeItem::getEssentialNodes()
{
    return mEssentialNodes;
}

void
NodeItem::setEssentialNodes(QString essentialNodes)
{
    mEssentialNodes = essentialNodes;
}

QString
NodeItem::getIntrusionIndicator()
{
    return mIntrusionIndicator;
}

QString
NodeItem::getIntrusionIndicator(qreal t)
{
    QString indicator = mIntrusionIndicators->at(t);
    return indicator;
}

std::map<qreal, QString>*
NodeItem::getIntrusionIndicatorPtr()
{
    return mIntrusionIndicators;
}

void
NodeItem::setIntrusionIndicator(QString value)
{
    mIntrusionIndicator = value;
}

void
NodeItem::setIntrusionIndicators(std::map<qreal, QString>* indicators)
{
    mIntrusionIndicators = indicators;
}

QString
NodeItem::getFailureIndicator()
{
    return mFailureIndicator;
}

QString
NodeItem::getFailureIndicator(qreal t)
{
    QString indicator = mFailureIndicators->at(t);
    return indicator;
}

std::map<qreal, QString>*
NodeItem::getFailureIndicatorPtr()
{
    return mFailureIndicators;
}

void
NodeItem::setFailureIndicator(QString value)
{
    mFailureIndicator = value;
}

void
NodeItem::setFailureIndicators(std::map<qreal, QString>* indicators)
{
    mFailureIndicators = indicators;
}

QString
NodeItem::getSecurityIndicator()
{
    return mSecurityIndicator;
}

void
NodeItem::setSecurityIndicator(QString value)
{
    mSecurityIndicator = value;
}

QString
NodeItem::getDefectRecoveryIndicator()
{
    return mDefectRecoveryIndicator;
}

void
NodeItem::setDefectRecoveryIndicator(QString value)
{
    mDefectRecoveryIndicator = value;
}

QString
NodeItem::getCorruptionRecoveryIndicator()
{
    return mCorruptionRecoveryIndicator;
}

void
NodeItem::setCorruptionRecoveryIndicator(QString value)
{
    mCorruptionRecoveryIndicator = value;
}

void
NodeItem::setCorruptionRecoveryStrategy(Recovery::Strategy strategy)
{
    mCorruptionRecoveryStrategy = strategy;
}

Recovery::Strategy
NodeItem::getCorruptionRecoveryStrategy()
{
    return mCorruptionRecoveryStrategy;
}

void
NodeItem::setDefectRecoveryStrategy(Recovery::Strategy strategy)
{
    mDefectRecoveryStrategy = strategy;
}

Recovery::Strategy
NodeItem::getDefectRecoveryStrategy()
{
    return mDefectRecoveryStrategy;
}

bool
NodeItem::providesFunctionality(unsigned int id)
{
    for (EdgeItem* edgeItem : mIncomingEdgeItems)
    {
        if (edgeItem->startItem()->getId() == id)
        {
            return true;
        }
    }
    return false;
}

void
NodeItem::setCustomCorruptionRecoveryFormula(QString formula)
{
    mCorruptionRecoveryFormula = std::move(formula);
}

QString
NodeItem::getCustomCorruptionRecoveryFormula()
{
    return mCorruptionRecoveryFormula;
}

void
NodeItem::setCustomDefectRecoveryFormula(QString formula)
{
    mDefectRecoveryFormula = formula;
}

QString
NodeItem::getCustomDefectRecoveryFormula()
{
    return mDefectRecoveryFormula;
}

void
NodeItem::changeCriticality()
{
    switch (mType)
    {
        case ComponentType::criticalNode: 
            mType = ComponentType::normalNode; 
            break;
        case ComponentType::normalNode: 
            mType = ComponentType::criticalNode;
            break;
        case ComponentType::environmentNode:
        default: 
            return;
    }

    initBrush();

    getCurrentGraphicScene()->setStateChanged(true);

    // DeprecatedSceneStatus::getInstance().setChanged(true);
}

void
NodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    //painter->setRenderHint(QPainter::HighQualityAntialiasing);
    QGraphicsEllipseItem::paint(painter, option, widget);
}

void
NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{  // check if collides, if not, update position !

    bool collidesWithNode = false;
    for (QGraphicsItem* item : scene()->collidingItems(this))
    {
        if (item->type() == NodeItem::Type)
        {
            collidesWithNode = true;
            break;
        }
    }
    if (collidesWithNode)
    {
        ErrorHandler::getInstance().setError(Errors::positionAlreadyOccupied());
        ErrorHandler::getInstance().show();
        setPos(mPreviousPosition);
    }
    else
    {  // position is good, update previous position to current
        mPreviousPosition = pos();
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void
NodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    openSettings();
    QGraphicsItem::mouseDoubleClickEvent(event);
}

QPixmap
NodeItem::image() const
{
    QPixmap pixmap(220, 220);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QRectF iconEllipse = QRect(0.0, 0.0, 200.0, 200.0);

    if (mType == ComponentType::normalNode)
    {
        painter.setBrush(Qt::white);
        painter.setPen(QPen(Qt::black, 8));
        painter.translate(12, 12);
        painter.drawEllipse(iconEllipse);
    }
    else if (mType == ComponentType::criticalNode)
    {
        painter.setBrush(mCriticalRed);
        painter.setPen(QPen(Qt::black, 8));
        painter.translate(12, 12);
        painter.drawEllipse(iconEllipse);
    }
    else if (mType == ComponentType::environmentNode)
    {
        painter.setBrush(Qt::black);
        painter.setPen(QPen(Qt::black, 8));
        painter.translate(12, 12);
        painter.drawEllipse(iconEllipse);
    }
    return pixmap;
}

QVariant
NodeItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == QGraphicsItem::ItemPositionChange)
    {
        for (EdgeItem* item : qAsConst(mOutgoingEdgeItems))
        {
            item->updatePosition();
        }
        for (EdgeItem* item : qAsConst(mIncomingEdgeItems))
        {
            item->updatePosition();
        }
    }
    return value;
}

void
NodeItem::nodeSettingsDeactivated()
{
    mNodeSettingsActive = false;
}

void
NodeItem::openSettings()
{
    if (!mNodeSettingsActive)
    {
        mSettings = new NodeSettings(this);
        mSettings->openWindow();
        mNodeSettingsActive = true;
    }
    mSettings->raise();
}

void
NodeItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsEllipseItem::mousePressEvent(event);
}

void
NodeItem::swapId(NodeItem* other)
{
    std::list<ComponentType> valid_types = {ComponentType::normalNode,
                                              ComponentType::criticalNode};

    bool this_valid = utils::isInList<ComponentType>(getComponentType(), valid_types);
    bool other_valid = utils::isInList<ComponentType>(other->getComponentType(), valid_types);

    if (!(this_valid && other_valid))
    {
        ErrorHandler::getInstance().setError(Errors::invalidSwapNodeId());
        ErrorHandler::getInstance().show();
        return;
    }

    unsigned int old_id = mId;
    setId(other->getId());
    other->setId(old_id);
}

void
NodeItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    if (!mSubmodulePath.isEmpty())
    {
        mContextMenu->popup(event->screenPos());
        QGraphicsItem::contextMenuEvent(event);
    }
}

void
NodeItem::inspect(bool /*checked*/)
{
    if (!mSubmodulePath.isEmpty())
    {
        widgets::MainWindowManager::getInstance()->openSubmodule(mSubmodulePath.toStdString());
    }
}

GraphicScene*
NodeItem::getCurrentGraphicScene() const
{
    GraphicScene* scenePtr = dynamic_cast<GraphicScene*>(scene());
    if (!scenePtr)
    {
        scenePtr = mScene;
    }
    ERIS_CHECK(scenePtr);
    return scenePtr;
}

void
NodeItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    PRINT_INFO("Hover Entered");
    QGraphicsItem::hoverMoveEvent(event);
}

void
NodeItem::setSubmodule(bool submodule, const QString& pathToSubmodule)
{
    if (submodule)
    {
        if (pathToSubmodule != mSubmodulePath) 
        {
            if (!mSubmodulePath.isEmpty())
            {// old path not empty
                PRINT_INFO("New Submodule given, attempting to remove old submodule");
                ERIS_CHECK(getCurrentGraphicScene()->removeSubmodule(mSubmodulePath, this));
                widgets::MainWindowManager::getInstance()->closeSubmodule(mSubmodulePath.toStdString());
            }
            if (!pathToSubmodule.isEmpty()) // should never happen
            {// new path not empty, load module
                ERIS_CHECK(getCurrentGraphicScene()->addSubmodule(pathToSubmodule, this));
            }
            mSubmodulePath = pathToSubmodule; // store path
        }
        mIsErisModule = submodule;
        // set pen to indicate node is submodule
        setPen(QPen(Qt::black, 2, Qt::DotLine));
        setToolTip("Submodule : " + mSubmodulePath);    
    }
    else
    {
        if (!mSubmodulePath.isEmpty())
        {// old path not empty
            //TODO check whether the submodule has already been removed
           PRINT_INFO("Empty submodule path given, attempting to remove old submodule");
           if (getCurrentGraphicScene()->removeSubmodule(mSubmodulePath, this))
           {
                widgets::MainWindowManager::getInstance()->closeSubmodule(mSubmodulePath.toStdString());
           }
        }
        if (pathToSubmodule.isEmpty())
        {
            mSubmodulePath = pathToSubmodule;
        }
        mIsErisModule = submodule;
        // set pen to indicate normal node mode
        // path is empty, cant be a submodule even though the flag is set
        if (!this->isSimulationModule())
        {
            setPen(mOriginalPen);
            setToolTip(mNodeText->toPlainText());
        }        
    }
}

QString
NodeItem::getSubmodulePath()
{
    return mSubmodulePath;
}

bool
NodeItem::isErisModule()
{
    return mIsErisModule;
}

bool
NodeItem::isModule()
{
    return mIsErisModule || mIsSimulationModule;
}


InspectActionObserver::InspectActionObserver(NodeItem* node) : node_item_(node)
{
}
InspectActionObserver::~InspectActionObserver() = default;
void
InspectActionObserver::actionTriggered(bool checked)
{
    node_item_->inspect(checked);
}
}  // namespace graph
