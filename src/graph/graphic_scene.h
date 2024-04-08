/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
 *      - 2020-2023 Rhea Rinaldo (Rhea@Odlanir.de)
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

#ifndef ERIS_GRAPH_GRAPH_SCENE_H
#define ERIS_GRAPH_GRAPH_SCENE_H

#include "component_type.h"

#include "eris_config.h"
#include "main_window_actions_manager.h"
#include "main_window_tool_buttons_manager.h"
#include "main_window_buttons_group_manager.h"
#include "file_manager.h"
#include "experiment.h"
#include "prism_results_parser.h"
#include "rate_interpretation.h"

#include <QGraphicsScene>
#include <vector>
#include <list>
#include <mutex>
#include <optional>
#include <unordered_map>

#define GRAPHIC_SCENE() (graph::GraphicScene::Factory::getInstance()->current())
#define GRAPHIC_SCENE_FACTORY() (graph::GraphicScene::Factory::getInstance())

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QGraphicsView;
class QInputDialog;
class QMessageBox;
QT_END_NAMESPACE

namespace widgets
{
class GraphicsView;
}

namespace graphInternal
{
class Transformer;
}
namespace graph
{
class TextItem;
class NodeItem;
class EdgeItem;
class Counter;
using widgets::MainWindowActionsManager;
using widgets::MainWindowButtonsGroupManager;
using widgets::MainWindowToolButtonsManager;
/**
 * This class represents the graphic scene, inheriting from the dedicated QT Object and contains
 * the graphically presented objects that.
 */
class ERIS_EXPORT GraphicScene : public QGraphicsScene
{
    Q_OBJECT

public:
    class ERIS_EXPORT Factory
    {
        
    public:
        
        class ERIS_EXPORT Observer
        {
            
        public:
            
            virtual ~Observer()
            {
                
            }
            
            virtual void
            graphicSceneIsAboutToBeDeleted(GraphicScene*, int /*idx*/)
            {
                
            }
            
            virtual void
            subGraphicSceneAdded(GraphicScene*, int /*idx*/) {};
        };
        
        static Factory* getInstance();
        
        ERIS_DISALLOW_COPY_AND_ASSIGN(Factory);
        
        GraphicScene* current();
        
        GraphicScene* at(int i);

        int
        getIdx();
        
        bool
        setCurrentGraphicScene(int idx);
        
        bool
        addSubGraphicScene();
        
        bool
        addSubGraphicScene(int* idx_out, const QString& model_path = QString());
        
        bool
        deleteSubGraphicScene(int idx);
        
        void
        clear();
        
        size_t
        size();
        
        bool
        isEmpty();

        void
        registerObserver(Observer* o);
        
        void
        unregisterObserver(Observer* o);

        ~Factory();

    private:
        
        Factory();
        
        void
        notifyDeleted(GraphicScene*, int /*idx*/);
        
        void
        notifyAdded(GraphicScene*, int /*idx*/);

    private:
        
        std::vector<std::unique_ptr<GraphicScene>> scenes_;
        std::list<Observer*> observers_;
        std::recursive_mutex lock_;
        int idx_;
        
    };
    enum Mode
    {
        InsertNormalNode,
        InsertCriticalNode,
        InsertEnvironmentNode,
        InsertReachEdge,
        InsertFunctionalEdge,
        InsertSecurityEdge,
        InsertText,
        ModifyItem,
        Navigate
    };

    ERIS_DISALLOW_COPY_AND_ASSIGN(GraphicScene);
    ~GraphicScene() override;

    widgets::GraphicsView*
    graphics_view() const
    {
        return mGraphicsView.get();
    }

    const QString&
    path() const
    {
        return mFileName;
    }

    bool
    isPrismMode() const
    {
        return mPrismMode;
    }
    
    void
    setPrismMode(bool v)
    {
        mPrismMode = v;
    }
    
    void
    setMode(Mode mode);
    
    /**
     * Updates the Rate Interpretation (Time Unit) from a previously set one to the provided.
     * Therefore it is iterated over all modeled node items and the rate are multiplied by the
     * according factor.
     * Also opens a User Prompt before proceeding.
     * @param newRateInterpret
     */
    void 
    updateRateInterpretation(graph::RateInterpretation newRateInterpret);
    
