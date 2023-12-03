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


#include "file_manager.h"
#include "node_settings.h"
#include "node_item.h"
#include "edge_item.h"
#include "text_item.h"
#include "model.h"
#include "main_window_manager.h"
#include "errors.h"
#include "error_handler.h"
#include "node_settings_validator.h"
#include "string_utils.h"
#include "file_manager_fields.h"

#include <QColor>
#include <QFile>
#include <QPrinter>
#include <QPainter>
#include <QResource>
#include <QSvgGenerator>
#include <QXmlStreamAttribute>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace utils
{
using widgets::ErrorHandler;
using widgets::Errors;

FileManager::FileManager(GraphicScene* scene) : mScene(scene)
{
    
}

FileManager::~FileManager()
{
    
}

void
FileManager::writeProperties(QXmlStreamWriter* xmlWriter)
{
    xmlWriter->writeStartDocument();
    xmlWriter->writeStartElement("SceneData");
    xmlWriter->writeAttribute("version", "v1.2");
    xmlWriter->writeStartElement(kProperties);
    xmlWriter->writeAttribute("Model", Model::getInstance().getTypeAsString());
    xmlWriter->writeAttribute("RateInterpretation", mScene->getCurrentRateInterpretation().toQString());
    xmlWriter->writeEndElement();
}

void
FileManager::writeNodes(QXmlStreamWriter* xmlWriter)
{
    xmlWriter->writeStartElement(kNodes);

    for (QGraphicsItem* item : mScene->items())
    {
        if (item->type() == NodeItem::Type)
        {
            NodeItem* nodeItem = qgraphicsitem_cast<NodeItem*>(item);
            xmlWriter->writeStartElement(kNode);
            if (nodeItem->isSimulationModule())
            {
                xmlWriter->writeAttribute(kSimulated, kTrue);
            }
            else
            {
                xmlWriter->writeAttribute(kSimulated, kFalse);
            }
            if (nodeItem->isErisModule())
            {
                xmlWriter->writeAttribute(kSubmodule, kTrue);
            }
            else
            {
                xmlWriter->writeAttribute(kSubmodule, kFalse);
            }
            xmlWriter->writeAttribute(kSubmodulePath, nodeItem->getSubmodulePath());

            xmlWriter->writeAttribute(kSimulationPath, nodeItem->getSimulationPath());
            xmlWriter->writeAttribute(kComponentType, QString::number(nodeItem->getComponentType()));

            xmlWriter->writeAttribute(kXcoord, QString::number(nodeItem->x()));
            xmlWriter->writeAttribute(kyCoord, QString::number(nodeItem->y()));
            xmlWriter->writeAttribute(kId, QString::number(nodeItem->getId()));
            xmlWriter->writeAttribute(kIntrusion, nodeItem->getIntrusionIndicator());
            xmlWriter->writeAttribute(kFailure, nodeItem->getFailureIndicator());
            xmlWriter->writeAttribute(kSecurity, nodeItem->getSecurityIndicator());

            if (nodeItem->isRecoverableFromDefect())
            {
                xmlWriter->writeAttribute(kRecoverableDef, kTrue);
            }
            else
            {
                xmlWriter->writeAttribute(kRecoverableDef, kFalse);
            }
            xmlWriter->writeAttribute(kDefRecIndicator,
                                      nodeItem->getDefectRecoveryIndicator());
            xmlWriter->writeAttribute(kDefRecStrategy,
                                      Recovery::toString(nodeItem->getDefectRecoveryStrategy()));
            xmlWriter->writeAttribute(kCustomDefRecFormula,
                                      nodeItem->getCustomDefectRecoveryFormula());

            if (nodeItem->isRecoverableFromCorruption())
            {
                xmlWriter->writeAttribute(kRecoverableCorr,kTrue);
            }
            else
            {
                xmlWriter->writeAttribute(kRecoverableCorr, kFalse);
            }

            xmlWriter->writeAttribute(kCorrRecIndicator,
                                      nodeItem->getCorruptionRecoveryIndicator());
            xmlWriter->writeAttribute(
                    kCorrRecStrategy,
                    Recovery::toString(nodeItem->getCorruptionRecoveryStrategy()));
            xmlWriter->writeAttribute(kCustomCorrRecFormula,
                                      nodeItem->getCustomCorruptionRecoveryFormula());

            xmlWriter->writeAttribute(kEssentialNodes, nodeItem->getEssentialNodes());
            xmlWriter->writeEndElement();  // mark end
        }
    }

    xmlWriter->writeEndElement();
}

void
FileManager::writeEdges(QXmlStreamWriter* xmlWriter)
{
    xmlWriter->writeStartElement(kEdges);
    for (QGraphicsItem* item : mScene->items())
    {
        if (item->type() == EdgeItem::Type)
        {
            EdgeItem* edgeItem = qgraphicsitem_cast<EdgeItem*>(item);
            xmlWriter->writeStartElement(kEdge);
            xmlWriter->writeAttribute(kComponentType, QString::number(edgeItem->getComponentType()));

            xmlWriter->writeAttribute(
                    kEdgeStart,
                    QString::number(qgraphicsitem_cast<NodeItem*>(edgeItem->startItem())->getId()));
            xmlWriter->writeAttribute(
                    kEdgeEnd,
                    QString::number(qgraphicsitem_cast<NodeItem*>(edgeItem->endItem())->getId()));
            xmlWriter->writeEndElement();  // end of MyGraphicsItem
        }
    }
    xmlWriter->writeEndElement();  // end of GraphicsItemList
}

void
FileManager::writeTexts(QXmlStreamWriter* xmlWriter)
{
    xmlWriter->writeStartElement(kTexts);
    for (QGraphicsItem* item : mScene->items())
    {
        if (item->type() == TextItem::Type)
        {
            TextItem* textItem = qgraphicsitem_cast<TextItem*>(item);
            xmlWriter->writeStartElement(kText);
            xmlWriter->writeAttribute(kData, textItem->toPlainText());
            xmlWriter->writeAttribute(kXcoord, QString::number(textItem->x()));
            xmlWriter->writeAttribute(kyCoord, QString::number(textItem->y()));
            xmlWriter->writeEndElement();  // end of MyGraphicsItem
        }
    }
}

void
FileManager::writeOptions(QXmlStreamWriter* xmlWriter, const QString& redundancyDefinition)
{
    xmlWriter->writeEndElement();
    xmlWriter->writeStartElement(kOptions);
    xmlWriter->writeStartElement(kDefinition);
    xmlWriter->writeAttribute(kRedundancy, redundancyDefinition);
    xmlWriter->writeEndElement();
    xmlWriter->writeEndElement();
    xmlWriter->writeEndElement();
}

bool
FileManager::write(const QString& fileName, const QString& redundancyDefinition)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        ErrorHandler::getInstance().setError(Errors::openFileError(fileName));
        return false;
    }
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);

    writeProperties(&xmlWriter);
    writeNodes(&xmlWriter);
    writeEdges(&xmlWriter);
    writeTexts(&xmlWriter);
    writeOptions(&xmlWriter, redundancyDefinition);
    file.close();

    return true;
}

