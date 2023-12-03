/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
 *      - 2020-2023 Rhea Rinaldo (Rhea@Odlanir.de)
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

#ifndef ERIS_WIDGETS_NODE_SETTINGS_H
#define ERIS_WIDGETS_NODE_SETTINGS_H

#include "eris_config.h"
#include "model.h"
#include "component_type.h"
#include "recovery_strategy.h"
#include "node_item.h"
#include "error_handler.h"
#include "scene_status.h"
#include "graphic_scene.h"
#include "checks.h"
#include "node_settings_validator.h"
#include "module_chooser_layout.h"
#include "scene_status.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QRegularExpression>
#include <QString>
#include <QTextEdit>
#include <string>
#include <tuple>
#include <vector>

namespace graph
{
class Model;
class NodeItem;
}  // namespace graph

namespace widgets
{
using graph::NodeItem;
class NodeSettingsSubsystemLayout;
class ERIS_EXPORT NodeSettings : public QDialog
{
    Q_OBJECT

public:
    /**
     * Initialises the node settings by providing the associated node and starting the creation
     * of labels and text/line edits.
     * @param node, the associated node
     */
    NodeSettings(NodeItem* nodeItem);

    ~NodeSettings();

    /**
     * Creates labels, line/text edits and buttons used in the layout. In case of an environment
     * node, probabilities, essential nodes cannot be set.
     */
    void
    createElements();

    /**
     * Creates the layout and adds the previously created buttons.
     */
    void
    createLayout();

    /**
     * Opens the window so the user can interact inserting/modifying settings.
     */
    void
    openWindow();

public slots:

    /**
     * Views all line and text edits and updates the values of the associated node accordingly if
     * the input is valid. If the input is invalid, the window is not closed and a dedicated
     * error message is displayed.
     */
    void
    save();

    /**
     * Disregards all changes in the line edits and closes the window.
     */
    void
    cancel();

    void
    updateLabels();

    void
    toggleDefectRecoveryOptions(bool toggled);

    void
    toggleCorruptionRecoveryOptions(bool toggled);

    void
    toggleCorruptionCustomTextEdit(bool toggled);

    void
    toggleDefectCustomTextEdit(bool toggled);

private:
    /**
     * Sets the fields according to the information given by the associated
     * node item. 
     */
    void
    initializeFields();

    /**
     * Disables or enables, indicated by the provided flag, the fields of the 
     * node settings window.
     * @param disabled
     */
    void
    disableEnableHelper(bool disabled);

    bool
    checkId();

    bool
    checkSimulationInput();
    
    bool
    checkSubmoduleInput();

    bool
    checkIntrusionInput();

    bool
    checkFailureInput();

    bool
    checkSecurityInput();

    bool
    checkDefectRecoveryInput();

    bool
    checkCorruptionRecoveryInput();

    bool
    checkEssentialInput();

    /** Node associated with these node settings */
    NodeItem* mAssociatedNode;

    /** Flag Indicating that a line edit was modified (during save) */
    bool mModified;

    /** Labels */
    QLabel* mEndLabel;
    QLabel* mIntrusionLabel;
    QLabel* mFailureLabel;
    QLabel* mSecurityLabel;
    QLabel* mDefectRecoveryLabel;
    QLabel* mCorruptionRecoveryLabel;
    QLabel* mCorruptionRecoveryModeLabel;
    QLabel* mDefectRecoveryModeLabel;

    QLabel* mIdLabel;
    QLabel* mEssentialLabel;
    QLabel* mErrorLabel;

    /** Checkboxes */
    QCheckBox* mDefectRecoveryCheckBox;
    QCheckBox* mCorruptionRecoveryCheckBox;

    /** Radio Buttons*/
    QRadioButton* mCorruptionRecoveryGeneralRadioButton;
    QRadioButton* mCorruptionRecoveryRestrictedRadioButton;
    QRadioButton* mCorruptionRecoveryCustomRadioButton;
    QRadioButton* mDefectRecoveryGeneralRadioButton;
    QRadioButton* mDefectRecoveryRestrictedRadioButton;
    QRadioButton* mDefectRecoveryCustomRadioButton;

    /** Buttons */
    QPushButton* mSaveButton;
    QPushButton* mCancelButton;

    /** Button Groups */
    QButtonGroup* mDefectButtonGroup;
    QButtonGroup* mCorruptionButtonGroup;

    /** Line Edits */
    QLineEdit* mIntrusionLineEdit;
    QLineEdit* mFailureLineEdit;
    QLineEdit* mSecurityLineEdit;
    QLineEdit* mDefectRecoveryLineEdit;
    QLineEdit* mCorruptionRecoveryLineEdit;
    QLineEdit* mIdLineEdit;
    QTextEdit* mEssentialTextEdit;
    QTextEdit* mCustomCorruptionRecoveryTextEdit;
    QTextEdit* mCustomDefectRecoveryTextEdit;
    QDialogButtonBox* mButtonBox;

    /** Module Layout (Off/Simulation/Eris) */
    ModuleChooserLayout* mModuleChooserLayout;
};

}  // namespace widgets

#endif /* ERIS_WIDGETS_NODE_SETTINGS_H */
