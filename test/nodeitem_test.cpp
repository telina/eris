#include <gtest/gtest.h>
#include "node_item.h"
#include "edge_item.h"
#include "graphic_scene.h"
#include "component_type.h"
#include <QPointF>
#include "main_window.h"

class NodeItemTest : public ::testing::Test {

  protected:

    NodeItemTest() 
    {

    }

    virtual ~NodeItemTest() 
    {
        
    }

    virtual void SetUp() 
    {

    }

    virtual void TearDown() 
    {
        Counter::getInstance().clear();
    }
    
  };
  

    TEST_F(NodeItemTest, addEdgeItemsAndNodeItems)
    {
        NodeItem* criticalNode = new NodeItem(Component::Type::criticalNode, 15, QPointF(10, 10));
        NodeItem* normalNode = new NodeItem(Component::Type::normalNode, 7, QPointF(30, 30));
        NodeItem* envNode = new NodeItem(Component::Type::environmentNode, 0, QPointF(50, 50));
        
        EdgeItem* secEdge = new EdgeItem(Component::Type::securityEdge, normalNode, criticalNode);
        criticalNode->addEdgeItem(secEdge);
        normalNode->addEdgeItem(secEdge);
        
        EdgeItem* reachEdge = new EdgeItem(Component::Type::reachEdge, envNode, normalNode);
        envNode->addEdgeItem(reachEdge);
        normalNode->addEdgeItem(reachEdge);
        
        EdgeItem* fctEdge = new EdgeItem(Component::Type::functionalEdge, normalNode, criticalNode);
        normalNode->addEdgeItem(fctEdge);
        criticalNode->addEdgeItem(fctEdge);
        
        EXPECT_TRUE(normalNode->edgeExists(criticalNode));
        EXPECT_TRUE(envNode->edgeExists(normalNode));
        EXPECT_TRUE(criticalNode->providesFunctionality(normalNode->getId()));
        
        EXPECT_FALSE(normalNode->edgeExists(envNode));
        EXPECT_FALSE(envNode->edgeExists(criticalNode));
        EXPECT_FALSE(criticalNode->edgeExists(normalNode));
        
        EXPECT_TRUE(normalNode->edgeWithTypeExists(criticalNode, Component::Type::securityEdge));
        
    }
    
    TEST_F(NodeItemTest, RemoveEdgeItems)
    {
        NodeItem* criticalNode = new NodeItem(Component::Type::criticalNode, 15, QPointF(10, 10));
        NodeItem* normalNode = new NodeItem(Component::Type::normalNode, 7, QPointF(30, 30));
        NodeItem* envNode = new NodeItem(Component::Type::environmentNode, 0, QPointF(50, 50));
        
        EdgeItem* secEdge = new EdgeItem(Component::Type::securityEdge, normalNode, criticalNode);
        criticalNode->addEdgeItem(secEdge);
        normalNode->addEdgeItem(secEdge);
        
        EdgeItem* reachEdge = new EdgeItem(Component::Type::reachEdge, envNode, normalNode);
        envNode->addEdgeItem(reachEdge);
        normalNode->addEdgeItem(reachEdge);
        
        EdgeItem* fctEdge = new EdgeItem(Component::Type::functionalEdge, normalNode, criticalNode);
        normalNode->addEdgeItem(fctEdge);
        criticalNode->addEdgeItem(fctEdge);
        
        EXPECT_TRUE(envNode->edgeExists(normalNode));
        envNode->removeEdgeItem(reachEdge);
        EXPECT_FALSE(envNode->edgeExists(normalNode));
        
        EXPECT_TRUE(normalNode->edgeExists(criticalNode));
        normalNode->removeEdgeItem(secEdge);
        EXPECT_TRUE(normalNode->edgeExists(criticalNode));
        EXPECT_FALSE(normalNode->edgeWithTypeExists(criticalNode, Component::Type::securityEdge));
        EXPECT_TRUE(normalNode->edgeWithTypeExists(criticalNode, Component::Type::functionalEdge));
    }
    
    TEST_F(NodeItemTest, nodeCriticality)
    {
        NodeItem* criticalNode = new NodeItem(Component::Type::criticalNode, 15, QPointF(10, 10));
        NodeItem* normalNode = new NodeItem(Component::Type::normalNode, 7, QPointF(30, 30));
        EXPECT_TRUE(criticalNode->isCritical());
        EXPECT_FALSE(normalNode->isCritical());
    }
    
    TEST_F(NodeItemTest, GetAndSetId)
    {
        unsigned int id = Counter::getInstance().getNext();
        NodeItem* nodeItem = new NodeItem(Component::Type::criticalNode, id, QPointF(10,10));
        EXPECT_EQ(nodeItem->getId(), id);
        unsigned int newTestId = 12;
        nodeItem->setId(newTestId);
        EXPECT_EQ(nodeItem->getId(), newTestId);
        
    }   
    
/*    TEST_F(NodeItemTest, setterAndGetter)
    {
        NodeItem* nodeItem = new NodeItem(Component::Type::criticalNode, 1, QPointF(10,10));
        QString exampleString = "n2=0";
        nodeItem->setEssentialNodes(exampleString);
        EXPECT_EQ(exampleString, nodeItem->getEssentialNodes());

    }*/