void
FileManager::readProperties(QXmlStreamReader* xmlReader)
{
    if (GRAPHIC_SCENE_FACTORY()->size() <= 1)
    {
        QString modelType = xmlReader->attributes()[0].value().toString();

        if (modelType == Model::getInstance().getTypeAsString(Model::Type::CTMC))
        {
            Model::getInstance().setType(Model::CTMC);
        }
        else if (modelType == Model::getInstance().getTypeAsString(Model::Type::CTMC))
        {
            Model::getInstance().setType(Model::MDP);
        }
    }

    if (xmlReader->attributes().size() >= 2) // needed to support older format
    {
        QString rateInterpretation = xmlReader->attributes()[1].value().toString();
        mScene->setRateInterpretation(graph::RateInterpretation(rateInterpretation));
    }
}

void
FileManager::readNodes(QXmlStreamReader* xmlReader,
                       std::map<NodeItem*, std::string>* essentials,
                       std::map<NodeItem*, std::pair<std::string, std::string>>* customs,
                       bool passive)
{
    ComponentType type;
    qreal x, y = 0;
    unsigned int id = 0;
    while (!(xmlReader->tokenType() == QXmlStreamReader::EndElement
             && xmlReader->name() == kNodes))
    {
        xmlReader->readNext();
        if (xmlReader->tokenType() == QXmlStreamReader::StartElement && xmlReader->name() == kNode)
        {
            QString curr = xmlReader->attributes().value(kId).toString();
            if (utils::isValidId(curr.toStdString(), mScene->getCounter()))
            {
                id = curr.toUInt();
            }
            else
            {
                ErrorHandler::getInstance().show();
                continue;
            }
            curr = xmlReader->attributes().value(kComponentType).toString();
            if (utils::isValidComponentType(curr.toStdString(), id))
            {
                type = ComponentType(curr.toUInt());
            }
            else
            {
                ErrorHandler::getInstance().show();
                continue;
            }

            x = xmlReader->attributes().value(kXcoord).toFloat();
            y = xmlReader->attributes().value(kyCoord).toFloat();

            NodeItem* nodeItem;
            if (mScene->getNodeItemById(id, &nodeItem))
            {
                ErrorHandler::getInstance().show();
                continue;
            }
            nodeItem = new NodeItem(type, id, QPointF(x, y), nullptr, mScene);

            curr = xmlReader->attributes().value(kSubmodule).toString();
            if (utils::isValidSubmoduleFlag(curr.toStdString(), id))
            {
                if (curr == kTrue)
                {
                    curr = xmlReader->attributes().value(kSubmodulePath).toString();
                    if (utils::isValidSubmodulePath(curr, id))
                    {
                        nodeItem->setSubmodule(true, curr);
                        if (!passive) // load the submodule in the GUI
                        {
                            widgets::MainWindowManager::getInstance()->openSubmodule(curr.toStdString());
                        }
                    }
                    else
                    {
                        ErrorHandler::getInstance().show();
                    }
                }
                else
                {
                    curr = xmlReader->attributes().value(kSubmodulePath).toString();
                    if (!curr.isEmpty())
                    {
                        if (utils::isValidSubmodulePath(curr, id))
                        {
                            nodeItem->setSubmodule(false, curr);
                        }
                        else
                        {
                            ErrorHandler::getInstance().show();
                        }
                    }
                }
            }
            else
            {
                ErrorHandler::getInstance().show();
            }

            curr = xmlReader->attributes().value(kSimulated).toString();
            if (utils::isValidSimulationFlag(curr.toStdString(), id))
            {
                if (curr == kTrue)
                {
                    curr = xmlReader->attributes().value(kSimulationPath).toString();
                    if (utils::isValidSimulationPath(curr, id))
                    {
                        nodeItem->setSimulated(true, curr);
                    }
                    else
                    {
                        ErrorHandler::getInstance().show();
                    }
                }
                else
                {
                    curr = xmlReader->attributes().value(kSimulationPath).toString();
                    if (!curr.isEmpty())
                    {
                        if (utils::isValidSimulationPath(curr, id))
                        {
                            nodeItem->setSimulated(false, curr);
                        }
                        else
                        {
                            ErrorHandler::getInstance().show();
                        }
                    }
                }
            }
            else
            {
                ErrorHandler::getInstance().show();
            }

            // add error handling : check if
            curr = xmlReader->attributes().value(kIntrusion).toString();  // todo change
            // to indicator
            if (curr.isEmpty())
            {
                nodeItem->setIntrusionIndicator(kDefaultZero);
            }
            else if (utils::isValidInputValue(
                             curr.toStdString(), "Intrusion", Model::getInstance().getType()))
            {
                nodeItem->setIntrusionIndicator(curr);
            }
            else
            {
                ErrorHandler::getInstance().show();
            }

            curr = xmlReader->attributes().value(kFailure).toString();
            if (curr.isEmpty())
            {
                nodeItem->setFailureIndicator(kDefaultZero);
            }
            else if (utils::isValidInputValue(
                             curr.toStdString(), "Failure", Model::getInstance().getType()))
            {
                nodeItem->setFailureIndicator(curr);
            }
            else
            {
                ErrorHandler::getInstance().show();
            }

            curr = xmlReader->attributes().value(kSecurity).toString();
            if (curr.isEmpty())
            {
                nodeItem->setSecurityIndicator(kDefaultZero);
            }
            else if (utils::isValidInputValue(
                             curr.toStdString(), "Security", Model::getInstance().getType()))
            {
                nodeItem->setSecurityIndicator(curr);
            }
            else
            {
                ErrorHandler::getInstance().show();
            }
            curr = xmlReader->attributes().value(kRecoverableDef).toString();
            if (utils::isValidRecoveryFlag(curr.toStdString(), id))
            {
                if (curr == kTrue)
                {
                    nodeItem->recoverableFromDefect(true);
                }
                else
                {
                    nodeItem->recoverableFromDefect(false);
                }
            }
            else
            {
                ErrorHandler::getInstance().show();
            }
            curr = xmlReader->attributes().value(kDefRecIndicator).toString();
            if (curr.isEmpty())
            {
                nodeItem->setDefectRecoveryIndicator(kDefaultZero);
            }
            else if (utils::isValidInputValue(
                             curr.toStdString(), "Defect Recovery", Model::getInstance().getType()))
            {
                nodeItem->setDefectRecoveryIndicator(curr);
            }
            else
            {
                ErrorHandler::getInstance().show();
            }

            curr = xmlReader->attributes().value(kDefRecStrategy).toString();
            if (utils::isValidRecoveryStrategy(curr, id))
            {
                nodeItem->setDefectRecoveryStrategy(Recovery::fromString(curr));
            }
            else
            {
                ErrorHandler::getInstance().show();
            }

            curr = xmlReader->attributes().value(kCustomDefRecFormula).toString();
            if (!curr.isEmpty())
            {
                if (customs->find(nodeItem) == customs->end())
                {  // key does not exists yet, make a new entry
                    std::pair<std::string, std::string> pair(curr.toStdString(),
                                                             "");  // other not known yet
                    customs->insert(std::pair<NodeItem*, std::pair<std::string, std::string>>(
                            nodeItem, pair));
                }
                else
                {  // key exists, only extend the existing data
                    customs->at(nodeItem).first = curr.toStdString();
                }
            }

            curr = xmlReader->attributes().value(kRecoverableCorr).toString();
            if (utils::isValidRecoveryFlag(curr.toStdString(), id))
            {
                if (curr == kTrue)
                {
                    nodeItem->recoverableFromCorruption(true);
                }
                else
                {
                    nodeItem->recoverableFromCorruption(false);
                }
            }
            else
            {
                ErrorHandler::getInstance().show();
            }

            curr = xmlReader->attributes().value(kCorrRecIndicator).toString();
            if (curr.isEmpty())
            {
                nodeItem->setCorruptionRecoveryIndicator(kDefaultZero);
            }
            else if (utils::isValidInputValue(curr.toStdString(),
                                                "Corruption Recovery",
                                                Model::getInstance().getType()))
            {
                nodeItem->setCorruptionRecoveryIndicator(curr);
            }
            else
            {
                ErrorHandler::getInstance().show();
            }

            curr = xmlReader->attributes().value(kCorrRecStrategy).toString();
            if (utils::isValidRecoveryStrategy(curr, id))
            {
                nodeItem->setCorruptionRecoveryStrategy(Recovery::fromString(curr));
            }
            else
            {
                ErrorHandler::getInstance().show();
            }

            curr = xmlReader->attributes().value(kCustomCorrRecFormula).toString();

            if (!curr.isEmpty())
            {
                if (customs->find(nodeItem) == customs->end())
                {  // key does not exists yet, make a new entry
                    std::pair<std::string, std::string> pair("", curr.toStdString());
                    customs->insert(std::pair<NodeItem*, std::pair<std::string, std::string>>(
                            nodeItem, pair));
                }
                else
                {  // key exists, only extend the existing data
                    customs->at(nodeItem).second = curr.toStdString();
                }
            }

            std::string en = xmlReader->attributes().value(kEssentialNodes).toUtf8().constData();
            if (!en.empty())
            {  // Can only be validated after ALL nodes and edges have been added
                essentials->insert(std::pair<NodeItem*, std::string>(nodeItem, en));
            }
            mScene->addItem(nodeItem);
        }
    }
}

