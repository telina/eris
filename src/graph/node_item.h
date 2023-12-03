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

#ifndef ERIS_GRAPH_NODE_ITEM_H
#define ERIS_GRAPH_NODE_ITEM_H

#include "component_type.h"
#include "recovery_strategy.h"
#include "eris_config.h"

#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QPen>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QGraphicsTextItem;
class QRectF;
class QMenu;
class QAction;
QT_END_NAMESPACE

#include <regex>
#include <set>

namespace widgets
{
class NodeSettings;
}
namespace graph
{
using widgets::NodeSettings;
class GraphicScene;
class Counter;
class Transcriber;
class EdgeItem;
class NodeItem;

// Since NodeItem cannot inherit from QObject, we cannot
// connect any signals to it. This class provides a workaround
// to forward inspect action when triggered.
class ERIS_EXPORT InspectActionObserver : public QObject
{
    Q_OBJECT
public:
    explicit InspectActionObserver(NodeItem*);
    InspectActionObserver() = delete;
    ~InspectActionObserver() override;

public slots:
    void
    actionTriggered(bool);

private:
    NodeItem* node_item_;
};
/**
 * Class representing the visual representation in the GUI of a node.
 */
class ERIS_EXPORT NodeItem : public QGraphicsEllipseItem
{
    friend class InspectActionObserver;

public:
    /**
     * Creates a new graphics item for the associated node.
     *
     * @param type, type of node (component)
     * @param node the associated node object
     * @param parent QT parent
     */
    NodeItem(graph::ComponentType type,
             unsigned int id,
             QPointF position,
             QGraphicsEllipseItem* parent = nullptr,
             GraphicScene* maybe_graph = nullptr);

    /**
     * Creates a dummy node item, without an associated node, used for initializing the node button.
     * This constructor can be used whenever only the image of the node is relevant, and not its
     * functionality.
     * @param type, type of node (component)
     * @param parent QT parent
     */
    explicit NodeItem(graph::ComponentType type, QGraphicsEllipseItem* parent = nullptr);

    ~NodeItem() override;

    /** Determines what kind of item this GraphicsItem is (Node or Edge) */
    static int const Type = UserType + 1;

    static constexpr double const ObjectSize = 50;

    static constexpr double const Radius = ObjectSize / 2;

    /**
     * Initialises the node item
     */
    void
    init();

    /**
     * Initialises the brush settings.
     */
    void
    initBrush();

    /**
     * Adds an Edge to the Node, i.e. inserts it in the list.
     * @param edge, the edge that shall be added
     */
    void
    addEdgeItem(EdgeItem* edgeItem);

    /**
     * Removes the given Edge from the node (list).
     * @param edge, the edge that shall be removed
     */
    void
    removeEdgeItem(EdgeItem* edgeItem);

    /**
     * Removes all edeges of the node, hence clears the list. This is mainly
     * called before removing the node.
     */
    void
    removeAllEdgeItems();

    /**
     * Prepares to delete the node, by removing all edges of the associated node, as well as the
     * nodeitem itself.
     */
    void
    remove();

    /**
     * Returns the Rect of the node.
     */
    QRectF
    rectangle() const
    {
        return mRect;
    }

    /**
     * Returns the precise type of the node.
     */
    graph::ComponentType
    getComponentType();

    /**
     *  Returns the kind of type of the Graphics Item, in this case, NodeType.
     */
    int
    type() const override;

    /**
     * Get the bounding rect of the node item.
     * @return bounding rect
     */
    QRectF
    boundingRect() const override;

    /**
     * Draw the node image and returns it.
     */
    QPixmap
    image() const;

    /**
     * Determines whether an edge exists from this node to the the input
     * node.
     * @param node the input node
     * @return true if edge exists, false otherwise
     */
    bool
    edgeExists(NodeItem* node);

    /**
     * Determines whether an edge with the provided type exists between the current node
     * and the input node.
     * @param node the input node
     * @return true if edge exists, false otherwise
     */
    bool
    edgeWithTypeExists(NodeItem* node, graph::ComponentType edgeType);

    /**
     * Returns the list of input and output edges between this node item and the input node item.
     * These edge items potentially collide in the GUI and may have to be repainted/repositioned.
     * @param node input node
     * @return list of colliding edges
     */
    std::vector<EdgeItem*>
    getCollidingEdgeItems(NodeItem* nodeItem);

    /**
     * Indicates whether the node is critical or not.
     * @return true of critical, false otherwise
     */
    bool
    isCritical();

    /**
     * Sets the node recoverable from defects or not.
     */
    void
    recoverableFromDefect(bool recoverable);

    /**
     * Indicates whether the node is recoverable from a previous defect or not.
     * @return true if recoverable, false otherwise
     */
    bool
    isRecoverableFromDefect();

    /**
     * Sets the node recoverable from corruptions or not.
     */
    void
    recoverableFromCorruption(bool recoverable);

    /**
     * Indicates whether the node is recoverable from a previous corruption or not.
     * @return true if recoverable, false otherwise
     */
    bool
    isRecoverableFromCorruption();

