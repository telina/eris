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

#include "counter.h"

namespace graph
{
    
unsigned int
Counter::getNext()
{  // use is occupied logic
    unsigned int ret = 0;
    if (!mFreedIds.empty())
    {
        ret = *mFreedIds.begin();
        mFreedIds.erase(ret);
    }
    else
    {  // take next ID -> check whether this ID has not already been occupied

        if (!mOccupiedIds.empty())
        {  // TODO hier ist irgendwo ein Fehler
            while (mOccupiedIds.find(mIncrementor) != mOccupiedIds.end())
            {  // already taken -> increment until free ID is found

                mOccupiedIds.erase(mIncrementor);
                mIncrementor++;
            }
            ret = mIncrementor;
            mIncrementor++;  // Incrementor must always point to the next free position
        }
        else
        {
            ret = mIncrementor;
            mIncrementor++;
        }
    }
    return ret;
}

void
Counter::free(unsigned int id)
{
    if (id < mIncrementor)
    {  // Add current/old id to the freed IDs if its smaller then the incrementor
        mFreedIds.insert(id);
    }
    else
    {  // ID is bigger than the incrementor so its custom -> erase from the occupied IDs
        mOccupiedIds.erase(id);
    }
}

bool
Counter::update(unsigned int oldId, unsigned int newId)
{
    if (setId(newId))
    {
        free(oldId);
        return true;
    }
    else
    {
        return false;
    }
}

bool
Counter::setId(unsigned int id)
{
    bool ret = false;
    if (id < mIncrementor)
    {  // desired ID is smaller than the incrementor, check for IDs that have been freed
        if (!mFreedIds.empty())
        {
            if (mFreedIds.find(id) != mFreedIds.end())  // search for the ID
            {                                           // ID is in the freed IDs!
                mFreedIds.erase(id);
                ret = true;
            }
        }
    }
    else
    {  // check if its in the set of occupied custom IDs
        if (mOccupiedIds.find(id) == mOccupiedIds.end())
        {  // ID is available
            mOccupiedIds.insert(id);
            ret = true;
        }
    }
    return ret;
}

void
Counter::clear()
{
    mIncrementor = 0;
    mFreedIds.clear();
    mOccupiedIds.clear();
}

std::unique_ptr<Counter>
Counter::Create()
{
    std::unique_ptr<Counter> instance(new (std::nothrow) Counter());
    return instance;
}

Counter::Counter() : mIncrementor(0)
{
    
}

}  // namespace graph