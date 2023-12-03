/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
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
#include "module_chooser_layout.h"


namespace widgets
{

ModuleChooserLayout::ModuleChooserLayout(QWidget* parent) :
    QGridLayout(nullptr), mParent(parent), mRadioButtonModified(false)
{   
    mSimulationFileExtension = ".m";
    mErisFileExtension = ".xml";
    createLayout();
}

ModuleChooserLayout::~ModuleChooserLayout()
{

}

void
ModuleChooserLayout::createLayout()
{
    mLabel = new QLabel("Module:");
    mRadioButtonOff = new QRadioButton("Off");
    mRadioButtonSimulation = new QRadioButton("Simulation");
    mRadioButtonEris = new QRadioButton("ERIS");

    mRadioButtonOff->setChecked(true);
    mRadioButtonSimulation->setChecked(false);
    mRadioButtonEris->setChecked(false);

    mPreviousRadioButtonState = mRadioButtonOff->isChecked();
    mSimulationLineEdit = new QLineEdit();
    mErisLineEdit = new QLineEdit();
    mButtonGroup = new QButtonGroup();
    mButtonGroup->addButton(mRadioButtonOff);
    mButtonGroup->addButton(mRadioButtonSimulation);
    mButtonGroup->addButton(mRadioButtonEris);
    mSimulationLineEdit->hide();
    mErisLineEdit->hide();

    mSimulationLineEdit->setFixedWidth(250);
    QSizePolicy policy = mSimulationLineEdit->sizePolicy();
    policy.setRetainSizeWhenHidden(true);
    mSimulationLineEdit->setSizePolicy(policy);

    mErisLineEdit->setFixedWidth(250);
    policy = mErisLineEdit->sizePolicy();
    policy.setRetainSizeWhenHidden(true);
    mErisLineEdit->setSizePolicy(policy);


    mSimulationLineEdit->setPlaceholderText("/home/user/model" + mSimulationFileExtension);
    connect(mSimulationLineEdit,
            &QLineEdit::textChanged,
            this,
            &ModuleChooserLayout::onSimulationPathChange);

    mErisLineEdit->setPlaceholderText("/home/user/model" + mErisFileExtension);
    connect(mErisLineEdit,
            &QLineEdit::textChanged,
            this,
            &ModuleChooserLayout::onErisPathChange);

    QObject::connect(mButtonGroup,
                     SIGNAL(buttonClicked(int)),
                     this,
                     SLOT(updateLayout(int)));

    mSearchFileButton = new QPushButton("Search");
    policy = mSearchFileButton->sizePolicy();
    policy.setRetainSizeWhenHidden(true);
    mSearchFileButton->setSizePolicy(policy);
    connect(mSearchFileButton,
            &QPushButton::clicked,
            this,
            &ModuleChooserLayout::onSearchFile);
    mSearchFileButton->hide();
    
    addWidget(mLabel, 0, 0);
    addWidget(mRadioButtonOff, 0, 1);
    addWidget(mRadioButtonSimulation, 0, 2);
    addWidget(mRadioButtonEris, 0, 3);
    addWidget(mSimulationLineEdit, 0, 4);
    addWidget(mErisLineEdit, 0, 4);
    addWidget(mSearchFileButton, 0, 5);
    //addStretch();
    
}

bool
ModuleChooserLayout::isModified()
{
    if (pathIsModified() || mRadioButtonModified)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
ModuleChooserLayout::pathIsModified()
{
    if (mSimulationLineEdit->isModified() || mErisLineEdit->isModified())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool
ModuleChooserLayout::isSimulation()
{
    return mRadioButtonSimulation->isChecked();
}

bool
ModuleChooserLayout::isEris()
{
    return mRadioButtonEris->isChecked();
}

void
ModuleChooserLayout::disable(bool checked)
{
    mRadioButtonSimulation->setDisabled(checked);
    mRadioButtonEris->setDisabled(checked);
    mRadioButtonOff->setDisabled(checked);
    mSimulationLineEdit->setDisabled(checked);
    mErisLineEdit->setDisabled(checked);
}

void
ModuleChooserLayout::onSearchFile(bool /*checked*/)
{
    QLineEdit* currentLineEdit;
    QString extension;
    if (isSimulation())
    {
        extension = mSimulationFileExtension;
        currentLineEdit = mSimulationLineEdit;
    }
    else if (isEris())
    {
        extension = mErisFileExtension;
        currentLineEdit = mErisLineEdit;
    }
    else
    {// Error
        return;
    }

    QString tmp = QFileDialog::getOpenFileName(
            MainWindow::getInstance(), "Open File", /*QDir::currentPath()*/ "", 
            "All Files (*" + extension + ")");
    if (!tmp.isNull())
    {
        currentLineEdit->setText(tmp);
        currentLineEdit->setModified(true);
    }

    if (mParent != nullptr)
    {
        mParent->raise();
    }
}


void
ModuleChooserLayout::updateLayout(int /*button_id*/)
{

    mSimulationLineEdit->setHidden(!mRadioButtonSimulation->isChecked());
    mErisLineEdit->setHidden(!mRadioButtonEris->isChecked());
    mSearchFileButton->setHidden(mRadioButtonOff->isChecked());

    if (mPreviousRadioButtonState != mRadioButtonOff->isChecked())
    {
        mRadioButtonModified = true;
    }
    mPreviousRadioButtonState = mRadioButtonOff->isChecked();
}

void
ModuleChooserLayout::onSimulationPathChange(const QString& text)
{
    onPathChange(mSimulationLineEdit,text);
}

void
ModuleChooserLayout::onErisPathChange(const QString& text)
{
    onPathChange(mErisLineEdit, text);
}

void
ModuleChooserLayout::onPathChange(QLineEdit* lineEdit, const QString& text)
{
    QPalette palette;

    if (QFile::exists(text))
    {
        palette.setColor(QPalette::Text, Qt::black);
    }
    else
    {
        palette.setColor(QPalette::Text, Qt::red);
    }
    lineEdit->setPalette(palette);
}

QString
ModuleChooserLayout::getSimulationPath() const
{
    return mSimulationLineEdit->text();
}

void
ModuleChooserLayout::setSimulationPath(const QString& path, bool state)
{
    if (!path.isEmpty())
    {// are empty paths not allowed?
        mRadioButtonSimulation->setChecked(state);
        updateLayout(0);
        mSimulationLineEdit->setText(path);
    }
}

QString
ModuleChooserLayout::getErisPath() const
{
    return mErisLineEdit->text();
}

void
ModuleChooserLayout::setErisPath(const QString& path, bool state)
{
    if (!path.isEmpty())
    {// are empty paths not allowed?
        mRadioButtonEris->setChecked(state);
        updateLayout(0);
        mErisLineEdit->setText(path);
    }
}


}