    /**
     * Marks the node simulated and sets the path to the simulation model.
     * @param simulated flag
     * @param pathToSimulation path to the simulation model
     */
    void
    setSimulated(bool simulated, const QString& pathToSimulation);

    /**
     * Indicates whether the node is simulated or not.
     * @return
     */
    bool
    isSimulationModule();

    /**
     * Returns the path to the simulation model.
     * @return path to the model as string
     */
    QString
    getSimulationPath();

    /**
     * Returns the number (id) of the node.
     * @return current id
     */
    unsigned int
    getId();

    /**
     * Swap current id with other id
     * */
    void
    swapId(NodeItem* other);

    /**
     * Updates the id of this node in the list of occupied IDs.
     * @param id
     */
    void
    setId(unsigned int id);

    /**
     * Returns the essential nodes of the current node as a string for displaying.
     * @return
     */
    QString
    getEssentialNodes();

    /**
     * Opens a window where the user can change/provide probabilities/rates, node dependencies and
     * the node ID.
     */
    void
    openSettings();

    /**
     * Returns the intrusion indicator (probability/rate) of the node.
     * @return probability/rate
     */
    QString
    getIntrusionIndicator();

    /**
     * Gets intrusion indicator at specified time point t.
     * Note: requires submodule evalutation to be processed.
     */
    QString
    getIntrusionIndicator(qreal t);

    /**
     * Gets pointer to map of intrusion indicators (for submodule
     * evaluation).
     * @return pointer to indicator map
     */
    std::map<qreal, QString>*
    getIntrusionIndicatorPtr();

    /**
     * Sets the intrusion indicator (probability/rate) to the new value.
     * @param probability/rate
     */
    void
    setIntrusionIndicator(QString value);

    void
    setIntrusionIndicators(std::map<qreal, QString>* indicators);

    /**
     * Returns the failure indicator (probability/rate) of the node
     * @return probability/rate
     */
    QString
    getFailureIndicator();

    /**
     * Gets failure indicator at specified time point t.
     * Note: requires submodule evalutation to be processed.
     */
    QString
    getFailureIndicator(qreal t);

    /**
     * Gets pointer to map of failure indicators (for submodule
     * evaluation).
     * @return pointer to indicator map
     */
    std::map<qreal, QString>*
    getFailureIndicatorPtr();

    /**
     * Sets the failure indicator (probability/rate) to the new value.
     * @param probability/rate
     */
    void
    setFailureIndicator(QString value);

    void
    setFailureIndicators(std::map<qreal, QString>* indicators);

    /**
     * Returns the security indicator (probability/rate) of the node
     * @return probability/rate
     */
    QString
    getSecurityIndicator();

    /**
     * Sets the security indicator (probability/rate) to the new value.
     * @param probability/rate
     */
    void
    setDefectRecoveryIndicator(QString value);

    /**
     * Returns the recovery indicator (probability/rate) of the node
     * @return probability/rate
     */
    QString
    getDefectRecoveryIndicator();

    /**
     * Sets the security indicator (probability/rate) to the new value.
     * @param probability/rate
     */
    void
    setCorruptionRecoveryIndicator(QString value);

    /**
     * Returns the recovery indicator (probability/rate) of the node
     * @return probability/rate
     */
    QString
    getCorruptionRecoveryIndicator();

    /**
     * Sets the recovery probability to the new value.
     * @param probability/rate
     */
    void
    setSecurityIndicator(QString value);

    /**
     * Sets the strategy that shall be applied in case the node gets corrupted.
     * @param strategy
     */
    void
    setCorruptionRecoveryStrategy(Recovery::Strategy strategy);

    /**
     * Returns the corruption recovery strategy.
     * @return strategy
     */
    Recovery::Strategy
    getCorruptionRecoveryStrategy();

    /**
     * Sets the strategy that shall be applied in case the node gets corrupted.
     * @param strategy
     */
    void
    setDefectRecoveryStrategy(Recovery::Strategy strategy);

    /**
     * Returns the defect recovery strategy.
     * @return strategy
     */
    Recovery::Strategy
    getDefectRecoveryStrategy();

    /**
     * Checks  whether the input string has the correct format and contains valid nodes (connections
     * to the set nodes actually exist) and if so, updates the nodes essential node to the provided
     * string.
     * @return true if input was valid and the string could be updated successfully
     */
    void
    setEssentialNodes(QString essentialNodes);

    void
    InvalidateScene()
    {
        mSceneIsInvalid = true;
    }

    /**
     * Sets the submodule status of the node and the path to the submodule. A
     * path may be set even if the submodule is deactivated.
     * @param submodule
     * @param pathToSubmodule
     */
    void
    setSubmodule(bool submodule, const QString& pathToSubmodule);

    /**
     * Gets the path to the submodule, empty string when unset.
     * @param path to the submodule
     */    
    QString
    getSubmodulePath();

    /**
     * Indicates whether the given node contains an ERIS module.
     * @return true if submodule, false otherwise
     */
    bool
    isErisModule();
    
    bool
    isModule();

