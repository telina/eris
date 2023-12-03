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

#ifndef ERIS_GRAPH_OPERATIONAL_H
#define ERIS_GRAPH_OPERATIONAL_H

#include <QObject>

namespace graphInternal
{

class Operational
{

public:
    
    enum Mode
    {
        optimized = 0,
        simple = 1,
    };

    /**
     * Singleton
     * @param
     */
    Operational(Operational const&) = delete;
    void
    operator=(Operational const&) = delete;

    /**
     * Returns an instance of the counter (singleton)
     * @return counter instance
     */
    static Operational&
    getInstance()
    {
        static Operational instance;
        return instance;
    }

    /**
     * Sets the global mode of operation.
     * @param type
     */
    void
    setMode(Mode mode);

    /**
     * Returns the currently set mode of operation.
     * @return
     */
    Mode
    getMode();

private:

    Operational();

    Mode mMode;
    
};

}  // namespace graph


#endif /* ERIS_GRAPH_OPERATIONAL_H */