void
FileManager::readEdges(QXmlStreamReader* xmlReader)
{
    NodeItem* startNode;
    NodeItem* endNode;
    unsigned int startId, endId = 0;
    while (!(xmlReader->tokenType() == QXmlStreamReader::EndElement
             && xmlReader->name() == kEdges))
    {
        xmlReader->readNext();

        if (xmlReader->tokenType() == QXmlStreamReader::StartElement && xmlReader->name() == kEdge)
        {
            ComponentType type = ComponentType(xmlReader->attributes().value(kComponentType).toUInt());
            startId = xmlReader->attributes().value(kEdgeStart).toUInt();
            endId = xmlReader->attributes().value(kEdgeEnd).toUInt();

            if (!mScene->getNodeItemById(startId, &startNode)
                || !mScene->getNodeItemById(endId, &endNode))
            {  // avoid dangling edges
                ErrorHandler::getInstance().show();
                continue;
            }
            if (startNode == endNode)
            {  // disallow self edges
                ErrorHandler::getInstance().setError(Errors::selfEdges());
                ErrorHandler::getInstance().show();
                continue;
            }
            if (startNode->edgeWithTypeExists(endNode, type))
            {  // disallow duplicates
                ErrorHandler::getInstance().show();
                continue;
            }
            EdgeItem* edgeItem = new EdgeItem(type, startNode, endNode);
            startNode->addEdgeItem(edgeItem);
            endNode->addEdgeItem(edgeItem);
            mScene->addItem(edgeItem);
            edgeItem->updatePosition();
            // gets edges of both directions!
            std::vector<EdgeItem*> collidingEdges = startNode->getCollidingEdgeItems(endNode);
            if (!collidingEdges.empty())  // collisions were found
            {
                // Edge in the opposite direction already exists, offset is needed
                // repaint/update colliding edge with offset
                mScene->updateCollidingEdges(collidingEdges);
            }
        }
    }
}

