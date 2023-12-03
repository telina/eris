/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
 *      - 2021-2021 Mohammed Hussam Al Turjman (Mohammed_Hussam.Al_Turjman@dfki.de)
 *      - 2023-2023  Rhea Rinaldo (Rhea@Odlanir.de)
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


#ifndef ERIS_WIDGETS_MAIN_WINDOW_H
#define ERIS_WIDGETS_MAIN_WINDOW_H

#include "component_type.h"
#include "main_window_actions_manager.h"
#include "main_window_tool_buttons_manager.h"
#include "main_window_buttons_group_manager.h"
#include "operational.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QToolButton;
class QToolBox;
class QToolBar;
class QTextEdit;
class QTabWidget;
class QResource;
class QPushButton;
class QMouseEvent;
class QMessageBox;
class QGraphicsItem;
class QAbstractButton;
class QButtonGroup;
class QLabel;
QT_END_NAMESPACE

namespace utils
{
class Command;
}

namespace graphInternal
{
class Operational;
}  // namespace graph
namespace widgets
{
class GraphicsView;
class ModulesTabWidget;

class MainWindow : public QMainWindow,
                   private MainWindowButtonsGroupManager::Observer
{
    Q_OBJECT

protected:
    
    void startVerificationActTriggered();
    
    void transformActTriggered();
    
    void experimentActTriggered();
    
    void itemSettingsActTriggered();
    
    void nodeSwapCriticalityActTriggered();
    
    void nodeSwapIdActTriggered();
        
    void deleteItemActTriggered();
    
    void showHelpWindow();
    
    void openFileActTriggered();
    
    void optionAddRedundancyActTriggered();
    
    void optionRateInterpretationToggled(graph::RateInterpretation);
    
    void setOutfileNameActTriggered();
    
    void exportAsPngActTriggered();
    
    void exportAsPdfActTriggered();

    void exportAsSvgActTriggered();
    
    void saveFileActTriggered();
    
    void saveFileAsActTriggered();

    void newFileActTriggered();

    void exitActTriggered();

    /**
     * Chooses the CTMC mode for transformation.
     */
    void optionModelCTMCActTriggered();

    /**
     * Chooses the MDP mode for transformation.
     */
    void optionModelMDPActTriggered();

    /**
     * Changes mode to strict mode.
     */
    void optionOptimizedModeActTriggered();

    /**
     * Changes mode to lenient mode.
     */
    void optionSimpleModeActTriggered();

    /**
     * Show evaluation settings dialog
     */
    void evaluationSettingsActTriggered();

    void
    ButtonsGroupButtonClicked(int /*id*/) override;
    
    void
    AnnotationGroupButtonClicked(int /*id*/) override;

private:
    
    MainWindow();
    
signals:
    void
    DisableGuiElementsSignal(bool);

private slots:
    void
    disableGuiElementsSlot(bool);

public slots:
    //  void backgroundButtonGroupClicked(QAbstractButton *button);

    /**
     * Opens window for information about this program.
     */
    void
    about();

public:
    
    static MainWindow*
    getInstance();
    void
    showInTheCenter();

    // Must be called after the first Get
    bool
    init();

    ~MainWindow() override;
    
    void
    closeEvent(QCloseEvent* event) override;

    /**
     * Creates all layouts of the GUI.
     */
    void
    createLayout();

    /**
     * Creates and connects actions.
     */
    void
    createActions();
    
    /**
     * Disables/enables the GUI elements that may not be available during the analysis/simulation is
     * performed.
     * @param status indicator whether elements should be disabled or enabled
     */
    void
    disableGuiElements(bool status);

    /** Mode of operation optimized or normal*/
    graphInternal::Operational::Mode mModeOfOperation;
    
    /** Labels */
    QLabel* mInformationLabel;
    
    friend class MainWindowToolButtonsManager;
    friend class MainWindowActionsManager;

};

}  // namespace widgets

#endif /* ERIS_WIDGETS_MAIN_WINDOW_H */