    /**
     * Checks whether the given node (by ID) provides functionality (function edge) to this node.
     * @param id viewed node
     * @return
     */
    bool
    providesFunctionality(unsigned int id);

    /**
     * Sets the custom recovery strategy for the corruption case.
     * @param formula
     */
    void
    setCustomCorruptionRecoveryFormula(QString formula);

    /**
     * Returns the custom corruption recovery strategy.
     * @return strategy as string
     */
    QString
    getCustomCorruptionRecoveryFormula();

    /**
     * Sets the custom recovery strategy for the defect case.
     * @param formula
     */
    void
    setCustomDefectRecoveryFormula(QString formula);

    /**
     * Returns the custom defect recovery strategy.
     * @return strategy as string
     */
    QString
    getCustomDefectRecoveryFormula();

    /**
     * Toggles the criticality of the node between normal (uncritical) to
     * critical and vice versa.
     */
    void
    changeCriticality();

    void
    nodeSettingsDeactivated();

protected:
    void
    contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
    
    /**
     * Updates the position on item change.
     * @param change
     * @param value
     * @return
     */
    QVariant
    itemChange(GraphicsItemChange change, const QVariant& value) override;

    /**
     * Paints the node item in the GUI (or also as a button).
     * @param painter
     * @param option
     * @param widget
     */
    void
    paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override;

    /**
     * Triggered when the mouse button is released; the node is planted in the scene.
     * It is checked, whether the node overlaps with another item. If thats the case an error
     * message is displayed and the node is not moved, hence, set back to its previous position.
     * @param event
     */
    void
    mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    /**
     *
     * @param event
     */
    void
    mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

    void
    mousePressEvent(QGraphicsSceneMouseEvent*) override;
    void
    hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;

private:
    
    void
    inspect(bool checked);
    
    GraphicScene*
    getCurrentGraphicScene() const;

private:
    /**
     * Sets/Updates the position of the text item after text was set or modified
     */
    void
    updateTextItemPosition();

    /** List of outgoing and incoming edge items */
    std::list<EdgeItem*> mIncomingEdgeItems;
    std::list<EdgeItem*> mOutgoingEdgeItems;

    /** Component type of the node (can only be node-type) */
    graph::ComponentType mType;

    /** Rect object of the ellipse */
    QRectF mRect;

    /** Colour of the text (node name) */
    QColor mTextColor;

    /** Number/ID of the node */
    unsigned int mId;

    /** Flag indicating criticality of the node */
    bool mCritical;

    /** Flag indicating that the node may be recovered if it turned defective*/
    bool mRecoverableCorruption;

    /** Flag indicating that the node may be recovered if it turned corrupted*/
    bool mRecoverableDefect;

    /** Flag indicating that the node is outsourced to simulation */
    bool mIsSimulationModule;
    
    /** Flag indicating that the node contains a submodule */
    bool mIsErisModule;
    
    /** Flag indicating whether node settings window is active or not */
    bool mNodeSettingsActive;

    NodeSettings* mSettings;

    /** The probability/rate that the node runs into a security failure*/
    QString mIntrusionIndicator;

    /** List to store submodule evaluated intrusion indicators */
    std::map<qreal, QString>* mIntrusionIndicators;

    /** The probability/rate that the node runs into a safety failure*/
    QString mFailureIndicator;

    /** List to store submodule evaluated failure indicators */
    std::map<qreal, QString>* mFailureIndicators;

    /** The security guarantees offered by this node*/
    QString mSecurityIndicator;

    /** Recovery mechanisms of the node */
    QString mDefectRecoveryIndicator;

    /** Recovery mechanisms of the node */
    QString mCorruptionRecoveryIndicator;

    /** Contains the path to the simulation model (if the node is simulated) */
    QString mSimulationPath;

    /** Strategy that is applied in case of corruption */
    Recovery::Strategy mCorruptionRecoveryStrategy;

    /** Strategy in case of defect */
    Recovery::Strategy mDefectRecoveryStrategy;

    /** String containing the node dependencies of this node */
    QString mEssentialNodes;

    /** String containing the corruption recovery formula (custom mode) */
    QString mCorruptionRecoveryFormula;

    /** String containing the defect recovery formula (custom mode) */
    QString mDefectRecoveryFormula;

    /** Previous position of the node used resetting when move event results in overlapping of items
     */
    QPointF mPreviousPosition;

    /** TextItem of the node displaying its number */
    QGraphicsTextItem* mNodeText;

    /** Path to the submodule (if given) */
    QString mSubmodulePath;

    /** Contect menu for inspecting submodules */
    std::unique_ptr<QMenu> mContextMenu;
    
    /** Action for inspecting submodules */
    std::unique_ptr<QAction> mInspectAct;
    
    InspectActionObserver mObserver;
    
    QColor mCriticalRed;
    
    /** Storage variable to keep the original pen when changing to submodule */
    QPen mOriginalPen;
    
    GraphicScene* mScene = nullptr;
    
    bool mSceneIsInvalid = false;
};

}  // namespace graph

#endif /* ERIS_GRAPH_NODE_ITEM_H */
