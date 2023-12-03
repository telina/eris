/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
 *
 * Authors:
 *      - 2020-2023 Rhea Rinaldo (Rhea@Rinaldo.biz)
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

#include "node_settings.h"

namespace widgets
{
using graph::Model;
using graph::Recovery;

NodeSettings::NodeSettings(NodeItem* nodeItem) :
    mAssociatedNode(nodeItem),
    mModuleChooserLayout(new ModuleChooserLayout(this))
{
    createElements();
    createLayout();
    setWindowTitle(QString("Node Settings: "
                           + QString("Node " + QString::number(mAssociatedNode->getId()))));

    connect(&Model::getInstance(), &Model::modelTypeUpdated, this, &NodeSettings::updateLabels);
}

NodeSettings::~NodeSettings()
{
    mAssociatedNode->nodeSettingsDeactivated();
}

void
NodeSettings::toggleDefectRecoveryOptions(bool toggled)
{
    if (toggled)
    {
        mDefectRecoveryLineEdit->show();
        mDefectRecoveryLabel->show();
        mDefectRecoveryModeLabel->show();
        mDefectRecoveryRestrictedRadioButton->show();
        mDefectRecoveryCustomRadioButton->show();
        mDefectRecoveryGeneralRadioButton->show();
        toggleDefectCustomTextEdit(mDefectRecoveryCustomRadioButton->isChecked());
    }
    else
    {
        mDefectRecoveryLineEdit->hide();
        mDefectRecoveryLabel->hide();
        mDefectRecoveryModeLabel->hide();
        mDefectRecoveryRestrictedRadioButton->hide();
        mDefectRecoveryCustomRadioButton->hide();
        mCustomDefectRecoveryTextEdit->hide();
        mDefectRecoveryGeneralRadioButton->hide();
    }
}

void
NodeSettings::toggleCorruptionRecoveryOptions(bool toggled)
{
    if (toggled)
    {
        mCorruptionRecoveryLineEdit->show();
        mCorruptionRecoveryLabel->show();
        mCorruptionRecoveryModeLabel->show();
        mCorruptionRecoveryRestrictedRadioButton->show();
        mCorruptionRecoveryCustomRadioButton->show();
        mCorruptionRecoveryGeneralRadioButton->show();
        toggleCorruptionCustomTextEdit(mCorruptionRecoveryCustomRadioButton->isChecked());
    }
    else
    {
        mCorruptionRecoveryLineEdit->hide();
        mCorruptionRecoveryLabel->hide();
        mCorruptionRecoveryModeLabel->hide();
        mCorruptionRecoveryRestrictedRadioButton->hide();
        mCorruptionRecoveryCustomRadioButton->hide();
        mCustomCorruptionRecoveryTextEdit->hide();
        mCorruptionRecoveryGeneralRadioButton->hide();
    }
}

void
NodeSettings::toggleCorruptionCustomTextEdit(bool toggled)
{
    if (toggled)
    {
        mCustomCorruptionRecoveryTextEdit->show();
    }
    else
    {
        mCustomCorruptionRecoveryTextEdit->hide();
    }
}

void
NodeSettings::toggleDefectCustomTextEdit(bool toggled)
{
    if (toggled)
    {
        mCustomDefectRecoveryTextEdit->show();
    }
    else
    {
        mCustomDefectRecoveryTextEdit->hide();
    }
}

void
NodeSettings::disableEnableHelper(bool disabled)
{
    /** Checkboxes and Radiobuttons*/
    mDefectRecoveryCheckBox->setDisabled(disabled);
    mCorruptionRecoveryCheckBox->setDisabled(disabled);
    mDefectRecoveryGeneralRadioButton->setDisabled(disabled);
    mDefectRecoveryRestrictedRadioButton->setDisabled(disabled);
    mDefectRecoveryCustomRadioButton->setDisabled(disabled);
    mCorruptionRecoveryGeneralRadioButton->setDisabled(disabled);
    mCorruptionRecoveryRestrictedRadioButton->setDisabled(disabled);
    mCorruptionRecoveryCustomRadioButton->setDisabled(disabled);

    /** LineEdits and TextEdits*/
    mIntrusionLineEdit->setDisabled(disabled);
    mFailureLineEdit->setDisabled(disabled);
    mSecurityLineEdit->setDisabled(disabled);
    mDefectRecoveryLineEdit->setDisabled(disabled);
    mCorruptionRecoveryLineEdit->setDisabled(disabled);
    mCustomCorruptionRecoveryTextEdit->setDisabled(disabled);
    mCustomDefectRecoveryTextEdit->setDisabled(disabled);

    /** Labels */
    mIntrusionLabel->setDisabled(disabled);
    mFailureLabel->setDisabled(disabled);
    mSecurityLabel->setDisabled(disabled);
    mDefectRecoveryLabel->setDisabled(disabled);
    mCorruptionRecoveryLabel->setDisabled(disabled);
    mCorruptionRecoveryModeLabel->setDisabled(disabled);
    mDefectRecoveryModeLabel->setDisabled(disabled);
}

void
NodeSettings::createElements()
{
    // Label + Value Pairs
    mIntrusionLabel = new QLabel();
    mFailureLabel = new QLabel();
    mSecurityLabel = new QLabel();
    mDefectRecoveryLabel = new QLabel();
    mCorruptionRecoveryLabel = new QLabel();
    mCorruptionRecoveryModeLabel = new QLabel("Recovery Strategy:");
    mDefectRecoveryModeLabel = new QLabel("Recovery Strategy:");
    this->updateLabels();  // set labels dependent of the Model type
    mIdLabel = new QLabel(tr("Node ID:"));
    mEssentialLabel = new QLabel(tr("Node Dependencies:"));
    mErrorLabel = new QLabel(tr(""));
    mErrorLabel->setStyleSheet("QLabel {color: red;}");

    mDefectRecoveryCheckBox = new QCheckBox("Defect Recovery");
    mDefectRecoveryCheckBox->setToolTip("Set whether the node is able to recover from a defect \n"
                                        "(safety incident) , hence, adsd a transition from the "
                                        "defective state into the ok state");
    mCorruptionRecoveryCheckBox = new QCheckBox("Corruption Recovery");
    mCorruptionRecoveryCheckBox->setToolTip("Set whether the node is able to recover from a "
                                            "corruption \n"
                                            "(security incident), hence, adds a transition from "
                                            "the corrupted state into the ok state\n "
                                            "Note: This requires at least one non-corrupted reach "
                                            "connection to the environment node");
    mCorruptionRecoveryGeneralRadioButton = new QRadioButton("General");
    mCorruptionRecoveryGeneralRadioButton->setToolTip("General recovery mode: \n"
                                                      "This node can always recover from a "
                                                      "corruption state, e.g. the node itself may "
                                                      "possess\n "
                                                      "multiple software images which can be "
                                                      "booted. This way, the node does not boot "
                                                      "the same"
                                                      "exploitable software image again.");
    mCorruptionRecoveryRestrictedRadioButton = new QRadioButton("Restricted");
    mCorruptionRecoveryRestrictedRadioButton->setToolTip(
            "Restricted recovery mode: \n"
            "This node can only recover from a corruption state if a functioning connection to "
            "the\n"
            "outside world exist, e.g. to supply a software update that eliminates the previously "
            "used exploit.");
    mCorruptionRecoveryCustomRadioButton = new QRadioButton("Custom");
    mCorruptionRecoveryCustomRadioButton->setToolTip("Custom recovery mode: \n"
                                                     "Enter a custom condition that is required "
                                                     "for the node to recover from a corruption "
                                                     "state.\n"
                                                     "E.g. the node could require a software image "
                                                     "stored on another node.");

    mDefectRecoveryGeneralRadioButton = new QRadioButton("General");
    mDefectRecoveryGeneralRadioButton->setToolTip("General recovery mode: \n"
                                                  "This node can always recover from a defective "
                                                  "state, e.g. the node itself may possess\n "
                                                  "multiple software images which can be booted. "
                                                  "This way, the node does not boot the same"
                                                  "faulty software image again.");
    mDefectRecoveryRestrictedRadioButton = new QRadioButton("Restricted");
    mDefectRecoveryRestrictedRadioButton->setToolTip(
            "Restricted recovery mode: \n"
            "This node can only recover from a defective state if a functioning connection to the\n"
            "outside world exist, e.g. to supply a software update that eliminates the previously "
            "existing software flaw.");
    mDefectRecoveryCustomRadioButton = new QRadioButton("Custom");
    mDefectRecoveryCustomRadioButton->setToolTip("Custom recovery mode: \n"
                                                 "Enter a custom condition that is required for "
                                                 "the node to recover from a defective state.\n"
                                                 "E.g. the node could require a software image "
                                                 "stored on another node.");

    mDefectRecoveryCheckBox->setChecked(false);
    mCorruptionRecoveryCheckBox->setChecked(false);
    mDefectRecoveryGeneralRadioButton->setChecked(false);
    mDefectRecoveryRestrictedRadioButton->setChecked(true);  // default mode
    mDefectRecoveryCustomRadioButton->setChecked(false);
    mCorruptionRecoveryGeneralRadioButton->setChecked(false);
    mCorruptionRecoveryRestrictedRadioButton->setChecked(true);  // default mode
    mCorruptionRecoveryCustomRadioButton->setChecked(false);

    mDefectButtonGroup = new QButtonGroup();
    mDefectButtonGroup->addButton(mDefectRecoveryGeneralRadioButton);
    mDefectButtonGroup->addButton(mDefectRecoveryRestrictedRadioButton);
    mDefectButtonGroup->addButton(mDefectRecoveryCustomRadioButton);
    mCorruptionButtonGroup = new QButtonGroup();
    mCorruptionButtonGroup->addButton(mCorruptionRecoveryGeneralRadioButton);
    mCorruptionButtonGroup->addButton(mCorruptionRecoveryRestrictedRadioButton);
    mCorruptionButtonGroup->addButton(mCorruptionRecoveryCustomRadioButton);

    QObject::connect(mDefectRecoveryCheckBox,
                     SIGNAL(toggled(bool)),
                     this,
                     SLOT(toggleDefectRecoveryOptions(bool)));
    QObject::connect(mCorruptionRecoveryCheckBox,
                     SIGNAL(toggled(bool)),
                     this,
                     SLOT(toggleCorruptionRecoveryOptions(bool)));
    QObject::connect(mDefectRecoveryCustomRadioButton,
                     SIGNAL(toggled(bool)),
                     this,
                     SLOT(toggleDefectCustomTextEdit(bool)));
    QObject::connect(mCorruptionRecoveryCustomRadioButton,
                     SIGNAL(toggled(bool)),
                     this,
                     SLOT(toggleCorruptionCustomTextEdit(bool)));

    mIntrusionLineEdit = new QLineEdit;
    mIntrusionLineEdit->setFixedWidth(100);
    mFailureLineEdit = new QLineEdit;
    mFailureLineEdit->setFixedWidth(100);
    mSecurityLineEdit = new QLineEdit;
    mSecurityLineEdit->setFixedWidth(100);
    mDefectRecoveryLineEdit = new QLineEdit;
    mDefectRecoveryLineEdit->setFixedWidth(100);
    mCorruptionRecoveryLineEdit = new QLineEdit;
    mCorruptionRecoveryLineEdit->setFixedWidth(100);
    mIdLineEdit = new QLineEdit;
    mIdLineEdit->setFixedWidth(100);
    mCustomCorruptionRecoveryTextEdit = new QTextEdit;
    mCustomCorruptionRecoveryTextEdit->setFixedHeight(60);
    mCustomCorruptionRecoveryTextEdit->setPlaceholderText("n7=0 & n5=0 | n7=0 & n6=0 & n1=0");
    mCustomDefectRecoveryTextEdit = new QTextEdit;
    mCustomDefectRecoveryTextEdit->setFixedHeight(60);
    mCustomDefectRecoveryTextEdit->setPlaceholderText("n7=0 & n5=0 | n7=0 & n6=0 & n1=0");
    mEssentialTextEdit = new QTextEdit;
    mEssentialTextEdit->setPlaceholderText("(n14=0 | n15=0) & n16=0");
    mEssentialTextEdit->setFixedHeight(50);

    mIntrusionLabel->setBuddy(mIntrusionLineEdit);
    mFailureLabel->setBuddy(mFailureLineEdit);
    mSecurityLabel->setBuddy(mSecurityLineEdit);
    mDefectRecoveryLabel->setBuddy(mDefectRecoveryLineEdit);
    mCorruptionRecoveryLabel->setBuddy(mCorruptionRecoveryLineEdit);
    mIdLabel->setBuddy(mIdLineEdit);
    mEssentialLabel->setBuddy(mEssentialTextEdit);

    mErrorLabel->setFixedWidth(230);
    mErrorLabel->setWordWrap(true);
    mIntrusionLabel->setFixedWidth(150);  // for better alignment
    mFailureLabel->setFixedWidth(150);
    mSecurityLabel->setFixedWidth(150);
    mDefectRecoveryLabel->setFixedWidth(150);
    mDefectRecoveryModeLabel->setFixedWidth(230);
    mDefectRecoveryModeLabel->setAlignment(Qt::AlignCenter);
    mCorruptionRecoveryLabel->setFixedWidth(150);
    mCorruptionRecoveryModeLabel->setFixedWidth(230);
    mCorruptionRecoveryModeLabel->setAlignment(Qt::AlignCenter);
    mIdLabel->setFixedWidth(150);
    mEssentialLabel->setFixedWidth(150);

    mIdLineEdit->setModified(true);
    if (mAssociatedNode->getComponentType() == graph::ComponentType::environmentNode)
    {
        disableEnableHelper(true);
        mModuleChooserLayout->disable(true);
        mEssentialTextEdit->setDisabled(true);
    }

    mIntrusionLineEdit->setModified(false);
    mFailureLineEdit->setModified(false);
    mSecurityLineEdit->setModified(false);
    mDefectRecoveryLineEdit->setModified(false);
    mCorruptionRecoveryLineEdit->setModified(false);
    mDefectRecoveryLineEdit->setModified(false);
    mEssentialTextEdit->document()->setModified(
            false);  // only set true if its NOT an environment node
    mCustomDefectRecoveryTextEdit->document()->setModified(false);
    mCustomCorruptionRecoveryTextEdit->document()->setModified(false);

    // BUTTONS
    mSaveButton = new QPushButton(tr("Save"));
    mCancelButton = new QPushButton(tr("Cancel"));
    mCancelButton->setDefault(true);

    mButtonBox = new QDialogButtonBox(Qt::Horizontal);
    mButtonBox->addButton(mSaveButton, QDialogButtonBox::ActionRole);
    mButtonBox->addButton(mCancelButton, QDialogButtonBox::ActionRole);

    connect(mSaveButton, &QPushButton::clicked, this, &NodeSettings::save);
    connect(mCancelButton, &QPushButton::clicked, this, &NodeSettings::cancel);
}

void
NodeSettings::createLayout()
{
    // Layouts
    QHBoxLayout* errorLayout = new QHBoxLayout;
    errorLayout->addWidget(mErrorLabel);

    QHBoxLayout* intrusionLayout = new QHBoxLayout;
    intrusionLayout->addWidget(mIntrusionLabel);
    intrusionLayout->addWidget(mIntrusionLineEdit);

    QHBoxLayout* failureLayout = new QHBoxLayout;
    failureLayout->addWidget(mFailureLabel);
    failureLayout->addWidget(mFailureLineEdit);

    QHBoxLayout* securityLayout = new QHBoxLayout;
    securityLayout->addWidget(mSecurityLabel);
    securityLayout->addWidget(mSecurityLineEdit);

    QHBoxLayout* idLayout = new QHBoxLayout;
    idLayout->addWidget(mIdLabel);
    idLayout->addWidget(mIdLineEdit);

    QHBoxLayout* corrModeLayout = new QHBoxLayout;
    // corrModeLayout->addSpacing(20);
    corrModeLayout->addWidget(mCorruptionRecoveryGeneralRadioButton);
    corrModeLayout->addWidget(mCorruptionRecoveryRestrictedRadioButton);
    corrModeLayout->addWidget(mCorruptionRecoveryCustomRadioButton);
    QHBoxLayout* corruptionTextLayout = new QHBoxLayout();
    corruptionTextLayout->addSpacing(20);
    corruptionTextLayout->addWidget(mCustomCorruptionRecoveryTextEdit);
    QVBoxLayout* corrModeParentLayout = new QVBoxLayout;
    corrModeParentLayout->addWidget(mCorruptionRecoveryModeLabel);
    corrModeParentLayout->addLayout(corrModeLayout);
    corrModeParentLayout->addLayout(corruptionTextLayout);
    corrModeParentLayout->addStretch();
    mCustomCorruptionRecoveryTextEdit->hide();

    QHBoxLayout* defModeLayout = new QHBoxLayout;
    defModeLayout->addWidget(mDefectRecoveryGeneralRadioButton);
    defModeLayout->addWidget(mDefectRecoveryRestrictedRadioButton);
    defModeLayout->addWidget(mDefectRecoveryCustomRadioButton);
    QHBoxLayout* defectTextLayout = new QHBoxLayout();
    defectTextLayout->addWidget(mCustomDefectRecoveryTextEdit);
    QVBoxLayout* defModeParentLayout = new QVBoxLayout;
    defModeParentLayout->addWidget(mDefectRecoveryModeLabel);
    defModeParentLayout->addLayout(defModeLayout);
    defModeParentLayout->addLayout(defectTextLayout);
    defModeParentLayout->addStretch();
    mCustomDefectRecoveryTextEdit->hide();

    QHBoxLayout* recoverySafetyLayout = new QHBoxLayout();
    recoverySafetyLayout->addWidget(mDefectRecoveryLabel);
    recoverySafetyLayout->addWidget(mDefectRecoveryLineEdit);
    mDefectRecoveryLabel->hide();
    mDefectRecoveryLineEdit->hide();
    QHBoxLayout* recoverySecurityLayout = new QHBoxLayout();
    recoverySecurityLayout->addWidget(mCorruptionRecoveryLabel);
    recoverySecurityLayout->addWidget(mCorruptionRecoveryLineEdit);
    mCorruptionRecoveryLabel->hide();
    mCorruptionRecoveryLineEdit->hide();

    QVBoxLayout* checkBoxLeftLayout = new QVBoxLayout;
    checkBoxLeftLayout->addWidget(mDefectRecoveryCheckBox);
    checkBoxLeftLayout->addLayout(recoverySafetyLayout);
    checkBoxLeftLayout->addLayout(defModeParentLayout);
    QVBoxLayout* checkBoxRightLayout = new QVBoxLayout;
    checkBoxRightLayout->addWidget(mCorruptionRecoveryCheckBox);
    checkBoxRightLayout->addLayout(recoverySecurityLayout);
    checkBoxRightLayout->addLayout(corrModeParentLayout);
    mCorruptionRecoveryModeLabel->hide();
    mDefectRecoveryModeLabel->hide();
    mCorruptionRecoveryRestrictedRadioButton->hide();
    mCorruptionRecoveryCustomRadioButton->hide();
    mCorruptionRecoveryGeneralRadioButton->hide();
    mDefectRecoveryRestrictedRadioButton->hide();
    mDefectRecoveryCustomRadioButton->hide();
    mDefectRecoveryGeneralRadioButton->hide();

    QVBoxLayout* essentialLayout = new QVBoxLayout;
    essentialLayout->addWidget(mEssentialLabel);
    essentialLayout->addWidget(mEssentialTextEdit);

    QVBoxLayout* topLayout = new QVBoxLayout;
    topLayout->addLayout(errorLayout);
    topLayout->addLayout(mModuleChooserLayout);
    QVBoxLayout* leftLayout = new QVBoxLayout;
    leftLayout->addLayout(intrusionLayout);
    leftLayout->addLayout(idLayout);
    leftLayout->addLayout(checkBoxLeftLayout);
    QVBoxLayout* rightLayout = new QVBoxLayout;
    rightLayout->addLayout(failureLayout);
    rightLayout->addLayout(securityLayout);
    rightLayout->addLayout(checkBoxRightLayout);
    rightLayout->setContentsMargins(12, 0, 0, 0);
    QVBoxLayout* botLayout = new QVBoxLayout;
    botLayout->addLayout(essentialLayout);

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(topLayout, 0, 0, 1, 2);
    mainLayout->addLayout(leftLayout, 1, 0);
    mainLayout->addLayout(rightLayout, 1, 1);
    mainLayout->addLayout(botLayout, 2, 0, 1, 2);
    mainLayout->addWidget(mButtonBox, 3, 0, 2, 2);
    mainLayout->setRowStretch(2, 2);

    setLayout(mainLayout);
}

void
NodeSettings::updateLabels()
{
    switch (Model::getInstance().getType())
    {
        case Model::MDP:
            mIntrusionLabel->setText(tr("Intrusion &Probability:"));
            mFailureLabel->setText(tr("Failure &Probability:"));
            mSecurityLabel->setText(tr("Security &Probability:"));
            mDefectRecoveryLabel->setText(tr("Recovery &Probability:"));
            mCorruptionRecoveryLabel->setText(tr("Recovery &Probability:"));
            break;
        case Model::CTMC:
            mIntrusionLabel->setText(tr("Intrusion &Rate:"));
            mFailureLabel->setText(tr("Failure &Rate:"));
            mSecurityLabel->setText(tr("Security &Rate:"));
            mDefectRecoveryLabel->setText(tr("Recovery &Rate:"));
            mCorruptionRecoveryLabel->setText(tr("Recovery &Rate:"));
            break;
        default: break;
    }
}

void
NodeSettings::initializeFields()
{
    QString valueAsString;
    
    mModuleChooserLayout->setSimulationPath(mAssociatedNode->getSimulationPath(), mAssociatedNode->isSimulationModule());
    mModuleChooserLayout->setErisPath(mAssociatedNode->getSubmodulePath(), mAssociatedNode->isErisModule());

    //toggleSimulationMode();     
    valueAsString = QString::number(mAssociatedNode->getId());
    mIdLineEdit->setText(valueAsString);  // resets isModified flag
    mIdLineEdit->setStyleSheet("QTextEdit {background-color: white;}");
    
    valueAsString = mAssociatedNode->getIntrusionIndicator();
    mIntrusionLineEdit->setText(valueAsString);  // resets isModified flag
    mIntrusionLineEdit->setStyleSheet("QLineEdit {background-color: white;}");

    valueAsString = mAssociatedNode->getFailureIndicator();
    mFailureLineEdit->setText(valueAsString);
    mFailureLineEdit->setStyleSheet("QLineEdit {background-color: white;}");
    

    valueAsString = mAssociatedNode->getSecurityIndicator();
    mSecurityLineEdit->setText(valueAsString);
    
    mSecurityLineEdit->setStyleSheet("QLineEdit {background-color: white;}");

    mDefectRecoveryCheckBox->setChecked(mAssociatedNode->isRecoverableFromDefect());
    valueAsString = mAssociatedNode->getDefectRecoveryIndicator();
    mDefectRecoveryLineEdit->setText(valueAsString);
    mDefectRecoveryLineEdit->setStyleSheet("QLineEdit {background-color: white;}");
    toggleDefectRecoveryOptions(mAssociatedNode->isRecoverableFromDefect());

    if (mAssociatedNode->getDefectRecoveryStrategy() == Recovery::Strategy::general)
    {
        mDefectRecoveryGeneralRadioButton->setChecked(true);
    }
    else if (mAssociatedNode->getDefectRecoveryStrategy() == Recovery::Strategy::restricted)
    {
        mDefectRecoveryRestrictedRadioButton->setChecked(true);
    }
    else if (mAssociatedNode->getDefectRecoveryStrategy() == Recovery::Strategy::custom)
    {
        mDefectRecoveryCustomRadioButton->setChecked(true);
    }
    
    mCustomDefectRecoveryTextEdit->setText(
            mAssociatedNode->getCustomDefectRecoveryFormula());
    mCustomDefectRecoveryTextEdit->setStyleSheet("QTextEdit {background-color: white;}");
    mCustomDefectRecoveryTextEdit->document()->setModified(false);

    
    mCorruptionRecoveryCheckBox->setChecked(mAssociatedNode->isRecoverableFromCorruption());
    valueAsString = mAssociatedNode->getCorruptionRecoveryIndicator();
    mCorruptionRecoveryLineEdit->setText(valueAsString);
    mCorruptionRecoveryLineEdit->setStyleSheet("QLineEdit {background-color: white;}");
    toggleCorruptionRecoveryOptions(mAssociatedNode->isRecoverableFromCorruption());
    if (mAssociatedNode->getCorruptionRecoveryStrategy() == Recovery::Strategy::general)
    {
        mCorruptionRecoveryGeneralRadioButton->setChecked(true);
    }
    else if (mAssociatedNode->getCorruptionRecoveryStrategy() == Recovery::Strategy::restricted)
    {
        mCorruptionRecoveryRestrictedRadioButton->setChecked(true);
    }
    else if (mAssociatedNode->getCorruptionRecoveryStrategy() == Recovery::Strategy::custom)
    {
        mCorruptionRecoveryCustomRadioButton->setChecked(true);
    }

    mCustomCorruptionRecoveryTextEdit->setText(
            mAssociatedNode->getCustomCorruptionRecoveryFormula());
    mCustomCorruptionRecoveryTextEdit->setStyleSheet("QTextEdit {background-color: "
                                                     "white;}");
    mCustomCorruptionRecoveryTextEdit->document()->setModified(false);

    mEssentialTextEdit->setText(mAssociatedNode->getEssentialNodes());
    mEssentialTextEdit->setStyleSheet("QTextEdit {background-color: white;}");
    mEssentialTextEdit->document()->setModified(false);
    mErrorLabel->setText("");
}

void
NodeSettings::openWindow()
{
    initializeFields();
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->show();
}

bool
NodeSettings::checkId()
{
    if (mIdLineEdit->isModified())
    {
        mModified = true;
        graph::GraphicScene* scene = dynamic_cast<graph::GraphicScene*>(mAssociatedNode->scene());
        ERIS_CHECK(scene);
        if (utils::isValidNewId(mIdLineEdit->text().toStdString(),
                         std::to_string(mAssociatedNode->getId()),
                         scene->getCounter()))
        {
            mAssociatedNode->setId(mIdLineEdit->text().toInt());
            mIdLineEdit->setStyleSheet("QLineEdit {background-color: white;}");
            mIdLineEdit->setModified(false);
            return true;
        }
        else
        {
            mIdLineEdit->setStyleSheet("QLineEdit {background-color: red;}");
            return false;
        }
    }
    return true;
}

bool
NodeSettings::checkSimulationInput()
{
    if (mModuleChooserLayout->isModified())
    {
        mModified = true;
        QString curr = mModuleChooserLayout->getSimulationPath();

        if (mModuleChooserLayout->isSimulation())
        {// Radio Button is on
            if (utils::isValidSimulationPath(curr, mAssociatedNode->getId()))
            {
                mAssociatedNode->setSimulated(mModuleChooserLayout->isSimulation(), curr);
                return true;
            }
            else
            {
               // mSimulationLineEdit->setStyleSheet("QLineEdit {background-color: red;}");
                return false;            
            }
        }
        else
        {
            mAssociatedNode->setSimulated(mModuleChooserLayout->isSimulation(), curr); 
            // does not require check, as only empty case is viewed
            return true;
        }
    }
    return true;
}

bool
NodeSettings::checkSubmoduleInput()
{
    if (mModuleChooserLayout->isModified())
    {
        mModified = true;
        QString curr = mModuleChooserLayout->getErisPath();

        if (mModuleChooserLayout->isEris())
        {// Radio Button is on
            if (utils::isValidSubmodulePath(curr, mAssociatedNode->getId()))
            {
                mAssociatedNode->setSubmodule(mModuleChooserLayout->isEris(), curr);
                return true;
            }
            else
            {
               // mSimulationLineEdit->setStyleSheet("QLineEdit {background-color: red;}");
                return false;            
            }
        }
        else
        {
            mAssociatedNode->setSubmodule(mModuleChooserLayout->isEris(), curr); // does not require check, as only empty case is viewed
            return true;
        }
    }
    return true;
}

bool
NodeSettings::checkIntrusionInput()
{
    if (mIntrusionLineEdit->isModified())
    {
        mModified = true;
        QString curr = mIntrusionLineEdit->text();
        if (curr.isEmpty())
        {
            mAssociatedNode->setIntrusionIndicator("0");
            mIntrusionLineEdit->setStyleSheet("QLineEdit {background-color: white;}");
            mIntrusionLineEdit->setModified(false);
            return true;
        }
        else if (utils::isValidInputValue(curr.toStdString(), "Intrusion", Model::getInstance().getType()))
        {
            mAssociatedNode->setIntrusionIndicator(curr);
            mIntrusionLineEdit->setStyleSheet("QLineEdit {background-color: white;}");
            mIntrusionLineEdit->setModified(false);
            return true;
        }
        else
        {  // some problem occurred and the error string was set
            mIntrusionLineEdit->setStyleSheet("QLineEdit {background-color: red;}");
            return false;
        }
    }
    return true;
}

bool
NodeSettings::checkFailureInput()
{
    if (mFailureLineEdit->isModified())
    {
        mModified = true;
        QString curr = mFailureLineEdit->text();

        if (curr.isEmpty())
        {
            mAssociatedNode->setFailureIndicator("0");
            mFailureLineEdit->setStyleSheet("QLineEdit {background-color: white;}");
            mFailureLineEdit->setModified(false);
            return true;
        }
        else if (utils::isValidInputValue(curr.toStdString(), "Failure", Model::getInstance().getType()))
        {
            mAssociatedNode->setFailureIndicator(curr);
            mFailureLineEdit->setStyleSheet("QLineEdit {background-color: white;}");
            mFailureLineEdit->setModified(false);
            return true;
        }
        else
        {
            mFailureLineEdit->setStyleSheet("QLineEdit {background-color: red;}");
            return false;
        }
    }
    return true;
}

bool
NodeSettings::checkSecurityInput()
{
    if (mSecurityLineEdit->isModified())
    {
        mModified = true;
        QString curr = mSecurityLineEdit->text();
        if (curr.isEmpty())
        {
            mAssociatedNode->setSecurityIndicator("0");
            mSecurityLineEdit->setStyleSheet("QLineEdit {background-color: white;}");
            mSecurityLineEdit->setModified(false);
            return true;
        }
        else if (utils::isValidInputValue(curr.toStdString(), "Security", Model::getInstance().getType()))
        {
            mAssociatedNode->setSecurityIndicator(curr);
            mSecurityLineEdit->setStyleSheet("QLineEdit {background-color: white;}");
            mSecurityLineEdit->setModified(false);
            return true;
        }
        else
        {
            mSecurityLineEdit->setStyleSheet("QLineEdit {background-color: red;}");
            return false;
        }
    }
    return true;
}

bool
NodeSettings::checkDefectRecoveryInput()
{
    mAssociatedNode->recoverableFromDefect(this->mDefectRecoveryCheckBox->isChecked());
    if (mDefectRecoveryCheckBox->isChecked())
    {
        mModified = true;
        if (mDefectRecoveryGeneralRadioButton->isChecked())
        {
            mAssociatedNode->setDefectRecoveryStrategy(Recovery::Strategy ::general);
        }
        else if (mDefectRecoveryRestrictedRadioButton->isChecked())
        {
            mAssociatedNode->setDefectRecoveryStrategy(Recovery::Strategy ::restricted);
        }
        else if (mDefectRecoveryCustomRadioButton->isChecked())
        {
            mAssociatedNode->setDefectRecoveryStrategy(Recovery::Strategy ::custom);
            if (mCustomDefectRecoveryTextEdit->document()->isModified())
            {
                QString input = mCustomDefectRecoveryTextEdit->toPlainText();
                if (utils::isValidRecoveryFormula(input.toStdString(), mAssociatedNode->scene()))
                {
                    input.replace(QRegularExpression("\\s+"),
                                  " ");  // remove unnecessary whitespaces
                    input = input.trimmed();
                    mCustomDefectRecoveryTextEdit->setStyleSheet("QTextEdit {background-color: "
                                                                 "white;}");
                    mCustomDefectRecoveryTextEdit->document()->setModified(
                            false);  // reset because its correct!
                    mAssociatedNode->setCustomDefectRecoveryFormula(input);
                }
                else
                {
                    mCustomDefectRecoveryTextEdit->setStyleSheet("QTextEdit {background-color: "
                                                                 "red;}");
                    return false;
                }
            }
        }

        QString curr = mDefectRecoveryLineEdit->text();
        if (curr.isEmpty())
        {
            mAssociatedNode->setDefectRecoveryIndicator("0");
            mDefectRecoveryLineEdit->setStyleSheet("QLineEdit {background-color: white;}");
            mDefectRecoveryLineEdit->setModified(false);
        }
        else if (utils::isValidInputValue(
                         curr.toStdString(), "Defect Recovery", Model::getInstance().getType()))
        {
            mAssociatedNode->setDefectRecoveryIndicator(curr);
            mDefectRecoveryLineEdit->setStyleSheet("QLineEdit {background-color: white;}");
            mDefectRecoveryLineEdit->setModified(false);
        }
        else
        {
            mDefectRecoveryLineEdit->setStyleSheet("QLineEdit {background-color: red;}");
            return false;
        }
    }
    return true;
}

bool
NodeSettings::checkCorruptionRecoveryInput()
{
    mAssociatedNode->recoverableFromCorruption(mCorruptionRecoveryCheckBox->isChecked());
    if (mCorruptionRecoveryCheckBox->isChecked())
    {
        mModified = true;
        if (mCorruptionRecoveryGeneralRadioButton->isChecked())
        {
            mAssociatedNode->setCorruptionRecoveryStrategy(Recovery::Strategy ::general);
        }
        else if (mCorruptionRecoveryRestrictedRadioButton->isChecked())
        {
            mAssociatedNode->setCorruptionRecoveryStrategy(Recovery::Strategy ::restricted);
        }
        else if (mCorruptionRecoveryCustomRadioButton->isChecked())
        {
            mAssociatedNode->setCorruptionRecoveryStrategy(Recovery::Strategy ::custom);
            if (mCustomCorruptionRecoveryTextEdit->document()->isModified())
            {
                QString input = mCustomCorruptionRecoveryTextEdit->toPlainText();
                if (utils::isValidRecoveryFormula(input.toStdString(), mAssociatedNode->scene()))
                {
                    input.replace(QRegularExpression("\\s+"), " ");  // removes whitespaces
                    input = input.trimmed();
                    mCustomCorruptionRecoveryTextEdit->setStyleSheet("QTextEdit {background-color: "
                                                                     "white;}");
                    mCustomCorruptionRecoveryTextEdit->document()->setModified(
                            false);  // reset because its correct!
                    mAssociatedNode->setCustomCorruptionRecoveryFormula(input);
                }
                else
                {
                    mCustomCorruptionRecoveryTextEdit->setStyleSheet("QTextEdit {background-color: "
                                                                     "red;}");
                    return false;
                }
            }
        }

        if (mCorruptionRecoveryLineEdit->isModified())
        {
            mModified = true;
            QString curr = mCorruptionRecoveryLineEdit->text();
            if (curr.isEmpty())
            {
                mAssociatedNode->setCorruptionRecoveryIndicator("0");
                mCorruptionRecoveryLineEdit->setStyleSheet("QLineEdit {background-color: white;}");
                mCorruptionRecoveryLineEdit->setModified(false);
            }
            else if (utils::isValidInputValue(curr.toStdString(),
                                       "Corruption Recovery",
                                       Model::getInstance().getType()))
            {
                mAssociatedNode->setCorruptionRecoveryIndicator(curr);
                mCorruptionRecoveryLineEdit->setStyleSheet("QLineEdit {background-color: white;}");
                mCorruptionRecoveryLineEdit->setModified(false);
            }
            else
            {
                mCorruptionRecoveryLineEdit->setStyleSheet("QLineEdit {background-color: red;}");
                return false;
            }
        }
    }
    return true;
}

bool
NodeSettings::checkEssentialInput()
{
    if (mEssentialTextEdit->document()->isModified())
    {
        mModified = true;
        QString input = mEssentialTextEdit->toPlainText();
        if (utils::isValidEssentialNodes(input.toStdString(), mAssociatedNode))
        {
            input.replace(QRegularExpression("\\s+"), " ");  // remove unnecessary whitespaces
            input = input.trimmed();
            mAssociatedNode->setEssentialNodes(input);
            mEssentialTextEdit->setStyleSheet("QTextEdit {background-color: white;}");
            mEssentialTextEdit->document()->setModified(false);  // reset because its correct!
            return true;
        }
        else
        {
            mEssentialTextEdit->setStyleSheet("QTextEdit {background-color: red;}");
            return false;
        }
    }
    return true;
}

void
NodeSettings::save()
{
    ErrorHandler& errorHandler = ErrorHandler::getInstance();
    errorHandler.clear();

    bool sound = true;
    mModified = false;

    //mAssociatedNode->setSubmodule(submoduleLayout->submodule_path());

    sound = checkId();
    if (sound && mAssociatedNode->getComponentType() != graph::ComponentType::environmentNode)
    // ID was fine, some fields depend on a correct ID! exclude env nodes
    {
        sound &= checkSimulationInput();
        sound &= checkSubmoduleInput();
        sound &= checkIntrusionInput();
        sound &= checkFailureInput();
        sound &= checkSecurityInput();
        sound &= checkDefectRecoveryInput();
        sound &= checkCorruptionRecoveryInput();
        sound &= checkEssentialInput();
    }

    if (sound)
    {
        if (mModified)
        {
            graph::GraphicScene* scene =
                    dynamic_cast<graph::GraphicScene*>(mAssociatedNode->scene());
            ERIS_CHECK(scene);
            scene->setStateChanged(true);
            // DeprecatedSceneStatus::getInstance().setChanged(true);
        }
        mErrorLabel->setText("");
        this->close();
    }
    else
    {
        mErrorLabel->setText(errorHandler.getErrorCollection());
    }
}

void
NodeSettings::cancel()
{  // cancel and close
    this->close();
}


}  // namespace widgets
