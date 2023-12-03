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

#ifndef ERIS_GRAPH_SCENE_STATUS_H
#define ERIS_GRAPH_SCENE_STATUS_H

#include "eris_config.h"

namespace graph
{
// TODO Deprecate and remove.
//  It does not make sense for submodules to use a singleton.
class ERIS_EXPORT DeprecatedSceneStatus
{
public:
    DeprecatedSceneStatus(DeprecatedSceneStatus const&) = delete;
    void
    operator=(DeprecatedSceneStatus const&) = delete;

    static DeprecatedSceneStatus&
    getInstance()
    {
        static DeprecatedSceneStatus instance;
        return instance;
    }

    void
    setChanged(bool change);

    bool
    hasChanged();

private:
    DeprecatedSceneStatus()
    {
    }

    bool mTransformationStatus;
};

}  // namespace graph

#endif /* ERIS_GRAPH_SCENE_STATUS_H */
