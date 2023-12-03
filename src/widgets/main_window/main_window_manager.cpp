/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
 *      - 2020-2021 Rhea Rinaldo (Rhea@Odlanir.de)
 *      - 2021-2021 Mohammed Hussam Al Turjman (Mohammed_Hussam.Al_Turjman@dfki.de)
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

#include "main_window_manager.h"
#include "graphic_scene.h"
#include "main_window_actions_manager.h"
#include "main_window_buttons_group_manager.h"
#include "main_window_tool_buttons_manager.h"
#include "main_window.h"
#include "modules_tab_widget.h"
#include "checks.h"
#include "evaluation_tab.h"
#include "chart_view.h"
#include "error_handler.h"

#include <memory>
#include <QFileDialog>

#include <QFileInfo>
#include <QAction>

namespace widgets
{
MainWindowManager*
MainWindowManager::getInstance()
{
    static std::unique_ptr<MainWindowManager> instance(new MainWindowManager(nullptr));
    return instance.get();
}
MainWindowManager::~MainWindowManager()
{
    GRAPHIC_SCENE_FACTORY()->unregisterObserver(this);
}

MainWindowManager::MainWindowManager(QObject* parent) :
    QObject(parent), mWorkingDialog(nullptr), mDone(true)
{
    mWorkingDialog.SetProgressBarActive(true);
    GRAPHIC_SCENE_FACTORY()->registerObserver(this);

    connect(this,
            &MainWindowManager::doUpdateProgressBar,
            &mWorkingDialog,
            &WorkingDialog::UpdateProgressBar,
            Qt::QueuedConnection);

    connect(this,
            &MainWindowManager::transformDone,
            &mWorkingDialog,
            &WorkingDialog::SetDoneState,
            Qt::QueuedConnection);
    connect(this,
            &MainWindowManager::updateOutputWidget,
            mWorkingDialog.output_widget(),
            &OutputWidget::writeOutput,
            Qt::QueuedConnection);
    
    this->createNewSession();
}

bool
MainWindowManager::createNewSession(const QString& model)
{
    mSubmodules.clear();
    GRAPHIC_SCENE_FACTORY()->clear();
    ERIS_CHECK(GRAPHIC_SCENE_FACTORY()->addSubGraphicScene());
    ERIS_CHECK(GRAPHIC_SCENE_FACTORY()->setCurrentGraphicScene(0));

    MainWindowActionsManager::getInstance()->enableAll();
    MainWindowButtonsGroupManager::getInstance()->enableAll();
    MainWindowToolButtonsManager::getInstance()->enableAll();
    EvaluationTab::Get()->view()->clear();
    if (!model.isEmpty())
    {
        ERIS_CHECK(GRAPHIC_SCENE_FACTORY()->current()->load(model));
    }

    ERIS_CHECK(GRAPHIC_SCENE_FACTORY()->setCurrentGraphicScene(0));

    mTotalSessions++;
    return true;
}

void
MainWindowManager::resetAndDisable()
{
    GRAPHIC_SCENE_FACTORY()->clear();
    MainWindowActionsManager::getInstance()->reset();
    MainWindowButtonsGroupManager::getInstance()->enableAll(false);
    MainWindowToolButtonsManager::getInstance()->enableAll(false);
    EvaluationTab::Get()->view()->clear();
    mSubmodules.clear();
}

bool
MainWindowManager::startVerification()
{
    return GRAPHIC_SCENE_FACTORY()->current()->buildAndStartXPrism();
}

bool
MainWindowManager::transform()
{
    if (!mDone.load(std::memory_order_acquire))
    {
        mWorkingDialog.RaiseIt();
        return false;
    }
    mThreadPool.waitForDone();
    mDone.store(false, std::memory_order_seq_cst);
    mWorkingDialog.Clear();

    mThreadPool.start([this] {
        const bool success = GRAPHIC_SCENE_FACTORY()->current()->transform();

        emit transformDone();

        if (!success)
        {// TODO fix this to show the log only on button click!
            PRINT_ERROR("Failed to transform");
            ErrorHandler::getInstance().show();
            emit updateOutputWidget("Failed to transform current model", Qt::red);
        }
        else
        {
            emit updateOutputWidget("Transformation was successful", Qt::green);
        }
        QThread::msleep(700);
        mDone.store(true, std::memory_order_seq_cst);
    });
    mWorkingDialog.ShowIt();
    return true;
}

bool
MainWindowManager::evaluate()
{
    return GRAPHIC_SCENE_FACTORY()->current()->evaluateExperiment();
}

bool
MainWindowManager::openItemSettings()
{
    return GRAPHIC_SCENE_FACTORY()->current()->openItemSettings();
}

bool
MainWindowManager::swapNodeCriticality()
{
    GRAPHIC_SCENE_FACTORY()->current()->swapNodeCriticality();
    return true;
}

bool
MainWindowManager::swapNodeId()
{
    GRAPHIC_SCENE_FACTORY()->current()->swapNodeId();
    return true;
}

bool
MainWindowManager::openFile()
{
    QString tmp = QFileDialog::getOpenFileName(
            MainWindow::getInstance(), "Open File", /*QDir::currentPath()*/ "", "All Files (*.xml)");

    if (!tmp.isNull())
    {
        createNewSession();
        ERIS_CHECK(GRAPHIC_SCENE_FACTORY()->current());
        const bool ret = GRAPHIC_SCENE_FACTORY()->current()->load(tmp);
        if (ret)
        {
            ERIS_CHECK(GRAPHIC_SCENE_FACTORY()->setCurrentGraphicScene(0));
        }

        return ret;
    }

    return false;
}

bool
MainWindowManager::optionAddRedundancy()
{
    return GRAPHIC_SCENE_FACTORY()->current()->optionAddRedundancy();
}

bool
MainWindowManager::setOutFileName()
{
    return GRAPHIC_SCENE_FACTORY()->current()->setOutfileName();
}

bool
MainWindowManager::exportAs(utils::FileManager::Extension extension)
{
    QString tmp;
    switch (extension)
    {
        case utils::FileManager::Extension::png:
            tmp = QFileDialog::getSaveFileName(MainWindow::getInstance(),
                                               tr("Save Graphic Scene"),
                                               "",
                                               tr("Graphic Scene (*.png);;All Files (*)"));
            break;
        case utils::FileManager::Extension::pdf:
            tmp = QFileDialog::getSaveFileName(MainWindow::getInstance(),
                                               tr("Save Graphic Scene"),
                                               "",
                                               tr("Graphic Scene (*.pdf);;All Files (*)"));
            break;
        case utils::FileManager::Extension::svg:
            tmp = QFileDialog::getSaveFileName(MainWindow::getInstance(),
                                               tr("Save Graphic Scene"),
                                               "",
                                               tr("Graphic Scene (*.svg);;All Files (*)"));
            break;
    }
    if (!tmp.isNull())
    {
        return GRAPHIC_SCENE_FACTORY()->current()->exportAs(extension, tmp);
    }
    return false;
}

bool
MainWindowManager::save()
{
    PRINT_INFO("save to [%d]: %s ",
               GRAPHIC_SCENE_FACTORY()->getIdx(),
               GRAPHIC_SCENE_FACTORY()->current()->path().toStdString().c_str());

    return GRAPHIC_SCENE_FACTORY()->current()->save();
}

bool
MainWindowManager::saveAs()
{
    QString tmp = QFileDialog::getSaveFileName(MainWindow::getInstance(),
                                               tr("Save Graphic Scene"),
                                               "",
                                               tr("Graphic Scene (*.xml);;All Files (*)"));
    if (!tmp.isNull())
    {
        return GRAPHIC_SCENE_FACTORY()->current()->saveAs(tmp);
    }
    return false;
}

bool
MainWindowManager::deleteItem()
{
    return GRAPHIC_SCENE_FACTORY()->current()->deleteItem();
}

void
MainWindowManager::showHelpWindow()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About ERIS");
    msgBox.setText("This is ERIS. A tool for modelling and evaluating complex, critical system structures of autonomous vehicles.\n"
    "For any questions regarding usage or licensing, please refer to the GitHub page, or contact the author directly.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

bool
MainWindowManager::setNodeMode(int id)
{
    return GRAPHIC_SCENE_FACTORY()->current()->setNodeMode(id);
}

bool
MainWindowManager::setAnnotationMode(int id)
{
    return GRAPHIC_SCENE_FACTORY()->current()->setAnnotationMode(id);
}

bool
MainWindowManager::closeSubmodule(const std::string& path)
{
    ERIS_CHECK(!path.empty());

    const int idx = mSubmodules[path];
    if (idx > 0)
    {
        ERIS_CHECK(GRAPHIC_SCENE_FACTORY()->deleteSubGraphicScene(idx));
        return true;
    }
    return false;
}

bool
MainWindowManager::openSubmodule(const std::string& path)
{
    ERIS_CHECK(!path.empty());

    if (!QFileInfo::exists(tr(path.c_str())))
    {
        PRINT_ERROR("Submodule path : %s does not exist ", path.c_str());
        return false;
    }

    if (mSubmodules[path] > 0)
    {
        ModulesTabWidget::getInstance()->setCurrentIndex(mSubmodules[path]);
        PRINT_INFO("Submodule already exist idx = %d ", mSubmodules[path]);
        return true;
    }

    int idx = -1;
    if (!GRAPHIC_SCENE_FACTORY()->addSubGraphicScene(&idx, tr(path.c_str())))
    {
        PRINT_ERROR("Failed to create subgraph");
        return false;
    }

    ERIS_CHECK(GRAPHIC_SCENE_FACTORY()->setCurrentGraphicScene(idx));

    return true;
}

void
MainWindowManager::graphicSceneIsAboutToBeDeleted(GraphicScene*g, int idx)
{
    if (idx > 0 && !mSubmodules.empty())
    {
        ERIS_CHECK(mSubmodules.erase(g->path().toStdString()) == 1);
    }
}

void
MainWindowManager::subGraphicSceneAdded(GraphicScene*g, int idx)
{
    if (idx > 0)
    {
        mSubmodules[g->path().toStdString()] = idx;
    }
}

void
MainWindowManager::setModelType(graph::Model::Type type)
{
    graph::Model::getInstance().setType(type);
    if (type == graph::Model::CTMC)
    {
        MainWindowActionsManager::getInstance()->toggleModelCTMC(true);
    }
    else if (type == graph::Model::MDP)
    {
        MainWindowActionsManager::getInstance()->toggleModelCTMC(false);
    }
    else
    {
        // Unreachable code
        ERIS_CHECK(0);
    }
}

void
MainWindowManager::updateTransformationState(int val)
{
    emit doUpdateProgressBar(val);
}

}  // namespace widgets