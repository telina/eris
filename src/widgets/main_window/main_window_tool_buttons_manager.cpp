/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
 *      - 2021-2021 Mohammed Hussam Al Turjman (Mohammed_Hussam.Al_Turjman@dfki.de)
 *      - 2023 Rhea Rinaldo (Rhea@Odlanir.de)
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

#include "main_window_tool_buttons_manager.h"
#include "main_window.h"
#include "checks.h"

#include <QToolButton>
#include <QToolBar>
#include <QPushButton>
#include <QLabel>

namespace widgets
{
MainWindowToolButtonsManager::~MainWindowToolButtonsManager()
{

}

MainWindowToolButtonsManager*
MainWindowToolButtonsManager::getInstance()
{
    // Intentionally leaked.
    static MainWindowToolButtonsManager* instance = new MainWindowToolButtonsManager(nullptr);

    return instance;
}

MainWindowToolButtonsManager::MainWindowToolButtonsManager(QObject* parent) :
    QObject(parent)
{
    mStartVerificationBut = new QToolButton();
    initToolButton(mStartVerificationBut, QString(":/start"), 
        QString("Start evaluation"), SLOT(startVerificationButClicked()));
    
    mTransformBut = new QToolButton();
    initToolButton(mTransformBut, QString(":/build"), 
        QString("Transform current model"), SLOT(transformButClicked()));

    mExperimentBut = new QToolButton();
    initToolButton(mExperimentBut, QString(":/runexperiment"), 
        QString("Perform experiment"), SLOT(experimentButClicked()));

    mItemSettingsBut = new QToolButton();
    initToolButton(mItemSettingsBut, QString(":/settings"), 
        QString("Node Settings"), SLOT(itemSettingsButClicked()));

    mNodeCriticalityBut = new QToolButton();
    initToolButton(mNodeCriticalityBut, QString(":/criticality"), 
        QString("Change the criticality"), SLOT(swapCriticalityButClicked()));

    mNodeSwapIdBut = new QToolButton();
    mNodeSwapIdBut->setEnabled(false);
    initToolButton(mNodeSwapIdBut, QString(":/id_button"), 
        QString("Swap the ID between two nodes"), SLOT(swapIdButClicked()));    

    QToolBar* verificationToolBar = MainWindow::getInstance()->addToolBar(tr("Verify"));
    verificationToolBar->addWidget(mTransformBut);
    verificationToolBar->addWidget(mExperimentBut);
    verificationToolBar->addWidget(mStartVerificationBut);
    verificationToolBar->setIconSize(QSize(32, 32));
    QToolBar* editToolBar = MainWindow::getInstance()->addToolBar(tr("Edit"));
    editToolBar->addWidget(mItemSettingsBut);
    editToolBar->addWidget(mNodeCriticalityBut);
    editToolBar->addWidget(mNodeSwapIdBut);
    editToolBar->setIconSize(QSize(32, 32));
    QToolBar* informationToolBar = MainWindow::getInstance()->addToolBar(tr("Information"));
    informationToolBar->addWidget(MainWindow::getInstance()->mInformationLabel);
}

void
MainWindowToolButtonsManager::initToolButton(QToolButton* button, QString iconpath, 
        QString tooltip, const char* slot)
{
    button->setIcon(QIcon(iconpath));
    button->setToolTip(tooltip);

    connect(button, SIGNAL(clicked()), this, slot); 
    mToolButtons.emplace_back(button);
}

void
MainWindowToolButtonsManager::enableAll(bool enable)
{
    for (int i = 0; i < mToolButtons.size(); ++i)
    {
        mToolButtons[i]->setEnabled(enable);
    }
    mNodeSwapIdBut->setEnabled(false);
}

void
MainWindowToolButtonsManager::enableNodeSwapIdButton(bool enable)
{
    mNodeSwapIdBut->setEnabled(enable);
}

void
MainWindowToolButtonsManager::disableVerificationButtons()
{
    mStartVerificationBut->setDisabled(true);
    mExperimentBut->setDisabled(true);
}

void
MainWindowToolButtonsManager::startVerificationButClicked()
{
    MainWindow::getInstance()->startVerificationActTriggered();
}

void
MainWindowToolButtonsManager::transformButClicked()
{
    MainWindow::getInstance()->transformActTriggered();
}

void
MainWindowToolButtonsManager::experimentButClicked()
{
    MainWindow::getInstance()->experimentActTriggered();
}

void
MainWindowToolButtonsManager::itemSettingsButClicked()
{
    MainWindow::getInstance()->itemSettingsActTriggered();
}

void
MainWindowToolButtonsManager::swapCriticalityButClicked()
{
    MainWindow::getInstance()->nodeSwapCriticalityActTriggered();
}

void
MainWindowToolButtonsManager::swapIdButClicked()
{
    MainWindow::getInstance()->nodeSwapIdActTriggered();
}

}  // namespace widgets