void
FileManager::readTexts(QXmlStreamReader* xmlReader)
{
    TextItem* textItem;
    QString text;
    qreal x, y = 0;
    while (!(xmlReader->tokenType() == QXmlStreamReader::EndElement
             && xmlReader->name() == kTexts))
    {
        xmlReader->readNext();

        if (xmlReader->tokenType() == QXmlStreamReader::StartElement && xmlReader->name() == kText)
        {
            text = xmlReader->attributes().value(kData).toString();
            x = xmlReader->attributes().value(kXcoord).toFloat();
            y = xmlReader->attributes().value(kyCoord).toFloat();
            textItem = new TextItem(text, QPointF(x, y));
            mScene->addItem(textItem);  // TODO sanity checks
        }
    }
}

void
FileManager::readOptions(QXmlStreamReader* xmlReader)
{
    while (!(xmlReader->tokenType() == QXmlStreamReader::EndElement
             && xmlReader->name() == kOptions))
    {
        xmlReader->readNext();
        if (xmlReader->tokenType() == QXmlStreamReader::StartElement
            && xmlReader->name() == kDefinition)
        {
            QString redundancy = xmlReader->attributes().value(kRedundancy).toString();
            if (redundancy.isEmpty())
            {
                mRedundancy.clear();
            }
            else if (utils::isValidRedundancyDefinition(redundancy.toStdString(), mScene))
            {
                mRedundancy = redundancy.toStdString();
            }
        }
    }
}

