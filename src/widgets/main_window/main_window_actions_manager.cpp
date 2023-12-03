/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
 *
 * Authors:
 *      - 2021-2021 Mohammed Hussam Al Turjman (Mohammed_Hussam.Al_Turjman@dfki.de)
 *      - 2023  Rhea Rinaldo (Rhea@Rinaldo.biz)
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


#include "main_window_actions_manager.h"
#include "checks.h"
#include "main_window.h"
#include "rate_interpretation.h"
#include <QAction>
#include <QMenuBar>

namespace widgets
{

MainWindowActionsManager::MainWindowActionsManager(QObject* parent) : QObject(parent)
{
    mNewFileAct = new QAction("&New File");
    this->initMenuAction(mNewFileAct, SLOT(newFileMenuItemClicked()), QKeySequence::New, 
        "Create a new file", false, false);

    mNewFileInTabAct = new QAction("New File in &Tab");
    this->initMenuAction(mNewFileInTabAct, SLOT(newFileInTabMenuItemClicked()), QKeySequence::AddTab, 
        "Create a new tab, e.g., for a module", false, false);
    
    mOpenFileAct = new QAction("&Open File");
    this->initMenuAction(mOpenFileAct, SLOT(openFileMenuItemClicked()), QKeySequence::Open, 
        "Choose file to open", false, false);

    mSaveFileAct = new QAction("&Save File"); 
    this->initMenuAction(mSaveFileAct, SLOT(saveFileMenuItemClicked()), QKeySequence::Save, 
        "Save current model to file", false, false);

    mSaveFileAsAct = new QAction("Save File as");
    this->initMenuAction(mSaveFileAsAct, SLOT(saveFileAsMenuItemClicked()), QKeySequence::SaveAs, 
        "Save current model as ...", false, false);

    mExportAsPngAct = new QAction("&PNG");
    this->initMenuAction(mExportAsPngAct, SLOT(exportAsPngMenuItemClicked()), Qt::Key_unknown, 
        "Export model as PNG", false, false);

    mExportAsPdfAct = new QAction("&PDF");
    this->initMenuAction(mExportAsPdfAct, SLOT(exportAsPdfMenuItemClicked()), Qt::Key_unknown, 
        "Export model as PDF", false, false);

    mExportAsSvgAct = new QAction("&SVG");
    this->initMenuAction(mExportAsSvgAct, SLOT(exportAsSvgMenuItemClicked()), Qt::Key_unknown, 
        "Export model as SVG", false, false);
    
    mSetOutFileName = new QAction("Set PRISM Filename");
    this->initMenuAction(mSetOutFileName, SLOT(setOutFileNameItemClicked()), Qt::Key_unknown, 
        "Set the name for the generated PRISM (.pm) file", false, false);
    
    mModelOptionCTMC = new QAction("&CTMC");
    this->initMenuAction(mModelOptionCTMC, SLOT(optionModelCTMCItemClicked()), Qt::Key_unknown, 
        "Transformation Mode: CTMC", true, true);
    
    mModelOptionMDP = new QAction("&MDP");
    this->initMenuAction(mModelOptionMDP, SLOT(optionModelMDPItemClicked()), Qt::Key_unknown, 
        "Transformation mode: MDP", true, false);
    
    mOptionHourlyRates = new QAction("&Hours");
    this->initMenuAction(mOptionHourlyRates, SLOT(optionHourlyRatesItemClicked()), Qt::Key_unknown, 
        "Use Hourly Rates (default)", true, true);
    
    mOptionMonthlyRates = new QAction("&Month");
    this->initMenuAction(mOptionMonthlyRates, SLOT(optionMonthlyRatesItemClicked()), Qt::Key_unknown, 
        "Use Monthly Rates", true, false);
    
    mOptionYearlyRates = new QAction("&Years");
    this->initMenuAction(mOptionYearlyRates, SLOT(optionYearlyRatesItemClicked()), Qt::Key_unknown, 
        "Use Yearly Rates (default)", true, false);
    
    mAddRedundancyDefinition = new QAction("&Define Redundancy");
    this->initMenuAction(mAddRedundancyDefinition, SLOT(optionAddRedundancyItemClicked()), Qt::Key_unknown, 
        "Define nodes as redundant (globally)", false, false);
    
    mEvaluationSettings = new QAction("&Evaluation Settings");
    this->initMenuAction(mEvaluationSettings, SLOT(optionEvaluationSettingsItemClicked()), Qt::Key_unknown, 
        "Show evaluation settings dialog", false, false);
    
    mOptionOptimizedMode = new QAction("&Optimized");
    this->initMenuAction(mOptionOptimizedMode, SLOT(optionOptimizedModeItemClicked()), Qt::Key_unknown, 
        "Enable strict mode of operation", true, true);
    
    mOptionSimpleMode = new QAction("&Simple");
    this->initMenuAction(mOptionSimpleMode, SLOT(optionSimpleModeItemClicked()), Qt::Key_unknown, 
        "Enable strict mode of operation", true, false);
    
    mHelpAct = new QAction("Help");
    this->initMenuAction(mHelpAct, SLOT(helpMenuItemClicked()), Qt::Key_F1, "Open the Help Menu", 
        false, false);

    mDeleteItemAct = new QAction("&Delete");
    this->initMenuAction(mDeleteItemAct, SLOT(deleteMenuItemClicked()), QKeySequence::Delete, 
        "Delete item from diagram", false, false); 

    mExitAct = new QAction("&Exit");
    this->initMenuAction(mExitAct, SLOT(exitMenuItemClicked()), QKeySequence::Quit, 
        "Exits AT-CARS", false, false); 
    
    // Place Actions in Menu bar
    // --- File ---
    mFileMenu = MainWindow::getInstance()->menuBar()->addMenu(tr("&File"));
    mFileMenu->addAction(mNewFileAct);
    mFileMenu->addAction(mNewFileInTabAct);
    mFileMenu->addAction(mOpenFileAct);
    mFileMenu->addAction(mSaveFileAct);
    mFileMenu->addAction(mSaveFileAsAct);
    mFileMenu->addAction(mSetOutFileName);
    mFileMenu->addSeparator();
    mExportFileSubMenu = mFileMenu->addMenu(tr("&Export Model as"));
    mExportFileSubMenu->addAction(mExportAsPdfAct);
    mExportFileSubMenu->addAction(mExportAsPngAct);
    mExportFileSubMenu->addAction(mExportAsSvgAct);
    mFileMenu->addSeparator();
    mFileMenu->addAction(mExitAct);

    // --- Item ---
    mItemMenu = MainWindow::getInstance()->menuBar()->addMenu(tr("&Item"));
    mItemMenu->addAction(mDeleteItemAct);

    // --- Help ---
    mHelpMenu = MainWindow::getInstance()->menuBar()->addMenu(tr("&Help"));
    mHelpMenu->addAction(mHelpAct);
    
    // --- Options ---
    mOptionsMenu = MainWindow::getInstance()->menuBar()->addMenu(tr("&Options"));
    mMarkovModelSubMenu = mOptionsMenu->addMenu(tr("&Markov Model"));
    mMarkovModelSubMenu->addAction(mModelOptionCTMC);
    mMarkovModelSubMenu->addAction(mModelOptionMDP);
    mRateInterpretationSubMenu = mOptionsMenu->addMenu(tr("&Interpret Rates as"));
    mRateInterpretationSubMenu->addAction(mOptionHourlyRates);
    mRateInterpretationSubMenu->addAction(mOptionMonthlyRates);
    mRateInterpretationSubMenu->addAction(mOptionYearlyRates);
           
    mModeOfOperationSubMenu = mOptionsMenu->addMenu(tr("Mode of &Operation"));
    mModeOfOperationSubMenu->addAction(mOptionOptimizedMode);
    mModeOfOperationSubMenu->addAction(mOptionSimpleMode);
    mOptionsMenu->addAction(mAddRedundancyDefinition);
    mOptionsMenu->addAction(mEvaluationSettings);
    
}

MainWindowActionsManager::~MainWindowActionsManager()
{

}

MainWindowActionsManager*
MainWindowActionsManager::getInstance()
{
    // Intentionally leaked.
    static MainWindowActionsManager* instance = new MainWindowActionsManager(nullptr);
    return instance;
}

void
MainWindowActionsManager::reset()
{
    enableAll(false);
    mNewFileAct->setEnabled(true);
    mOpenFileAct->setEnabled(true);
}

void
MainWindowActionsManager::initMenuAction(QAction* action, const char* slot, QKeySequence shortcut, 
    QString status_tip, bool checkable, bool checked)
{
    connect(action, SIGNAL(triggered()), this, slot); 

    action->setStatusTip(status_tip);                                              
    action->setCheckable(checkable);                                               
    action->setChecked(checked);                                                   
    action->setShortcut(shortcut);                                                 
    action->setEnabled(true);      
                                                    
    mActions.emplace_back(action);
}

void
MainWindowActionsManager::enableAll(bool enable)
{
    for (int i = 0; i < mActions.size(); ++i)
    {
        mActions[i]->setEnabled(enable);
    }
}

void
MainWindowActionsManager::toggleModelCTMC(bool flag)
{
    mModelOptionCTMC->setChecked(flag);
    mModelOptionMDP->setChecked(!flag);
}

void 
MainWindowActionsManager::toggleRateInterpretation(graph::RateInterpretation rateInterpretation)
{
    if (rateInterpretation == graph::RateInterpretation::hourly)
    {
        mOptionHourlyRates->setChecked(true);
        mOptionMonthlyRates->setChecked(false);
        mOptionYearlyRates->setChecked(false);
    }
    else if (rateInterpretation == graph::RateInterpretation::monthly)
    {
        mOptionMonthlyRates->setChecked(true);
        mOptionHourlyRates->setChecked(false);
        mOptionYearlyRates->setChecked(false);
    }
    else if (rateInterpretation == graph::RateInterpretation::yearly)
    {
        mOptionYearlyRates->setChecked(true);
        mOptionMonthlyRates->setChecked(false);
        mOptionHourlyRates->setChecked(false);
    }
}


// Slots

void
MainWindowActionsManager::newFileMenuItemClicked()
{
    MainWindow::getInstance()->newFileActTriggered();
}

void
MainWindowActionsManager::newFileInTabMenuItemClicked()
{
    // Implement me
}

void
MainWindowActionsManager::openFileMenuItemClicked()
{
    MainWindow::getInstance()->openFileActTriggered();
}

void
MainWindowActionsManager::saveFileMenuItemClicked()
{
    MainWindow::getInstance()->saveFileActTriggered();
}

void
MainWindowActionsManager::saveFileAsMenuItemClicked()
{
    MainWindow::getInstance()->saveFileAsActTriggered();
}

void
MainWindowActionsManager::setOutFileNameItemClicked()
{
    MainWindow::getInstance()->setOutfileNameActTriggered();
}
void
MainWindowActionsManager::exportAsPdfMenuItemClicked()
{
    MainWindow::getInstance()->exportAsPdfActTriggered();
}

void
MainWindowActionsManager::exportAsPngMenuItemClicked()
{
    MainWindow::getInstance()->exportAsPngActTriggered();
}

void
MainWindowActionsManager::exportAsSvgMenuItemClicked()
{
    MainWindow::getInstance()->exportAsSvgActTriggered();
}

void
MainWindowActionsManager::exitMenuItemClicked()
{
    MainWindow::getInstance()->exitActTriggered();
}

void
MainWindowActionsManager::deleteMenuItemClicked()
{
    MainWindow::getInstance()->deleteItemActTriggered();
}

void
MainWindowActionsManager::helpMenuItemClicked()
{
    MainWindow::getInstance()->showHelpWindow();
}

void
MainWindowActionsManager::optionModelCTMCItemClicked()
{
    mRateInterpretationSubMenu->setEnabled(true);
    MainWindow::getInstance()->optionModelCTMCActTriggered();
}

void
MainWindowActionsManager::optionModelMDPItemClicked()
{
    mRateInterpretationSubMenu->setEnabled(false);
    MainWindow::getInstance()->optionModelMDPActTriggered();
}


void
MainWindowActionsManager::optionHourlyRatesItemClicked()
{   
    toggleRateInterpretation(graph::RateInterpretation::hourly);
    MainWindow::getInstance()->optionRateInterpretationToggled(graph::RateInterpretation::hourly);
}

void
MainWindowActionsManager::optionMonthlyRatesItemClicked()
{
    toggleRateInterpretation(graph::RateInterpretation::monthly);
    MainWindow::getInstance()->optionRateInterpretationToggled(graph::RateInterpretation::monthly);
}

void
MainWindowActionsManager::optionYearlyRatesItemClicked()
{
    toggleRateInterpretation(graph::RateInterpretation::yearly);
    MainWindow::getInstance()->optionRateInterpretationToggled(graph::RateInterpretation::yearly);
}

void
MainWindowActionsManager::optionOptimizedModeItemClicked()
{
    mOptionOptimizedMode->setChecked(true);
    mOptionSimpleMode->setChecked(false);
    MainWindow::getInstance()->optionOptimizedModeActTriggered();
}

void
MainWindowActionsManager::optionSimpleModeItemClicked()
{
    mOptionOptimizedMode->setChecked(true);
    mOptionSimpleMode->setChecked(false);
    MainWindow::getInstance()->optionSimpleModeActTriggered();
}

void
MainWindowActionsManager::optionAddRedundancyItemClicked()
{
    MainWindow::getInstance()->optionAddRedundancyActTriggered();
}

void
MainWindowActionsManager::optionEvaluationSettingsItemClicked()
{
    MainWindow::getInstance()->evaluationSettingsActTriggered();
}


}  // namespace widgets