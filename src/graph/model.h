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

#ifndef ERIS_GRAPH_MODEL_H
#define ERIS_GRAPH_MODEL_H

#include <QObject>

namespace graph
{
//
// FIXME: This is valid only if we dont have submodules -_-
//
class Model : public QObject
{
    Q_OBJECT

public:
    enum Type
    {
        MDP = 0,
        CTMC = 1,
    };

    /**
     * Singleton
     * @param
     */
    Model(Model const&) = delete;
    void
    operator=(Model const&) = delete;

    /**
     * Returns an instance of the counter (singleton)
     * @return counter instance
     */
    static Model&
    getInstance()
    {
        static Model instance;
        return instance;
    }

    /**
     * Sets the global model type.
     * @param type
     */
    void
    setType(Type type);

    /**
     * Returns the currently set type.
     * @return
     */
    Type
    getType();

    QString
    getTypeAsString(Type=Model::getInstance().getType()); // Geht das?
        

signals:

    void
    modelTypeUpdated();

private:
    /** Initialise counter object */
    Model() : mType(Type::CTMC) {};

    /** Incrementing integer used to get the next ID */
    Type mType;
};

}  // namespace graph

#endif /* ERIS_GRAPH_MODEL_H */
