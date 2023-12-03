#include <gtest/gtest.h>
#include "node_item.h"
#include "edge_item.h"
#include "graphic_scene.h"
#include "component_type.h"
#include <QPointF>
#include "main_window.h"

class GraphTest : public ::testing::Test {

  protected:

    GraphTest() 
    {
        mWindow = new MainWindow();
        mTestScene = new GraphicScene();
    }

    virtual ~GraphTest() 
    {
        
    }

    virtual void SetUp() 
    {
        
    }

    virtual void TearDown() 
    {
        Counter::getInstance().clear();
        mTestScene->clear();
    }

    void createGraph()
    {
        mIdA = Counter::getInstance().getNext();
        mIdB = Counter::getInstance().getNext();
        mNodeA = new NodeItem(Component::Type::criticalNode, mIdA, QPointF(10, 10));
        mNodeB = new NodeItem(Component::Type::normalNode, mIdB, QPointF(30, 30));
        mEdge = new EdgeItem(Component::Type::reachEdge, mNodeA, mNodeB);
        
        mTestScene->addItem(mNodeA);
        mTestScene->addItem(mNodeB);
        mTestScene->addItem(mEdge);
        mNodeA->addEdgeItem(mEdge);
        mNodeB->addEdgeItem(mEdge);
    }
    
    GraphicScene* mTestScene;
    MainWindow* mWindow;
    
    NodeItem* mNodeA;
    NodeItem* mNodeB;
    EdgeItem* mEdge;
    unsigned int mIdA;
    unsigned int mIdB;

  };
  

    TEST_F(GraphTest, InsertNode)
    {
        // Test adding a node via mouse event


    }

    TEST_F(GraphTest, CreateGraph)
    {     
        createGraph();
        
        EXPECT_TRUE(mTestScene->nodeExists(mNodeA->getId()));
        EXPECT_TRUE(mTestScene->nodeExists(mNodeB->getId()));
        EXPECT_TRUE(mNodeA->edgeExists(mNodeB));      
    }
  
  
    TEST_F(GraphTest, RemoveNodeItem)
    {
        createGraph();
        mTestScene->removeNodeItem(mNodeA);
       
        EXPECT_FALSE(mTestScene->nodeExists(mIdA)); // Expect that the node cannot be found anymore
        EXPECT_TRUE(mTestScene->nodeExists(mIdB)); // Expect that node B still exists!
        mTestScene->removeNodeItem(mNodeB);
        // Expect that no node exists anymore because both nodes got deleted and also the edge was removed
        EXPECT_TRUE(mTestScene->items().empty());
    }
    
    
    TEST_F(GraphTest, RemoveEdgeItem)
    {
        createGraph(); 
        
        mTestScene->removeEdgeItem(mEdge);
        EXPECT_FALSE(mNodeA->edgeExists(mNodeB));
    }
    
    TEST_F(GraphTest, AddTextItem)
    {
        // TODO via mouse event
        
    }
    
    TEST_F(GraphTest, RemoveTextItem)
    {
        TextItem* textItem = new TextItem("Hello World", QPointF(30,30));
        mTestScene->addItem(textItem);
        EXPECT_EQ(mTestScene->items().length(), 1);
        mTestScene->removeTextItem(textItem);
        EXPECT_EQ(mTestScene->items().length(), 0);
        
    }
    
    TEST_F(GraphTest, FindNodeItem)
    {
        createGraph();
        NodeItem* testItemA;
        EXPECT_TRUE(mTestScene->getNodeItemById(mNodeA->getId(), &testItemA));
        EXPECT_TRUE(testItemA != NULL);
        EXPECT_EQ(mNodeA, testItemA);

        NodeItem* testItemB;
        EXPECT_TRUE(mTestScene->getNodeItemById(mNodeB->getId(), &testItemB));
        EXPECT_TRUE(testItemB != NULL);
        EXPECT_EQ(mNodeB, testItemB);
    }