bool
FileManager::read(QString fileName, bool passive)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        ErrorHandler::getInstance().setError(Errors::openFileError(fileName));
        return false;
    }

    QXmlStreamReader xmlReader(&file);

    std::map<NodeItem*, std::string> essentials;
    std::map<NodeItem*, std::pair<std::string, std::string>> customs;

    // First read all nodes to avoid loading edges before nodes later
    while (!xmlReader.atEnd() && !xmlReader.hasError())
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == kProperties)
            {
                readProperties(&xmlReader);
                continue;
            }
            else if (xmlReader.name() == kNodes)
            {
                readNodes(&xmlReader, &essentials, &customs, passive);
            }
            else if (xmlReader.name() == kEdges)
            {
                readEdges(&xmlReader);
            }
            else if (xmlReader.name() == kTexts)
            {
                readTexts(&xmlReader);
            }
            else if (xmlReader.name() == kOptions)
            {
                readOptions(&xmlReader);
            }
        }
    }
    for (auto const& elem : essentials)  // so hässlich dass es schon wider schön ist
    {
        if (utils::isValidEssentialNodes(elem.second, elem.first))
        {
            elem.first->setEssentialNodes(QString::fromStdString(elem.second));
        }
        else
        {
            ErrorHandler::getInstance().show();
        }
    }
    for (auto const& elem : customs)
    {
        if (utils::isValidRecoveryFormula(elem.second.first, mScene))
        {
            elem.first->setCustomDefectRecoveryFormula(QString::fromStdString(elem.second.first));
        }
        else
        {
            ErrorHandler::getInstance().show();
        }

        if (utils::isValidRecoveryFormula(elem.second.second, mScene))
        {
            elem.first->setCustomCorruptionRecoveryFormula(
                    QString::fromStdString(elem.second.second));
        }
        else
        {
            ErrorHandler::getInstance().show();
        }
    }
    file.close();

    return true;
}