    /**
     * Sets the internal rate interpretation member variable. Note, this function does not change 
     * the actual rate interpretation of the nodes! It is used to set the rate interpretation
     * of a model after loading it.
     * @param newRateInterpret
     */
    void 
    setRateInterpretation(graph::RateInterpretation rateInterpret);
    
    
    graph::RateInterpretation
    getCurrentRateInterpretation();

    Counter*
    getCounter() const
    {
        return mCounter.get();
    }

    // Do not call this method if you don't know what you are doing.
    void
    setStateChanged(bool changed)
    {
        has_changed_ = changed;
    }
    
    bool
    hasChanged() const
    {
        // TODO Just for now to make start Verification work
        return true;  // has_changed_;
    }
    
    bool
    exportAs(utils::FileManager::Extension extension, const QString& path);

    /**
     * Loads the submodule given by the string path. The passive flags indicate
     * whether the submodule shall be openend in the GUI or not.
     * @param path submodule
     * @param passive open in GUI if false
     * @return true on success, false otherwise
     */
    bool
    load(const QString& path, bool passive = false);
    
    bool
    saveAs(const QString& path);
    
    bool
    save();
    
    
    bool
    buildAndStartXPrism();
    
    bool
    transform();

    /**
     * Starts the experiment evaluation in PRISM for the current module in the graphic scene.
     * If the scene contains submodule nodes, their evaluation is performed beforehand.
     * Results are displayed in the plot.
     */
    bool
    evaluateExperiment();

   /* bool
    stopTransformer();*/
    bool
    optionAddRedundancy();
    
    bool
    setOutfileName();
    
    bool
    deleteItem();
    
    bool
    setNodeMode(int id);
    
    bool
    setAnnotationMode(int id);
    
    bool
    addSubmodule(const QString& submodule_path, NodeItem* node);
    
    bool
    removeSubmodule(const QString& submodule_path, NodeItem* node);
    
    int
    id() const
    {
        return mId;
    }

    /**
     * Opens the settings of the selected node(s).
     */
    bool
    openItemSettings(bool checked = false);

    /**
     * Iterates over all items in the scene and sorts them by their type. The provided vectors
     * are filled with envnode, critical and normal nodes, and edges.
     * @param nodes vector filled with critical and nromal nodes
     * @param envNodes vector filled with env nodes
     * @param edges vector filled with edges
     */
    void
    getSortedSceneItems(std::vector<NodeItem*>& nodes,
                        std::vector<NodeItem*>& envNodes,
                        std::vector<EdgeItem*>& edges);

    /**
     * Iterates over all items in the scene and collects only submodule nodes
     * The provided vectors are filled with the found critical and normal nodes.
     * @param nodes vector filled with critical and nromal nodes
     */
    void
    getModuleNodeItems(std::vector<NodeItem*>& nodes);


    QString
    getOutfileName();
    
    /**
     * Removes a node item from the scene and takes care of edges and other dependencies.
     * @param nodeItem that shall be deleted
     */
    void
    removeNodeItem(NodeItem* nodeItem);

    /**
     * Removes an edge item from the scene and takes care of edges and other dependencies.
     * @param edgeItem that shall be deleted
     */
    void
    removeEdgeItem(EdgeItem* edgeItem);

    /**
     * Removes a tect item from the scene.
     * @param textItem that shall be deleted
     */
    void
    removeTextItem(TextItem* textItem);

    /**
     * Get the item at the provided coordinates of the scene.
     * @param x coordinate
     * @param y coordinate
     * @return item at the position
     */
    NodeItem*
    getNodeItemAt(QPointF point);

    /**
     * Checks if an item is at the given position.
     * @param point
     * @return true if item is at point, false otherwise
     */
    bool
    isItemAt(QPointF point);
    
    /**
     * Filters all graphics items by nodes and searches for the node with the
     * given ID. Returns the node if it is found, NULL otherwise.
     * @param id identifier for the searched node
     * @param node the node to store the found node in
     * @return Node Item if found, NULL otherwise
     */
    bool
    getNodeItemById(unsigned int id, NodeItem** node, bool silent=false);

    /**
     * Iterates over all normal/crtical nodes and checks whether a node with the given id exists!
     * Environment are explicitly excluded.
     * @param id
     * @return true if node exists, false otherwise
     */
    bool
    nodeExists(unsigned int id);

    /**
     * When an edge item is drawn between two node items that already are connected by edge items,
     * the existing edge items have to be updated to make space for the new edge item. Thus this
     * function is called and resets the position of the edge items using offsets.
     * @param collidingEdges all edge items between two node items
     */
    void
    updateCollidingEdges(std::vector<EdgeItem*> collidingEdges);

signals:

    /**
     * Triggered when two items are selected
     * **/
    void
    enableSwapId(bool enable);

    /**
     * Signal triggered on item insertion.
     */
    void
    itemInserted();

    /**
     * Signal triggered on item selection.
     */
    void
    itemSelected();

    /**
     * Signal triggered on item deselection.
     */
    void
    itemDeselected();

public slots:

    /**
     * If triggered change the state of the swap id button
     * */
    void
    onEnableSwapId();

    /**
     * Triggered when the mouse is right-clicked on empty space. Deselects all selected items and
     * buttons.
     */
    void
    onItemDeselected();

    /**
     * Not implemented yet
     * @param item
     */
    void
    onItemSelected();

    /**
     * Not implemented yet
     * @param item
     */
    void
    onItemInserted();

    void
    onAreaSelection(const QPolygonF&);
    /**
     * Swap node id with another node id
     * **/
    void
    swapNodeId(bool checked = false);
    /**
     * Swaps the criticality of the node from critical to uncritical and vice versa.
     */
    void
    swapNodeCriticality(bool checked = false);

protected:
    /**
     * Whenever the mouse is pressed this function is triggered and the current mode is viewed.
     * In case an insertion of an item is detected, the routine for adding a node or an edge is
     * performed.
     * @param mouseEvent
     */
    void
    mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent) override;

    /**
     * This function is used to draw a line/edge between two nodes.
     * @param mouseEvent
     */
    void
    mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent) override;

    /**
     * Depending on the node either a line is drawn or a selected item is moved.
     * @param mouseEvent
     */
    void
    mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
    
private:
    static std::unique_ptr<GraphicScene>
    Create(QObject* parent, bool passive_submodule = false);

    /**
     * Creates GraphicScene object with QT parent.
     * @param parent
     */
    explicit GraphicScene(QObject* parent, bool passive_submodule);
    bool
    init();
    /**
     * Checks whether an edge between the given node to an env item already exists.
     * @param targetNode
     * @return true if edge exists, false otherwise
     */
    bool
    edgeToOtherEnvExists(NodeItem* targetNode);

    /**
     * Checks whether the given node item overlaps with the already existing node items. In case it
     * does, true is returned and the error handler is shown.
     * @param newItem
     * @return true if overlaps, false otherwise
     */
    bool
    nodeOverlaps(NodeItem* newItem);

    /**
     * Triggers Markov evaluation process (via PRISM) for the given ERIS module node and
     * stores results in the associated NodeItems map. 
     */
    bool
    evaluteErisModule(NodeItem* submoduleNode, eval::ExperimentInterval interval);
    
    /**
     * Triggers the simulation process (via Octave) for the given Simulation module node and
     * stores results in the associated NodeItems map. 
     */
    bool
    evaluteSimulationModule(NodeItem* submoduleNode, eval::ExperimentInterval interval);

    bool mPrismMode = false;

    /** Currently drawn line, basis for a new edge item */
    QGraphicsLineItem* mLine;

    /** Type of currently drawn edge item */
    graph::ComponentType mLineType;

    /** Insertion mode due to clicked button etc in the main window */
    Mode mMode;

    /** Origin mouse position relative to the scene, when the selection area Started */
    QPointF mOriginPos;

    std::unique_ptr<widgets::GraphicsView> mGraphicsView;

    /** Output file name of the transformed scene (.pm) */
    QString mOutFileName;

    /** Redundancy definition */
    std::string mRedundancy;

    /** File name to store and load the graphics scene in */
    QString mFileName;

    /** Name of the exportAs file (.png/.pdf/.svg) */
    QString mExportName;

    const int mId;
    
    /** Reference to the counter object, needed to get a fresh ID for an inserted node */
    std::unique_ptr<Counter> mCounter;
    
    bool has_changed_ = false;
    
    const bool mIsPassiveSubmodule;
    
    /** List or rather map of Submodules*/
    std::unordered_map<QString /*path*/, NodeItem* /*node item*/> mSubmodules;
    
    /** Enum indicating in what form rates are interpreted. This is currently graphic scene dependent*/
    graph::RateInterpretation mActiveRateInterpretation;

public:
    std::unique_ptr<graphInternal::Transformer> mTransformer;
};

}  // namespace graph

#endif /* ERIS_GRAPH_GRAPH_SCENE_H */
