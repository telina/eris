/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
 *
 * Authors:
 *      - 2020-2021 Rhea Rinaldo (Rhea@Rinaldo.biz)
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

#include "node_settings_validator.h"

#include "error_handler.h"

#include "counter.h"
#include "graphic_scene.h"
#include "error_handler.h"
#include "errors.h"
#include "node_item.h"
#include "string_utils.h"
#include <QGraphicsScene>
#include <QFileInfo>
#include <algorithm>
#include <cctype>
#include <string>

using namespace widgets;

namespace utils
{
bool
isValidInputValue(const std::string& value, const QString& valueType, Model::Type modelType)
{
    setlocale(LC_ALL, "C");  // point as decimal separator
    ErrorHandler& errorHandler = ErrorHandler::getInstance();
    std::regex reg("^[0-9]+(\\.[0-9]+)?(e-?[0-9]+)?$");

    if (std::regex_match(value, reg))
    {  // input is syntactically correct, check whether it is in bounds!

        if (modelType == Model::CTMC)
        {
            return true;
        }
        else
        {
            double num = std::stod(value);
            if (num < 0 || num > 1)
            {
                errorHandler.setError(Errors::inputValueOutOfBounds(valueType));
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    else
    {
        errorHandler.setError(Errors::inputValueSyntaxError(valueType));
        return false;
    }
}

bool
isValidNewId(const std::string& newId, const std::string& currId, Counter* counter)
{
    ErrorHandler& errorHandler = ErrorHandler::getInstance();
    std::regex reg("\\d+");

    if (std::regex_match(newId, reg))
    {
        unsigned int numNewId = std::stoi(newId);
        unsigned int numCurrId = std::stoi(currId);

        if (numNewId > INT_MAX)  // unsinged int can't be negative
        {                        // Out of bounds
            errorHandler.setError(Errors::iDOutOfBounds());
            return false;
        }
        else if (numNewId == numCurrId)
        {
            return true;
        }
        else if (counter->update(numCurrId, numNewId))
        {
            return true;
        }
        else
        {
            errorHandler.setError(Errors::idUnavailable());
            return false;
        }
    }
    else
    {
        errorHandler.setError(Errors::iDInvalid(QString::fromStdString(newId)));
        return false;
    }
}

bool
isValidId(const std::string& id, Counter* counter)
{
    ErrorHandler& errorHandler = ErrorHandler::getInstance();
    std::regex reg("\\d+");

    if (std::regex_match(id, reg))
    {  // input is syntactically correct, check whether it is in bounds!
        unsigned int numId = std::stoi(id);

        if (numId > INT_MAX)  // unsinged int can't be negative
        {                     // Out of bounds
            errorHandler.setError(Errors::iDOutOfBounds());
            return false;
        }
        else if (counter->setId(numId))
        {
            return true;
        }
        else
        {
            errorHandler.setError(Errors::idUnavailable());
            return false;
        }
    }
    else
    {
        errorHandler.setError(Errors::iDInvalid(QString::fromStdString(id)));
        return false;
    }
}

bool
isValidEssentialNodes(std::string essentialNodes, NodeItem* nodeItem)
{
    ErrorHandler& errorHandler = ErrorHandler::getInstance();
    if (!essentialNodes.empty())
    {
        std::regex reg("^((\\(?n|N)[0-9][0-9]*[ ]*=[ ]*[0-2])[ ]*\\)?[ ]*([|&][ ]*"
                       "(\\(?n|N)[0-9][0-9]*[ ]*=[ ]*[0-2]\\)?[ ]*)*");
        if (!std::regex_match(essentialNodes, reg))
        {  // input is syntactically incorrect
            errorHandler.setError(Errors::essentialNodeSyntacticallyIncorrect(nodeItem->getId()));
            return false;
        }
        // at this point the string is syntactically correct so we can only view the number
        std::regex elem("(n[0-9][0-9]*)");
        std::sregex_iterator iter(essentialNodes.begin(), essentialNodes.end(), elem);
        std::sregex_iterator end;
        std::string id;
        while (iter != end)
        {
            id = std::string((*iter)[1]).substr(1);
            if (!nodeItem->providesFunctionality(std::stoi(id)))
            {  // if no functional edge from the node exists, the expression is invalid/useless
                errorHandler.setError(Errors::essentialNodeDoesNotProvideFunctionality());
                return false;
            }
            ++iter;
        }
    }
    return true;
}

bool
isValidRecoveryFormula(std::string recoveryFormula, QGraphicsScene* scene)
{
    ErrorHandler& errorHandler = ErrorHandler::getInstance();
    if (!recoveryFormula.empty())
    {
        std::regex reg("^((\\(?n|N)[0-9][0-9]*[ ]*=[ ]*[0-2])[ ]*\\)?[ ]*([|&][ ]*"
                       "(\\(?n|N)[0-9][0-9]*[ ]*=[ ]*[0-2]\\)?[ ]*)*");
        if (!std::regex_match(recoveryFormula, reg))
        {  // input is syntactically incorrect
            errorHandler.setError(Errors::recoveryFormulaSyntacticallyIncorrect());
            return false;
        }
        // at this point the string is syntactically correct so we can only view the number
        std::regex elem("(n[0-9][0-9]*)");
        std::sregex_iterator iter(recoveryFormula.begin(), recoveryFormula.end(), elem);
        std::sregex_iterator end;
        unsigned int id;
        bool exists = false;
        while (iter != end)
        {
            id = stoi(std::string((*iter)[1]).substr(1));
            for (QGraphicsItem* item : scene->items())
            {
                if (item->type() == NodeItem::Type)
                {
                    NodeItem* node = qgraphicsitem_cast<NodeItem*>(item);
                    if (node->getId() == id)
                    {
                        exists = true;
                    }
                }
            }
            if (!exists)
            {  // one or more nodes could not be found in the scene
                errorHandler.setError(Errors::customRecoveryNodeDoesNotExist(id));
                return false;
            }
            exists = false;
            ++iter;
        }
    }
    return true;
}

bool
isValidRedundancyDefinition(std::string redundancy, GraphicScene* scene)
{
    // regex that allows multiple nodes at once: "^([ ]*(n|N)[0-9][0-9]*[ ]*=[ ]*(n|N)[0-9][0-9]*[
    // ]*)([=[ ]*(n|N)[0-9][0-9]*[ ]*|,[ ])*" Allows: n1=n2, n3= n4 .... disallows n3=n2=n1
    std::regex reg("^([ ]*(n|N)[0-9][0-9]*[ ]*=[ ]*(n|N)[0-9][0-9]*[ ]*)(,[ ]*(n|N)[0-9][0-9]*"
                   "[ ]*=[ ]*(n|N)[0-9][0-9]*[ ]*)*");
    if (std::regex_match(redundancy, reg))
    {
        std::regex elem("(n[0-9][0-9]*)");  // match nodes
        std::sregex_iterator iter(redundancy.begin(), redundancy.end(), elem);
        std::sregex_iterator end;
        while (iter != end)
        {
            unsigned int id = std::stoi(std::string((*iter)[1]).substr(1));
            if (scene->nodeExists(id))
            {
                NodeItem* nodeItem;
                scene->getNodeItemById(id, &nodeItem, true);
                if (nodeItem->isCritical())
                {
                    ++iter;
                }
                else
                {
                    ErrorHandler::getInstance().clear();
                    ErrorHandler::getInstance().setError(Errors::nonCriticalNodeRedundant());
                    ErrorHandler::getInstance().show();
                    return false;
                }
            }
            else
            {  // Node does not exist!
                ErrorHandler::getInstance().setError(Errors::noNodeWithId(id));
                ErrorHandler::getInstance().show();
                return false;
            }
        }
    }
    else
    {
        ErrorHandler::getInstance().setError(Errors::redundancySyntacticallyIncorrect());
        ErrorHandler::getInstance().show();
        return false;
    }
    return true;  // If we made it this far, the input must be fine
}

bool
isValidComponentType(const std::string& type, unsigned int nodeItemId)
{
    ErrorHandler& errorHandler = ErrorHandler::getInstance();
    if (!type.empty())
    {
        std::regex reg("^[1-6]");
        if (!std::regex_match(type, reg))
        {  // input is syntactically incorrect
            errorHandler.setError(Errors::componentTypeOutOfBounds(nodeItemId));
            return false;
        }
    }
    else
    {
        errorHandler.setError(Errors::componentTypeUndefined(nodeItemId));
    }
    return true;
}

bool
isValidRecoveryFlag(const std::string& recoveryflag, unsigned int nodeItemId)
{
    ErrorHandler& errorHandler = ErrorHandler::getInstance();
    if (!recoveryflag.empty())
    {
        std::regex reg("^((true)|(false))$");
        if (!std::regex_match(recoveryflag, reg))
        {  // input is syntactically incorrect
            errorHandler.setError(Errors::invalidRecoveryFlagInput(nodeItemId));
            return false;
        }
    }
    else
    {
        errorHandler.setError(Errors::recoveryFlagUndefined(nodeItemId));
    }
    return true;
}

bool
isValidRecoveryStrategy(const QString& strategy, unsigned int nodeItemId)
{
    if (!strategy.isEmpty())
    {
        Recovery::Strategy convertedStrategy = Recovery::fromString(strategy);
        if (convertedStrategy != Recovery::Strategy::undefined)
        {  // input is valid
            return true;
        }
    }
    ErrorHandler::getInstance().setError(Errors::recoveryStrategyUndefined(nodeItemId));
    return false;
}

bool
isValidSimulationFlag(const std::string& simulationFlag, unsigned int nodeItemId)
{
    ErrorHandler& errorHandler = ErrorHandler::getInstance();
    if (!simulationFlag.empty())
    {
        std::regex reg("^((true)|(false))$");
        if (!std::regex_match(simulationFlag, reg))
        {  // input is syntactically incorrect
            errorHandler.setError(Errors::invalidSimulationFlagInput(nodeItemId));
            return false;
        }
    }
    else
    {
        errorHandler.setError(Errors::simulationFlagUndefined(nodeItemId));
    }
    return true;
}
bool
isValidSubmoduleFlag(const std::string& subgraphFlag, unsigned int nodeItemId)
{
    ErrorHandler& errorHandler = ErrorHandler::getInstance();
    if (!subgraphFlag.empty())
    {
        std::string flag = utils::ToLowerASCII(subgraphFlag);

        std::regex reg("^((true)|(false))$");
        if (!std::regex_match(flag, reg))
        {  // input is syntactically incorrect
            errorHandler.setError(Errors::InvalidSubmobuleFlagInput(nodeItemId));
            return false;
        }
    }
    else
    {
        errorHandler.setError(Errors::SubmoduleFlagUndefined(nodeItemId));
    }
    return true;
}

bool
isValidSimulationPath(const QString& path, unsigned int nodeItemId)
{
    ErrorHandler& errorHandler = ErrorHandler::getInstance();
    if (path.isEmpty())
    {
        errorHandler.setError(Errors::simulationPathNotSet(nodeItemId));
        return false;
    }
    if (QFileInfo::exists(path))
    {
        return true;
    }
    errorHandler.setError(Errors::simulationPathIncorrect(nodeItemId));
    return false;
}

bool
isValidSubmodulePath(const QString& path, unsigned int nodeItemId)
{
    ErrorHandler& errorHandler = ErrorHandler::getInstance();
    if (path.isEmpty())
    {
        errorHandler.setError(Errors::submodulePathNotSet(nodeItemId));
        return false;
    }
    if (QFileInfo::exists(path))
    {
        return true;
    }
    errorHandler.setError(Errors::submodulePathIncorrect(path, nodeItemId));
    return false;
}

}  // namespace widgets