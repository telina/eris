/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
 *      - 2021-2021 Mohammed Hussam Al Turjman (Mohammed_Hussam.Al_Turjman@dfki.de)
 *      - 2023  Rhea Rinaldo (Rhea@Odlanir.de)
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

#ifndef ERIS_FILE_MANAGER_FIELDS_H
#define ERIS_FILE_MANAGER_FIELDS_H

#include "eris_config.h"

namespace utils
{
    static const char kNodes[] = "Nodes";
    static const char kNode[] = "Node";
    static const char kEdges[] = "Edges";
    static const char kEdge[] = "Edge";
    static const char kEdgeStart[] = "StartNodeId";
    static const char kEdgeEnd[] = "EndNodeId";
    static const char kText[] = "Text";
    static const char kTexts[] = "Texts";
    static const char kData[] = "Data";
    static const char kOptions[] = "Options";
    static const char kDefinition[] = "Definition";
    static const char kRedundancy[] = "Redundancy";
    static const char kProperties[] = "Properties";
    static const char kSubmodule[] = "Submodule";
    static const char kSubmodulePath[] = "SubmodulePath";
    static const char kSimulated[] = "Simulated";
    static const char kSimulationPath[] = "SimulationPath";
    static const char kComponentType[] = "Type";
    static const char kId[] = "ID";
    static const char kXcoord[] = "xCoord";
    static const char kyCoord[] = "yCoord";
    static const char kIntrusion[] = "IntrusionIndicator";
    static const char kFailure[] = "FailureIndicator";
    static const char kSecurity[] = "SecurityIndicator";
    static const char kRecoverableDef[] = "RecoverableDefect";
    static const char kDefRecIndicator[] = "DefectRecoveryIndicator";
    static const char kDefRecStrategy[] = "DefectRecoveryStrategy";
    static const char kRecoverableCorr[] = "RecoverableCorruption";
    static const char kCorrRecIndicator[] = "CorruptionRecoveryIndicator";
    static const char kCorrRecStrategy[] = "CorruptionRecoveryStrategy";
    static const char kCustomDefRecFormula[] = "CustomDefectRecoveryFormula";
    static const char kCustomCorrRecFormula[] = "CustomCorruptionRecoveryFormula";
    static const char kEssentialNodes[] = "EssentialNodes";
    static const char kDefaultZero[] = "0";
    static const char kTrue[] = "true";
    static const char kFalse[] = "false";

}  // namespace graph

#endif  // ERIS_FILE_MANAGER_FIELDS_H
