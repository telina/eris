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

#ifndef ERIS_WIDGETS_MAIN_WINDOW_ACTIONS_MANAGER_H
#define ERIS_WIDGETS_MAIN_WINDOW_ACTIONS_MANAGER_H

#include "eris_config.h"
#include <QObject>
#include <QMenu>
#include <list>
#include <vector>
#include "rate_interpretation.h"

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

namespace widgets
{
    
class ERIS_EXPORT MainWindowActionsManager : public QObject
{
    Q_OBJECT
public:

    ERIS_DISALLOW_COPY_AND_ASSIGN(MainWindowActionsManager);
    
    MainWindowActionsManager() = delete;

    ~MainWindowActionsManager() override;

    void
    reset();
    
    void
    enableAll(bool enable = true);
    
    static MainWindowActionsManager*
    getInstance();
    
    /** Toggles the CTMC/MDP mode. Access from outside needed when files are being loaded in */
    void toggleModelCTMC(bool);
    
    void toggleRateInterpretation(graph::RateInterpretation rateInterpretation);
    
private slots:

/** Several slots for connecting Menu item clicks with their dedicated action */
    void newFileMenuItemClicked();
    
    void newFileInTabMenuItemClicked();

    void openFileMenuItemClicked();

    void saveFileMenuItemClicked();

    void saveFileAsMenuItemClicked();

    void exportAsPdfMenuItemClicked();

    void exportAsPngMenuItemClicked();

    void exportAsSvgMenuItemClicked();
    
    void setOutFileNameItemClicked();

    void helpMenuItemClicked();

    void deleteMenuItemClicked();

    void exitMenuItemClicked();
    
    void optionModelCTMCItemClicked();
    
    void optionModelMDPItemClicked();
    
    void optionHourlyRatesItemClicked();
    
    void optionMonthlyRatesItemClicked();
    
    void optionYearlyRatesItemClicked();
    
    void optionOptimizedModeItemClicked();
    
    void optionSimpleModeItemClicked();
    
    void optionAddRedundancyItemClicked();
    
    void optionEvaluationSettingsItemClicked();
        

private:
    explicit MainWindowActionsManager(QObject* parent);
    
    void 
    initMenuAction(QAction* action, const char* slot, QKeySequence shortcut, 
    QString status_tip, bool checkable, bool checked);

    std::vector<QAction*> mActions;
    
        /** Action for each Menu Item*/
    QAction* mNewFileAct;
    QAction* mNewFileInTabAct;
    QAction* mOpenFileAct;
    QAction* mSaveFileAct;
    QAction* mSaveFileAsAct;
    QAction* mExportAsPngAct;
    QAction* mExportAsPdfAct;
    QAction* mExportAsSvgAct;
    QAction* mSetOutFileName;
    QAction* mModelOptionCTMC;
    QAction* mModelOptionMDP;
    QAction* mAddRedundancyDefinition;
    QAction* mEvaluationSettings;
    QAction* mOptionOptimizedMode;
    QAction* mOptionSimpleMode;
    QAction* mOptionHourlyRates;
    QAction* mOptionMonthlyRates;
    QAction* mOptionYearlyRates;
    QAction* mHelpAct;
    QAction* mDeleteItemAct;
    QAction* mExitAct;

    /** Menus */
    QMenu* mFileMenu;
    QMenu* mItemMenu;
    QMenu* mHelpMenu;
    QMenu* mOptionsMenu;
    QMenu* mExportFileSubMenu;
    QMenu* mMarkovModelSubMenu;
    QMenu* mModeOfOperationSubMenu;
    QMenu* mRateInterpretationSubMenu;

};

}  // namespace widgets

#endif  // ERIS_WIDGETS_MAIN_WINDOW_ACTIONS_MANAGER_H
