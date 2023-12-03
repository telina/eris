#include <gtest/gtest.h>
#include "../src/node_item.h"
#include "../src/node_settings.h"
#include "../src/graphic_scene.h"
#include "../src/component_type.h"
#include <QPointF>
#include "../src/main_window.h"

class NodeSettingsValidatorTest : public ::testing::Test {

  protected:

    NodeSettingsValidatorTest() 
    {

    }

    virtual ~NodeSettingsValidatorTest() 
    {
        
    }

    virtual void SetUp() 
    {

    }

    virtual void TearDown() 
    {

    }
    
    
  };
  
    TEST_F(NodeSettingsValidatorTest, isValidIntrusionValueTest)
    {
        // MDP
        ASSERT_TRUE(NodeSettingsValidator::isValidIntrusionValue("1", ModelType::MDP));
        ASSERT_TRUE(NodeSettingsValidator::isValidIntrusionValue("0.515", ModelType::MDP));
        ASSERT_TRUE(NodeSettingsValidator::isValidIntrusionValue("0", ModelType::MDP));
        ASSERT_TRUE(NodeSettingsValidator::isValidIntrusionValue("0.00", ModelType::MDP));
        ASSERT_TRUE(NodeSettingsValidator::isValidIntrusionValue("1.0", ModelType::MDP));
        
        ASSERT_FALSE(NodeSettingsValidator::isValidIntrusionValue("-1", ModelType::MDP));
        ASSERT_FALSE(NodeSettingsValidator::isValidIntrusionValue("2", ModelType::MDP));
        ASSERT_FALSE(NodeSettingsValidator::isValidIntrusionValue("1.1", ModelType::MDP));
        
        //CTMC
        ASSERT_TRUE(NodeSettingsValidator::isValidIntrusionValue("1", ModelType::CTMC));
        ASSERT_TRUE(NodeSettingsValidator::isValidIntrusionValue("1.54356", ModelType::CTMC));
        ASSERT_TRUE(NodeSettingsValidator::isValidIntrusionValue("0.000005", ModelType::CTMC));
        ASSERT_TRUE(NodeSettingsValidator::isValidIntrusionValue("265.15", ModelType::CTMC));
        ASSERT_TRUE(NodeSettingsValidator::isValidIntrusionValue("0", ModelType::CTMC));
        
        //General
        ASSERT_FALSE(NodeSettingsValidator::isValidIntrusionValue("1,6", ModelType::CTMC));
        ASSERT_FALSE(NodeSettingsValidator::isValidIntrusionValue("1,6", ModelType::MDP));
    }
    
    TEST_F(NodeSettingsValidatorTest, isValidFailureValueTest)
    {
        // MDP
        ASSERT_TRUE(NodeSettingsValidator::isValidFailureValue("1", ModelType::MDP));
        ASSERT_TRUE(NodeSettingsValidator::isValidFailureValue("0.515", ModelType::MDP));
        ASSERT_TRUE(NodeSettingsValidator::isValidFailureValue("0", ModelType::MDP));
        ASSERT_TRUE(NodeSettingsValidator::isValidFailureValue("0.00", ModelType::MDP));
        ASSERT_TRUE(NodeSettingsValidator::isValidFailureValue("1.0", ModelType::MDP));
        
        ASSERT_FALSE(NodeSettingsValidator::isValidFailureValue("-1", ModelType::MDP));
        ASSERT_FALSE(NodeSettingsValidator::isValidFailureValue("2", ModelType::MDP));
        ASSERT_FALSE(NodeSettingsValidator::isValidFailureValue("1.1", ModelType::MDP));
        
        //CTMC
        ASSERT_TRUE(NodeSettingsValidator::isValidFailureValue("1", ModelType::CTMC));
        ASSERT_TRUE(NodeSettingsValidator::isValidFailureValue("1.54356", ModelType::CTMC));
        ASSERT_TRUE(NodeSettingsValidator::isValidFailureValue("0.000005", ModelType::CTMC));
        ASSERT_TRUE(NodeSettingsValidator::isValidFailureValue("265.15", ModelType::CTMC));
        ASSERT_TRUE(NodeSettingsValidator::isValidFailureValue("0", ModelType::CTMC));
        
        //General
        ASSERT_FALSE(NodeSettingsValidator::isValidFailureValue("1,6", ModelType::CTMC));
        ASSERT_FALSE(NodeSettingsValidator::isValidFailureValue("1,6", ModelType::MDP));
    }
    
    TEST_F(NodeSettingsValidatorTest, isValidSecurityValueTest)
    {
        // MDP
        ASSERT_TRUE(NodeSettingsValidator::isValidSecurityValue("1", ModelType::MDP));
        ASSERT_TRUE(NodeSettingsValidator::isValidSecurityValue("0.515", ModelType::MDP));
        ASSERT_TRUE(NodeSettingsValidator::isValidSecurityValue("0", ModelType::MDP));
        ASSERT_TRUE(NodeSettingsValidator::isValidSecurityValue("0.00", ModelType::MDP));
        ASSERT_TRUE(NodeSettingsValidator::isValidSecurityValue("1.0", ModelType::MDP));
        
        ASSERT_FALSE(NodeSettingsValidator::isValidSecurityValue("-1", ModelType::MDP));
        ASSERT_FALSE(NodeSettingsValidator::isValidSecurityValue("2", ModelType::MDP));
        ASSERT_FALSE(NodeSettingsValidator::isValidSecurityValue("1.1", ModelType::MDP));
        
        //CTMC
        ASSERT_TRUE(NodeSettingsValidator::isValidSecurityValue("1", ModelType::CTMC));
        ASSERT_TRUE(NodeSettingsValidator::isValidSecurityValue("1.54356", ModelType::CTMC));
        ASSERT_TRUE(NodeSettingsValidator::isValidSecurityValue("0.000005", ModelType::CTMC));
        ASSERT_TRUE(NodeSettingsValidator::isValidSecurityValue("265.15", ModelType::CTMC));
        ASSERT_TRUE(NodeSettingsValidator::isValidSecurityValue("0", ModelType::CTMC));
        
        //General
        ASSERT_FALSE(NodeSettingsValidator::isValidSecurityValue("1,6", ModelType::CTMC));
        ASSERT_FALSE(NodeSettingsValidator::isValidSecurityValue("1,6", ModelType::MDP));
    }
    
    TEST_F(NodeSettingsValidatorTest, isValidIdTest)
    {
        ASSERT_TRUE(NodeSettingsValidator::isValidId("1"));
        ASSERT_TRUE(NodeSettingsValidator::isValidId("145253"));
        ASSERT_FALSE(NodeSettingsValidator::isValidId("1.6"));
        ASSERT_FALSE(NodeSettingsValidator::isValidId("-15"));
        ASSERT_FALSE(NodeSettingsValidator::isValidId(""));
    }
    
    TEST_F(NodeSettingsValidatorTest, isValidNewIdTest)
    {
        Counter::getInstance().clear();
        Counter::getInstance().getNext();
        ASSERT_TRUE(NodeSettingsValidator::isValidNewId("1", "0"));
        ASSERT_TRUE(NodeSettingsValidator::isValidNewId("145253", "1"));
        
        ASSERT_FALSE(NodeSettingsValidator::isValidNewId("1.6", "1"));
        ASSERT_FALSE(NodeSettingsValidator::isValidNewId("-15", "1"));
    }
    
    TEST_F(NodeSettingsValidatorTest, isValidRedundancyDefinitionTest)
    {
        Counter::getInstance().clear();
        GraphicScene* testScene = new GraphicScene();
        NodeItem* nodeA = new NodeItem(Component::Type::criticalNode, 0, QPointF(10, 10));
        NodeItem* nodeB = new NodeItem(Component::Type::criticalNode, 1, QPointF(30, 30));
        NodeItem* nodeC = new NodeItem(Component::Type::criticalNode, 2, QPointF(55, 70));
        testScene->addItem(nodeA);
        testScene->addItem(nodeB);
        testScene->addItem(nodeC);
    
        ASSERT_TRUE(NodeSettingsValidator::isValidRedundancyDefinition("n0 = n1", testScene));
        ASSERT_TRUE(NodeSettingsValidator::isValidRedundancyDefinition("n0=n1", testScene));
        ASSERT_TRUE(NodeSettingsValidator::isValidRedundancyDefinition("n0=n1, n2=n0", testScene));
        ASSERT_TRUE(NodeSettingsValidator::isValidRedundancyDefinition("n1=n0", testScene));
        
        //ASSERT_FALSE(NodeSettingsValidator::isValidRedundancyDefinition("n1=n2=n3", testScene));
        //ASSERT_FALSE(NodeSettingsValidator::isValidRedundancyDefinition("n1==n2", testScene));
    }
    
    TEST_F(NodeSettingsValidatorTest, isValidEssentialNodesTest)
    {
        Counter::getInstance().clear();
        GraphicScene* testScene = new GraphicScene();
        NodeItem* nodeA = new NodeItem(Component::Type::criticalNode, 0, QPointF(10, 10));
        NodeItem* nodeB = new NodeItem(Component::Type::criticalNode, 1, QPointF(30, 30));
        NodeItem* nodeC = new NodeItem(Component::Type::criticalNode, 2, QPointF(55, 70));
        EdgeItem* funcEdge1 = new EdgeItem(Component::Type::functionalEdge, nodeA, nodeB);
        EdgeItem* funcEdge2 = new EdgeItem(Component::Type::functionalEdge, nodeC, nodeB);
        testScene->addItem(nodeA);
        testScene->addItem(nodeB);
        testScene->addItem(nodeC);
        nodeA->addEdgeItem(funcEdge1);
        nodeB->addEdgeItem(funcEdge1);
        nodeC->addEdgeItem(funcEdge2);
        nodeB->addEdgeItem(funcEdge2);
    
        ASSERT_TRUE(NodeSettingsValidator::isValidEssentialNodes("n0=0", nodeB));
        ASSERT_TRUE(NodeSettingsValidator::isValidEssentialNodes("n0=0 & n2=0 ", nodeB));
        ASSERT_TRUE(NodeSettingsValidator::isValidEssentialNodes("n0=0 | n2 = 0", nodeB));
        
        ASSERT_FALSE(NodeSettingsValidator::isValidEssentialNodes("n1=0", nodeB)); // no func edge 
        ASSERT_FALSE(NodeSettingsValidator::isValidEssentialNodes("n3=0", nodeB)); // does not exist
        ASSERT_FALSE(NodeSettingsValidator::isValidEssentialNodes("n1=0, n2=0", nodeB)); // syntactically incorrect
    }
    
    TEST_F(NodeSettingsValidatorTest, isValidRecoveryStrategy)
    {
        ASSERT_TRUE(NodeSettingsValidator::isValidRecoveryStrategy("Restricted", 1));
        ASSERT_TRUE(NodeSettingsValidator::isValidRecoveryStrategy("General", 1));
        ASSERT_TRUE(NodeSettingsValidator::isValidRecoveryStrategy("Custom", 1));
        ASSERT_FALSE(NodeSettingsValidator::isValidRecoveryStrategy("custom", 1));
        ASSERT_FALSE(NodeSettingsValidator::isValidRecoveryStrategy("ReSTricted", 1));
        ASSERT_FALSE(NodeSettingsValidator::isValidRecoveryStrategy("", 1));
    }
    
    TEST_F(NodeSettingsValidatorTest, isValidRecoveryFormula)
    {
        GraphicScene* testScene = new GraphicScene();
        NodeItem* nodeA = new NodeItem(Component::Type::criticalNode, 0, QPointF(10, 10));
        NodeItem* nodeB = new NodeItem(Component::Type::criticalNode, 1, QPointF(30, 30));
        NodeItem* nodeC = new NodeItem(Component::Type::criticalNode, 2, QPointF(55, 70));
        testScene->addItem(nodeA);
        testScene->addItem(nodeB);
        testScene->addItem(nodeC);
        
        ASSERT_TRUE(NodeSettingsValidator::isValidRecoveryFormula("n0=0", testScene));
        ASSERT_TRUE(NodeSettingsValidator::isValidRecoveryFormula("n0=0 | n1=0", testScene));
        ASSERT_TRUE(NodeSettingsValidator::isValidRecoveryFormula("n0=1 & n1=0 | n2=0", testScene));
        ASSERT_FALSE(NodeSettingsValidator::isValidRecoveryFormula("n3=0", testScene)); // does not exist
        ASSERT_FALSE(NodeSettingsValidator::isValidRecoveryFormula("n1=3", testScene));
    }

    TEST_F(NodeSettingsValidatorTest, isValidRecoveryFlag)
    {
        ASSERT_TRUE(NodeSettingsValidator::isValidRecoveryFlag("true", 1));
        ASSERT_TRUE(NodeSettingsValidator::isValidRecoveryFlag("false", 1));
        ASSERT_FALSE(NodeSettingsValidator::isValidRecoveryFlag("1", 1));
        ASSERT_TRUE(NodeSettingsValidator::isValidRecoveryFlag("", 1)); // interpreted as false
    }

        
    TEST_F(NodeSettingsValidatorTest, isValidComponentType)
    {
        ASSERT_FALSE(NodeSettingsValidator::isValidComponentType("0", 1));
        ASSERT_TRUE(NodeSettingsValidator::isValidComponentType("1", 1));
        ASSERT_TRUE(NodeSettingsValidator::isValidComponentType("2", 1));
        ASSERT_TRUE(NodeSettingsValidator::isValidComponentType("3", 1));
        ASSERT_TRUE(NodeSettingsValidator::isValidComponentType("4", 1));
        ASSERT_TRUE(NodeSettingsValidator::isValidComponentType("5", 1));
        ASSERT_TRUE(NodeSettingsValidator::isValidComponentType("6", 1));
        ASSERT_FALSE(NodeSettingsValidator::isValidComponentType("7", 1));
    }