std::string
FileManager::getRedundancyDefinition()
{
    return mRedundancy;
}

void
FileManager::exportAs(Extension extension, QString fileName)
{
    switch (extension)
    {
        case Extension::pdf:
        {
            QPrinter printer(QPrinter::HighResolution);
            printer.setPageSize(QPageSize(QPageSize::PageSizeId::A4));
            //printer.setPaperSize(QPrinter::A4);
            printer.setPageOrientation(QPageLayout::Landscape);
            //printer.setOrientation(QPrinter::Landscape);
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setOutputFileName(fileName);

            QPainter painter(&printer);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setRenderHint(QPainter::SmoothPixmapTransform);
            mScene->render(&painter, printer.pageLayout().paintRectPixels(printer.resolution()), 
                    mScene->itemsBoundingRect().adjusted(-5, -5, 5, 5));
            break;
        }
        case Extension::png:
        {
            //  const QPixmap pixmap = mView->grab(mScene->itemsBoundingRect());
            //  pixmap.save(mExportName, "png", 100); // TODO remove borders/scrollbar before
            //  saving?
            QPainter painter;
            QBrush brush(Qt::white);
            QPixmap pixmap(mScene->itemsBoundingRect().x(), mScene->itemsBoundingRect().y());
            painter.begin(&pixmap);
            painter.fillRect(pixmap.rect(), brush);
            mScene->render(&painter, QRectF(pixmap.rect()), mScene->itemsBoundingRect().adjusted(-5, -5, 5, 5));
            painter.end();
            pixmap.save(fileName, "png", 100);
            break;
        }
        case Extension::svg:
        {
            QSvgGenerator generator;
            generator.setFileName(fileName);
            generator.setSize(
                    QSize(mScene->itemsBoundingRect().x(), mScene->itemsBoundingRect().y()));
            generator.setViewBox(mScene->itemsBoundingRect());
            generator.setTitle("SVG Generator");
            generator.setDescription("An SVG drawing created by the SVG Generator, QT (ERIS) ");

            QPainter painter(&generator);
            mScene->render(&painter, mScene->itemsBoundingRect(), mScene->itemsBoundingRect().adjusted(-5, -5, 5, 5));
            break;
        }
        default: break;
    }
}
}  // namespace graph
