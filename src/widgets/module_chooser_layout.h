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

#ifndef ERIS_WIDGETS_MODULE_CHOOSER_H
#define ERIS_WIDGETS_MODULE_CHOOSER_H

#include "eris_config.h"
#include "main_window.h"
#include "logger.h"

#include <QGridLayout>
#include <QFileDialog>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QButtonGroup>
#include <QPushButton>

namespace widgets
{
class ERIS_EXPORT ModuleChooserLayout : public QGridLayout
{
    Q_OBJECT

public:
    ERIS_DISALLOW_COPY_AND_ASSIGN(ModuleChooserLayout);
    
    explicit ModuleChooserLayout(QWidget* parent);
    
    ModuleChooserLayout();
    
    ~ModuleChooserLayout() override;

    QString
    getSimulationPath() const;

    void
    setSimulationPath(const QString& path, bool state /*on or off*/);

    QString
    getErisPath() const;

    void
    setErisPath(const QString& path, bool state /*on or off*/);
    
    bool
    isModified();
    
    bool
    pathIsModified();

    void
    disable(bool checked);

    bool
    isSimulation();

    bool
    isEris();
    
private slots:
    
    /**
     * Updates Configuration when RadioButton status changed. 
     * Off: no LineEdit displayed
     * Simulation: Simulation LineEdit displayed/active
     * Eris: Eris LineEdit displayed/active
     */
    void
    updateLayout(int button_id);
    
    /**
     * Opens search file dialogue on button click. Takes care of the file extension
     * depending on the current mode (simulation or eris).
     */
    void
    onSearchFile(bool checked);
    
    /** 
     * Triggered whenever the LineEdit for the Eris module has changed.
     */
    void
    onErisPathChange(const QString& path);

    /** 
     * Triggered whenever the LineEdit for the Simulation module has changed
     */
    void
    onSimulationPathChange(const QString& path);

private:
    
    /**
     * Internal helper method called by either Path Changed method. 
     */
    void
    onPathChange(QLineEdit* lineEdit, const QString& path);

    /**
     * Creates the basic layout and initializes fields.
     */
    void
    createLayout();

    bool mPreviousRadioButtonState;
    bool mRadioButtonModified;

    QLabel* mLabel;

    /**
     * Radiobuttons on, simulation model, eris model
     */
    QRadioButton* mRadioButtonOff;
    QRadioButton* mRadioButtonSimulation;
    QRadioButton* mRadioButtonEris;
    QButtonGroup* mButtonGroup;

    QPushButton* mSearchFileButton;

    QString mErisFileExtension;
    QString mSimulationFileExtension;

    /**
     * LineEdits for Simulation model and Eris model
     */
    QLineEdit* mSimulationLineEdit;
    QLineEdit* mErisLineEdit;

    /**
     * File extensions for the respective Eris/Simulation model
     */
    QString mSubmodulePath;
    QString mFileExtension;

    /** Parent widget, in this case the NodeSettings Window */
    QWidget* mParent;
};
}

#endif // ERIS_WIDGETS_MODULE_CHOOSER_H