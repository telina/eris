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

#ifndef ERIS_GRAPH_INTERNAL_COUNTER_H
#define ERIS_GRAPH_INTERNAL_COUNTER_H

#include <set>
#include <memory>
namespace graph
{
class Counter
{
public:
    static std::unique_ptr<Counter>
    Create();

    Counter(Counter const&) = delete;
    void
    operator=(Counter const&) = delete;

    /**
     * Get the next available ID.
     * @return ID
     */
    unsigned int
    getNext();

    /**
     * Frees the provided ID, making it available for another node (usually called when a node is
     * deleted).
     * @param id, the ID that shall be freed
     */
    void
    free(unsigned int id);

    /**
     * Updates the old ID of a node with the provided new ID. Note: isAvailable should be called
     * before, otherwise this might lead to the setting of unavailable IDs!
     * @param oldId the current ID of the node
     * @param newId the desired ID of the node
     */
    bool
    update(unsigned int oldId, unsigned int newId);

    /**
     * Resets the incrementing counter and clears the ID containers. This function is usually called
     * when a new File is opened or loaded.
     */
    void
    clear();

    /**
     * Checks whether the given ID is available. If so, the Counter is updated and true is returned.
     * The counter remains unchanged and false is returned.
     * @param id
     * @return
     */
    bool
    setId(unsigned int id);

private:
    /** Initialise counter object */
    Counter();

    /** Set of IDs that were previously occupied and are currently freed */
    std::set<unsigned int> mFreedIds;

    /** Set of IDs that are currently occupied */
    std::set<unsigned int> mOccupiedIds;

    /** Incrementing integer used to get the next ID */
    unsigned int mIncrementor;
};

}  // namespace graph

#endif /* ERIS_GRAPH_INTERNAL_COUNTER_H */
