/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
 *      - 2020-2021 Rhea Rinaldo (Rhea@Odlanir.de)
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

#ifndef ERIS_WIDGETS_NODE_SETTINGS_VALIDATOR_H
#define ERIS_WIDGETS_NODE_SETTINGS_VALIDATOR_H

#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <QString>
#include <regex>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
QT_END_NAMESPACE

#include "recovery_strategy.h"
#include "model.h"

namespace graph
{
class NodeItem;
class GraphicScene;
class Counter;

}  // namespace graph
namespace widgets
{
    class ErrorHandler;
}

namespace utils
{
using graph::Counter;
using graph::GraphicScene;
using graph::Model;
using graph::NodeItem;
using graph::Recovery;

/**
 * Checks whether the given probability or rate is valid.
 * Probability: Expects a double with a dot as a comma and the value probability must be higher
 * or equal to 0 and lower or equal 1.
 * Rate: must be a digit
 * If so, true is returned, otherwise a dedicated error is set in the error handler and
 * false is returned.
 * @param value (rate or probability) input as string
 * @return, true if valid rate or probability, false otherwise
 */
bool
isValidInputValue(const std::string& value, const QString& valueType, Model::Type modelType);

/**
 * Checks whether the ID that shall be set as the new ID is syntactically correct (integer
 * within bounds) and whether it is still available.
 * If so, the new ID is set and the current ID is freed, otherwise a dedicated error is
 * set in the error handler and false is returned.
 * @param newId ID that shall be set as the new ID
 * @param currId currently used ID
 * @return true if the ID is valid and could be set, false otherwise
 */
bool
isValidNewId(const std::string& newId, const std::string& currId, Counter* counter);

/**
 * Checks whether the input ID is syntactically correct (integer within bounds) and whether it
 * is still available. If so, the ID is set and true is returned, otherwise a dedicated error is
 * set in the error handler and false is returned.
 * @param id the ID that shall be set
 * @return true if ID was valid and could be set, false otherwise
 */
bool
isValidId(const std::string& id, Counter* counter);
/**
 * Parses the input string of essential nodes and checks whether it is syntactically
 * correct and if the defined nodes provide functionality to the given node.
 * If not, a dedicated error is set in the error handler.
 * @param essentialNodes (user) input string of essential nodes
 * @param nodeItem the nodeitem the essential nodes are defined for
 * @return true if input is valid, false otherwise
 */
bool
isValidEssentialNodes(std::string essentialNodes, NodeItem* nodeItem);

/**
 * Parses the input string of the recovery formula and checks whether it is syntactically
 * correct and if the defined nodes exist.
 * If not, a dedicated error is set in the error handler.
 * @param essentialNodes (user) input string of essential nodes
 * @param nodeItem the nodeitem the essential nodes are defined for
 * @return true if input is valid, false otherwise
 */
bool
isValidRecoveryFormula(std::string recoveryFormula, QGraphicsScene* scene);

/**
 * Parses the input string of redundant nodes and checks whether it is syntactically
 * correct and if the defined nodes exist.
 * If not, a dedicated error is set in the error handler.
 * @param redundancy (user) input string of redundancy defintion
 * @param scene reference to the graphic scene to access existing nodeitems
 * @return true if input is valid, false otherwise
 */
bool
isValidRedundancyDefinition(std::string redundancy, GraphicScene* scene);

/**
 * Parses the input string of the component type and checks whether it is syntactically
 * correct and within bounds.
 * If not, a dedicated error is set in the error handler.
 * @param type component type (digit) as string
 * @return true if input is valid, false otherwise
 */
bool
isValidComponentType(const std::string& type, unsigned int nodeItemId);

/**
 * Parses the input string of the recovery flag and checks whether it is syntactically
 * correct and can be represented by a boolean value.
 * If not, a dedicated error is set in the error handler.
 * @param recovery flag as a string
 * @return true if input is valid, false otherwise
 */
bool
isValidRecoveryFlag(const std::string& recoveryflag, unsigned int nodeItemId);

/**
 * Parses the input string of the recovery flag and checks whether it is syntactically
 * correct and can be represented by a boolean value.
 * If not, a dedicated error is set in the error handler.
 * @param recovery flag as a string
 * @return true if input is valid, false otherwise
 */
bool
isValidRecoveryStrategy(const QString& strategy, unsigned int nodeItemId);

/**
 * Parses the input string of the simulation flag and checks whether it is syntactically
 * correct and can be represented by a boolean value.
 * If not, a dedicated error is set in the error handler.
 * @param simulation flag as a string
 * @return true if input is valid, false otherwise
 */
bool
isValidSimulationFlag(const std::string& simulationFlag, unsigned int nodeItemId);

/**
 * Parses the input string of the subgraph flag and checks whether it is syntactically
 * correct and can be represented by a boolean value.
 * If not, a dedicated error is set in the error handler.
 * @param submoduleFlag as a string
 * @return true if input is valid, false otherwise
 */
bool
isValidSubmoduleFlag(const std::string& submoduleFlag, unsigned int nodeItemId);

bool
isValidSimulationPath(const QString& path, unsigned int nodeItemId);

bool
isValidSubmodulePath(const QString& path, unsigned int nodeItemId);

}  // namespace widgets

#endif /* ERIS_WIDGETS_NODE_SETTINGS_VALIDATOR_H */
