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


#include "task.h"

#include "main_window.h"
#include "node_item.h"
#include "counter.h"
#include "file_manager.h"
#include "scene_status.h"
#include "prism.h"
#include "transformer.h"
#include "evaluation_settings.h"
#include "error_handler.h"
#include "graphic_scene.h"
#include "command.h"
#include "edge_item.h"
#include "text_item.h"
#include "checks.h"
#include "transcriber.h"
#include "graphics_view.h"
#include "main_window_manager.h"

#include <QToolButton>
#include <QToolBar>
#include <QLabel>
#include <QButtonGroup>
#include <QToolBox>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QInputDialog>
#include <QFileDialog>
#include <QApplication>
#include <QCloseEvent>

namespace widgets
{

using eval::Prism;
using graph::Counter;
using graph::NodeItem;
using utils::FileManager;

void
MainWindow::exitActTriggered()
{
    close();
}

void
MainWindow::createActions()
{
    // Avoid running Prism multiple times
   /* connect(eval::Prism::getInstance(),
            &eval::Prism::DoneWorking,
            MainWindowActionsManager::getInstance()->mStartVerificationAct,
            &QAction::setEnabled); */

    // mDeleteAct = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
}

void
MainWindow::optionModelCTMCActTriggered()
{
    MainWindowManager::getInstance()->setModelType(graph::Model::CTMC);
}

void
MainWindow::optionModelMDPActTriggered()
{
    MainWindowManager::getInstance()->setModelType(graph::Model::MDP);
}

void
MainWindow::optionOptimizedModeActTriggered()
{
    graphInternal::Operational::getInstance().setMode(graphInternal::Operational::Mode::optimized);
}

void
MainWindow::optionSimpleModeActTriggered()
{
    graphInternal::Operational::getInstance().setMode(graphInternal::Operational::Mode::simple);
}

void
MainWindow::newFileActTriggered()
{
    MainWindowManager::getInstance()->createNewSession();
}

void
MainWindow::closeEvent(QCloseEvent* event)
{
    if (eval::Prism::getInstance()->isRunning())
    {
        if (!eval::Prism::getInstance()->askForAbort())
        {
            event->ignore();
            return;
        }
    }

    event->accept();
}

void
MainWindow::evaluationSettingsActTriggered()
{
    EvaluationSettingsDialog::Get()->show();
}

void
MainWindow::startVerificationActTriggered()
{
    if (!MainWindowManager::getInstance()->startVerification())
    {
        ErrorHandler::getInstance().showErrorCollection();
    }
}

void
MainWindow::transformActTriggered()
{
    if (!MainWindowManager::getInstance()->transform())
    {
        ErrorHandler::getInstance().showErrorCollection();
    }
}

void
MainWindow::experimentActTriggered()
{
    if (!MainWindowManager::getInstance()->evaluate())
    {
        ErrorHandler::getInstance().showErrorCollection();
    }
}

void
MainWindow::itemSettingsActTriggered()
{
    MainWindowManager::getInstance()->openItemSettings();
}

void
MainWindow::nodeSwapCriticalityActTriggered()
{
    MainWindowManager::getInstance()->swapNodeCriticality();
}

void
MainWindow::nodeSwapIdActTriggered()
{
    MainWindowManager::getInstance()->swapNodeId();
}

void
MainWindow::openFileActTriggered()
{
    MainWindowManager::getInstance()->openFile();
}
void
MainWindow::optionAddRedundancyActTriggered()
{
    MainWindowManager::getInstance()->optionAddRedundancy();
}
void
MainWindow::setOutfileNameActTriggered()
{
    MainWindowManager::getInstance()->setOutFileName();
}
void
MainWindow::exportAsPngActTriggered()
{
    MainWindowManager::getInstance()->exportAs(utils::FileManager::Extension::png);
}
void
MainWindow::exportAsPdfActTriggered()
{
    MainWindowManager::getInstance()->exportAs(utils::FileManager::Extension::pdf);
}
void
MainWindow::exportAsSvgActTriggered()
{
    MainWindowManager::getInstance()->exportAs(utils::FileManager::Extension::svg);
}
void
MainWindow::saveFileActTriggered()
{
    MainWindowManager::getInstance()->save();
}
void
MainWindow::saveFileAsActTriggered()
{
    MainWindowManager::getInstance()->saveAs();
}

void
MainWindow::deleteItemActTriggered()
{
    MainWindowManager::getInstance()->deleteItem();
}

void
MainWindow::showHelpWindow()
{
    MainWindowManager::getInstance()->showHelpWindow();
}

void
MainWindow::optionRateInterpretationToggled(graph::RateInterpretation rateInt)
{
    GRAPHIC_SCENE_FACTORY()->current()->updateRateInterpretation(rateInt);
}

void
MainWindow::ButtonsGroupButtonClicked(int id)
{
    MainWindowManager::getInstance()->setNodeMode(id);
}
void
MainWindow::AnnotationGroupButtonClicked(int id)
{
    MainWindowManager::getInstance()->setAnnotationMode(id);
}

}  // namespace widgets
