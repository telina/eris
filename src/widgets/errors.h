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

#ifndef ERIS_WIDGETS_ERRORS_H
#define ERIS_WIDGETS_ERRORS_H

#include <QString>
#include <utility>
#include "eris_config.h"

namespace widgets
{
class ERIS_EXPORT Errors
{
public:
    enum Type
    {
        WARNING = 0,
        ERROR = 1,
    };

    static inline std::pair<Type, QString>
    inputValueSyntaxError(const QString& valueType)
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        valueType
                                                + " Probability/Rate "
                                                  "syntactically incorrect\n");
    }

    static inline std::pair<Type, QString>
    inputValueOutOfBounds(const QString& valueType)
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        valueType
                                                + " Probability out of "
                                                  "bounds\n");
    }

    static inline std::pair<Type, QString>
    iDOutOfBounds()
    {
        return std::pair<Type, QString>(Type::ERROR,
                                        "ID out of bounds.\n A node "
                                        "cannot be placed without a valid ID.\n");
    }

    static inline std::pair<Type, QString>
    iDInvalid(const QString& id)
    {
        return std::pair<Type, QString>(Type::ERROR,
                                        "ID \"" + id
                                                + "\" invalid.\n A node"
                                                  "cannot be placed without a valid ID.\n");
    }

    static inline std::pair<Type, QString>
    idUnavailable()
    {
        return std::pair<Type, QString>(Type::ERROR,
                                        "ID already in use.\n A node "
                                        "cannot be placed without a valid ID.\n");
    }

    static inline std::pair<Type, QString>
    essentialNodeSyntacticallyIncorrect(unsigned int node)
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        "Essential Nodes of n" + QString::number(node)
                                                + " syntactically incorrect.\n");
    }

    static inline std::pair<Type, QString>
    recoveryFormulaSyntacticallyIncorrect()
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        "Custom recovery formula "
                                        "syntactically incorrect.\n");
    }

    static inline std::pair<Type, QString>
    essentialNodeNotInSetOfFunctionalNodes(unsigned int node, unsigned int essential)
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        "Essential Node " + QString::number(essential)
                                                + " not in set of functional nodes of "
                                                + QString::number(node) + " - will be skipped.\n");
    }

    static inline std::pair<Type, QString>
    redundancySyntacticallyIncorrect()
    {
        return std::pair<Type, QString>(Type::ERROR,
                                        "Redundancy Definition "
                                        "syntactically incorrect.\n");
    }

    static inline std::pair<Type, QString>
    nonCriticalNodeRedundant()
    {
        return std::pair<Type, QString>(Type::ERROR,
                                        "Only critical nodes may be defined "
                                        "redundant!");
    }

    static inline std::pair<Type, QString>
    essentialNodeDoesNotProvideFunctionality()
    {
        return std::pair<Type, QString>(Type::ERROR,
                                        "Essential Nodes: one or more nodes "
                                        "do not provide functionality!\n");
    }

    static inline std::pair<Type, QString>
    customRecoveryNodeDoesNotExist(unsigned int id)
    {
        return std::pair<Type, QString>(Type::ERROR,
                                        "Custom recovery formula: node "
                                        "with id "
                                                + QString::number(id) + " does not exist!\n");
    }

    static inline std::pair<Type, QString>
    noNodeWithId(unsigned int id)
    {
        return std::pair<Type, QString>(
                Type::ERROR, "Could not find node with ID: " + QString::number(id) + "\n");
    }

    static inline std::pair<Type, QString>
    nodeAlreadyExists(unsigned int id)
    {
        return std::pair<Type, QString>(
                Type::ERROR, "Node with ID: " + QString::number(id) + " already exists!\n");
    }

    static inline std::pair<Type, QString>
    openFileError(const QString& fileName)
    {
        return std::pair<Type, QString>(Type::ERROR,
                                        "Could not open file with name: " + fileName + "\n");
    }

    static inline std::pair<Type, QString>
    edgeAlreadyExists(unsigned int id, const QString& edgeType)
    {
        return std::pair<Type, QString>(
                Type::ERROR, edgeType + " to node " + QString::number(id) + " already exists\n");
    }

    static inline std::pair<Type, QString>
    edgeAlreadyExists()
    {
        return std::pair<Type, QString>(Type::ERROR, "Edge already exists!\n");
    }

    static inline std::pair<Type, QString>
    selfEdges()
    {
        return std::pair<Type, QString>(Type::ERROR, "Self edges are not permitted!\n");
    }

    static inline std::pair<Type, QString>
    edgeMustNotDirectToEnvironment()
    {
        return std::pair<Type, QString>(Type::ERROR,
                                        "Edges must not direct to "
                                        "environment nodes\n");
    }

    static inline std::pair<Type, QString>
    incorrectEdgeTypeForEnvironmentNode()
    {
        return std::pair<Type, QString>(Type::ERROR,
                                        "Edge coming from the environment "
                                        "node must be of type \"Reach\"\n");
    }

    static inline std::pair<Type, QString>
    positionAlreadyOccupied()
    {
        return std::pair<Type, QString>(Type::ERROR,
                                        "The requested position is already "
                                        "occupied (items must not overlap)\n");
    }

    static inline std::pair<Type, QString>
    missingNodes()
    {
        return std::pair<Type, QString>(Type::ERROR, "No system nodes given!\n");
    }

    static inline std::pair<Type, QString>
    transformationError()
    {
        return std::pair<Type, QString>(Type::ERROR,
                                        "PRISM could not be Started due to "
                                        "an unsuccessful transformation\n");
    }

    static inline std::pair<Type, QString>
    componentTypeUndefined(unsigned int id)
    {
        return std::pair<Type, QString>(Type::ERROR,
                                        "Component Type of n" + QString::number(id)
                                                + " not set or undefined\n");
    }

    static inline std::pair<Type, QString>
    componentTypeOutOfBounds(unsigned int id)
    {
        return std::pair<Type, QString>(
                Type::ERROR, "Component Type of n" + QString::number(id) + " is out of bounds!\n");
    }

    static inline std::pair<Type, QString>
    invalidRecoveryFlagInput(unsigned int id)
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        "Recovery flag of n" + QString::number(id)
                                                + " could not be interpreted\n Using default "
                                                  "'false' instead\n");
    }

    static inline std::pair<Type, QString>
    invalidSimulationFlagInput(unsigned int id)
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        "Simulation flag of n" + QString::number(id)
                                                + " could not be interpreted\n Using default "
                                                  "'false' instead\n");
    }
    static inline std::pair<Type, QString>
    InvalidSubmobuleFlagInput(unsigned int id)
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        "Submodule flag of n" + QString::number(id)
                                                + " could not be interpreted\n Using default "
                                                  "'false' instead\n");
    }

    static inline std::pair<Type, QString>
    recoveryFlagUndefined(unsigned int id)
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        "Recovery flag of n" + QString::number(id)
                                                + " was not defined.\n Using default 'false' "
                                                  "instead.");
    }

    static inline std::pair<Type, QString>
    simulationFlagUndefined(unsigned int id)
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        "Simulation flag of n" + QString::number(id)
                                                + " was not defined.\n Using default flag "
                                                  "'false' "
                                                  "instead.\n");
    }
    static inline std::pair<Type, QString>
    SubmoduleFlagUndefined(unsigned int id)
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        "Submodule flag of n" + QString::number(id)
                                                + " was not defined.\n Using default flag "
                                                  "'false' "
                                                  "instead.\n");
    }

    static inline std::pair<Type, QString>
    recoveryStrategyUndefined(unsigned int id)
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        "Recovery Strategy of n" + QString::number(id)
                                                + " was not defined.\n Using default strategy "
                                                  "'Restricted'"
                                                  " instead\n");
    }

    static inline std::pair<Type, QString>
    simulationPathNotSet(unsigned int id)
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        "Simulation path of n" + QString::number(id)
                                                + " was not set.\n Disabling simulation mode"
                                                  " instead\n");
    }
    static inline std::pair<Type, QString>
    submodulePathNotSet(unsigned int id)
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        "Submodule path of n" + QString::number(id)
                                                + " was not set.\n Disabling submodule mode"
                                                  " instead\n");
    }

    static inline std::pair<Type, QString>
    simulationPathIncorrect(unsigned int id)
    {
        return std::pair<Type, QString>(
                Type::WARNING, "Simulation path of n" + QString::number(id) + " is incorrect.\n ");
    }
    
    static inline std::pair<Type, QString>
    submodulePathIncorrect(const QString& path, unsigned int id)
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        "Submodule path of n" + QString::number(id) + " : " + path
                                                + " does not exist.\n ");
    }
    
    static inline std::pair<Type, QString>
    submoduleTransformationFailed(const QString& path, unsigned int id)
    {
        return std::pair<Type, QString>(Type::ERROR,
                                        "Failed to transform Submodule of node " + QString::number(id) + " with path: " + path + ".\n ");
    }

    static inline std::pair<Type, QString>
    octaveCommandNotAvailable()
    {
        return std::pair<Type, QString>(Type::ERROR,
                                        "Octave is not available via command"
                                        " 'octave'.\n"
                                        "Make sure that you installed octave correctly and "
                                        "set it in the PATH variable,"
                                        "otherwise the simulation cannot be used.\n");
    }

    static inline std::pair<Type, QString>
    prismCommandNotAvailable(const QString& cmd)
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        "PRISM is not available via command"
                                        " '" + cmd
                                                + "', the analysis is deactivated!\n"
                                                  "Make sure that you installed PRISM "
                                                  "correctly and set it in the PATH "
                                                  "variable.\n");
    }

    static inline std::pair<Type, QString>
    programExecutionError(const QString& program)
    {
        return std::pair<Type, QString>(Type::ERROR,
                                        "Due to an unknown problem the "
                                        "program '"
                                                + program
                                                + "' did not start/could not be executed.\n");
    }

    static inline std::pair<Type, QString>
    simulationStartError(const QString& program, const QString& errorOutput)
    {
        return std::pair<Type, QString>(Type::ERROR,
                                        "Due to an unknown problem the "
                                        "simulation using '"
                                                + program + "' could not be Started.\n\n"
                                                + errorOutput);
    }

    static inline std::pair<Type, QString>
    simulationError(const QString& program, const QString& errorOutput)
    {
        return std::pair<Type, QString>(
                Type::ERROR,
                "Due to an unknown problem the "
                "simulation using '"
                        + program + "' could not be Finished successfully.\n" + errorOutput);
    }

    static inline std::pair<Type, QString>
    transformationStopped()
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        "The Transformation process was "
                                        "stopped before it could be Finished.\n");
    }

    static inline std::pair<Type, QString>
    simulationOutputError(const QString& variable)
    {
        return std::pair<Type, QString>(Type::ERROR,
                                        "The value for '" + variable
                                                + "' could not be obtained. Perhaps the "
                                                  "simulation output was incorrect. \n");
    }

    static inline std::pair<Type, QString>
    invalidTextItem()
    {
        return std::pair<Type, QString>(Type::ERROR, "Text items should not be empty.\n");
    }

    static inline std::pair<Type, QString>
    invalidSwapNodeId()
    {
        return std::pair<Type, QString>(Type::ERROR,
                                        "Only normal and critical Nodes can swap their "
                                        "IDs.\n");
    }

    static inline std::pair<Type, QString>
    invalidTransitionsRemoved(const std::string& node, const std::string& transitionType)
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        "A " + QString::fromStdString(transitionType)
                                                + " transitions of node "
                                                + QString::fromStdString(node)
                                                + " that evaluated to or below"
                                                  " zero was skipped to avoid errors in "
                                                  "PRISM.\n");
    }

    static inline std::pair<Type, QString>
    restrictedRecoveryTransitionEmpty(const std::string& node)
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        "Restricted recovery strategy was activated for node "
                                                + QString::fromStdString(node)
                                                + ", but no path from the "
                                                  "environment node to this node exists."
                                                  "The transition was skipped to avoid "
                                                  "unnecessary "
                                                  "actions that could lead to errors in "
                                                  "PRISM.\n");
    }

    static inline std::pair<Type, QString>
    restrictedRecoveryReachableFromEnv(const std::string& node)
    {
        return std::pair<Type, QString>(Type::WARNING,
                                        "Restricted recovery strategy was activated for node "
                                                + QString::fromStdString(node)
                                                + ", but node is directly connected to Env! "
                                                  "Will attempt to use general recovery "
                                                  "strategy instead!\n");
    }
};

}  // namespace widgets

#endif /* ERIS_WIDGETS_ERRORS_H */
