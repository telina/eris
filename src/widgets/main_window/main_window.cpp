/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
 *      - 2021-2021 Mohammed Hussam Al Turjman (Mohammed_Hussam.Al_Turjman@dfki.de)
 *      - 2020-2023  Rhea Rinaldo (Rhea@Odlanir.de)
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


#include "main_window.h"

#include "command.h"
#include "logger.h"
#include "node_item.h"
#include "edge_item.h"
#include "text_item.h"
#include "file_manager.h"
#include "scene_status.h"
#include "prism.h"
#include "evaluation_settings.h"
#include "error_handler.h"
#include "graphic_scene.h"
#include "transcriber.h"
#include "transformer.h"
#include "evaluation_tab.h"
#include "graphics_view.h"
#include "xprism.h"
#include "modules_tab_widget.h"
#include "main_window_manager.h"
#include "main_window_tool_buttons_manager.h"
#include "main_window_buttons_group_manager.h"
#include "graphic_scene.h"
#include "utils.h"

#include <memory>
#include <QToolButton>
#include <QToolBar>
#include <QLabel>
#include <QButtonGroup>
#include <QToolBox>
#include <QMenu>
#include <QMenuBar>
#include <QResource>
#include <QStatusBar>

namespace widgets
{
using eval::Prism;
using eval::XPrism;
using graph::EdgeItem;
using graph::NodeItem;
using graph::TextItem;
using utils::FileManager;


MainWindow::MainWindow() :
    mModeOfOperation(graphInternal::Operational::Mode::simple), mInformationLabel(new QLabel(""))
{
    connect(this, &MainWindow::DisableGuiElementsSignal, this, &MainWindow::disableGuiElementsSlot,
            Qt::QueuedConnection);
}

MainWindow::~MainWindow() = default;

bool        
MainWindow::init()
{
    // Ensure XPrism and Prism commands are initialized
    Prism::getInstance();
    XPrism::getInstance();

    // Ensure all managers are initialized in this order
    MainWindowActionsManager::getInstance();
    MainWindowToolButtonsManager::getInstance();
    MainWindowButtonsGroupManager::getInstance();
    GraphicScene::Factory::getInstance();
    ModulesTabWidget::getInstance();
    EvaluationSettingsDialog::Create(this);
    MainWindowManager::getInstance();

    MainWindowButtonsGroupManager::getInstance()->registerObserver(this);

    qRegisterMetaType<QList<QPointF>>("const QList<QPointF>&");
    qRegisterMetaType<eval::PrismResultsParser::ParseError>("parser::PrismResultsParser::"
                                                              "ParseError");
    qRegisterMetaType<Qt::GlobalColor>("Qt::GlobalColor");

    QResource::registerResource("resources.qrc");
    // Default file names

    QFont font("Helvetica [Cronyx]", 10);
    mInformationLabel->setStyleSheet("QLabel { color :rgb(15, 150, 105); }");
    mInformationLabel->setFont(font);

    this->setWindowTitle("ERIS");
    // this->setMinimumSize(1100, 750);

    createActions();
    createLayout();

    if (!utils::commandExists("xprism"))
    {
        MainWindowToolButtonsManager::getInstance()->disableVerificationButtons();
        ErrorHandler::getInstance().setError(Errors::prismCommandNotAvailable("xprism"));
        ErrorHandler::getInstance().show();
    }

    // DeprecatedSceneStatus::getInstance().setChanged(true);

    statusBar()->showMessage("");
    MainWindowManager::getInstance()->createNewSession();
    return true;
}

void
MainWindow::createLayout()
{
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(MainWindowButtonsGroupManager::getInstance()->mToolBox);

    auto tabWidget = new QTabWidget();

    tabWidget->addTab(ModulesTabWidget::getInstance(), "Model");
    tabWidget->addTab(EvaluationTab::Get(), "Evaluation");

    layout->addWidget(tabWidget);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    setWindowTitle(tr("ERIS"));
    setUnifiedTitleAndToolBarOnMac(true);
}

void
MainWindow::disableGuiElements(bool status)
{
    emit DisableGuiElementsSignal(status);
}

void
MainWindow::disableGuiElementsSlot(bool status)
{
    MainWindowActionsManager::getInstance()->enableAll(!status);
    MainWindowButtonsGroupManager::getInstance()->enableAll(!status);
    MainWindowToolButtonsManager::getInstance()->enableAll(!status);
    /*MainWindowToolButtonsManager::getInstance()->mStartVerificationBut->setDisabled(status);
    MainWindowToolButtonsManager::getInstance()->mTransformBut->setDisabled(status);
    MainWindowToolButtonsManager::getInstance()->mItemSettingsBut->setDisabled(status);
    MainWindowButtonsGroupManager::getInstance()->mToolBox->setDisabled(status);
    MainWindowMenusManager::getInstance()->mFileMenu->setDisabled(status);
    MainWindowMenusManager::getInstance()->mHelpMenu->setDisabled(status);
    MainWindowMenusManager::getInstance()->mOptionsMenu->setDisabled(status);
    MainWindowMenusManager::getInstance()->mItemMenu->setDisabled(status);
    GRAPHIC_SCENE()->graphics_view()->setDisabled(status); */
}

void
MainWindow::about()
{
    QMessageBox::about(this,
                       tr("About Diagram Scene"),
                       tr("The <b>Diagram Scene</b> example shows "
                          "use of the graphics framework."));
}

MainWindow*
MainWindow::getInstance()
{
    // Intentionally leaked.
    static MainWindow* instance = new MainWindow();
    return instance;
}
void
MainWindow::showInTheCenter()
{
    int x = (width()) / 2;
    int y = (height()) / 2;
    move(x, y);
    show();
}

}  // namespace widgets
