/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
 *
 * Authors:
 *      - 2020-2021  Rhea Rinaldo (Rhea@Rinaldo.biz)
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

#ifndef ERIS_GRAPH_FILE_MANAGER_H
#define ERIS_GRAPH_FILE_MANAGER_H

#include "node_settings_validator.h"

#include <QString>

QT_BEGIN_NAMESPACE
class QColor;
class QFile;
class QPrinter;
class QResource;
class QSvgGenerator;
class QXmlStreamAttribute;
class QXmlStreamReader;
class QXmlStreamWriter;
QT_END_NAMESPACE

using namespace graph;

namespace utils
{
class Model;
class GraphicScene;

/**
 * Manager that handles read and write actions on files to store and load scenes from/to XML.
 * @param scene
 */
class FileManager
{
    
public:
    FileManager(GraphicScene* scene);

    ~FileManager();

    enum class Extension
    {
        pdf = 1,
        png = 2,
        svg = 3,
    };

    /**
     * Reads the file given by filename and builds the graphic scene according to it.
     * @param fileName input file
     * @param passive load state: if true submodules will not be opened
     * @return false if the file could not be read, true otherwise
     */
    bool
    read(QString fileName, bool passive);

    /**
     * Writes the current scene into a file provided by filename.
     * @param fileName input file
     * @param redundancyDefinition currently set redundancy definition
     * @return
     */
    bool
    write(const QString& fileName, const QString& redundancyDefinition);

    /**
     * Exports the current scene to a file fileName and the given extension.
     * @param extension
     * @param fileName
     */
    void
    exportAs(Extension extension, QString fileName);

    /**
     * Returns the loaded redundancy definition.
     * @return
     */
    std::string
    getRedundancyDefinition();

private:
    /**
     * Helper function that writes properties to the file.
     * @param xmlWriter
     */
    void
    writeProperties(QXmlStreamWriter* xmlWriter);

    /**
     * Helper function that writes all node items to the file.
     * @param xmlWriter
     */
    void
    writeNodes(QXmlStreamWriter* xmlWriter);

    /**
     * Helper function that writes all edge items to the file.
     * @param xmlWriter
     */
    void
    writeEdges(QXmlStreamWriter* xmlWriter);

    /**
     * Helper function that writes all text items to the file.
     * @param xmlWriter
     */
    void
    writeTexts(QXmlStreamWriter* xmlWriter);

    /**
     * Helper function that writes the options to the file.
     * @param xmlWriter
     */
    void
    writeOptions(QXmlStreamWriter* xmlWriter, const QString& redundancyDefinition);

    /**
     * Helper function that reads all the properties and from the file.
     * @param xmlWriter
     */
    void
    readProperties(QXmlStreamReader* xmlReader);

    /**
     * Helper function that reads all node items from the file and adds them to the scene.
     * @param xmlWriter
     */
    void
    readNodes(QXmlStreamReader* xmlReader,
              std::map<NodeItem*, std::string>* essentials,
              std::map<NodeItem*, std::pair<std::string, std::string>>* customs,
              bool passive);

    /**
     * Helper function that reads all edge items from the file and adds them to the scene.
     * @param xmlWriter
     */
    void
    readEdges(QXmlStreamReader* xmlReader);

    /**
     * Helper function that reads all text items from the file and adds them to the scene.
     * @param xmlWriter
     */
    void
    readTexts(QXmlStreamReader* xmlReader);

    /**
     * Helper function that reads all properties from the file and sets them accordingly.
     * @param xmlWriter
     */
    void
    readOptions(QXmlStreamReader* xmlReader);

    /** Pointer to the GraphicScene */
    GraphicScene* mScene;

    /** Redundancy definition to store freshly loaded definitions */
    std::string mRedundancy;
};

}  // namespace graph

#endif /* ERIS_GRAPH_FILE_MANAGER_H */
