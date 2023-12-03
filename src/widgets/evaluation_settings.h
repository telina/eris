/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
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

#ifndef ERIS_WIDGETS_EVALUATION_SETTINGS_H
#define ERIS_WIDGETS_EVALUATION_SETTINGS_H

#include "experiment.h"

#include <QDialog>
#include <QSlider>

QT_BEGIN_NAMESPACE
class QFormLayout;
class QCheckBox;
class QLabel;
class QSpinBox;
QT_END_NAMESPACE

namespace widgets
{
   
class EditorWidget;
// Just like a normal slider but it shows the current
// value in a tooltip.
class FancySlider : public QSlider
{
    Q_OBJECT
public:
    explicit FancySlider(QWidget* parent = nullptr);
    explicit FancySlider(Qt::Orientation orientation, QWidget* parent = nullptr);

protected:
    void
    sliderChange(SliderChange change) override;
};

// Wrapper widget around the editor to control it
// and extract the experimentDocument when needed.
class EvaluationSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    static EvaluationSettingsDialog*
    Create(QWidget* parent);
    
    static EvaluationSettingsDialog*
    Get();

    EvaluationSettingsDialog() = delete;
    ~EvaluationSettingsDialog() override;

    // Notice that both parameters here will be overridden
    // once this method is called.
    void
    experimentDocument(QString& output, eval::ExperimentInterval* interval);

private slots:
    
    void
    dialogClosed(int result);
    
    void
    updateEditor(int);
    
    void
    updateInterval(int);
    
    void
    updateIntervalSteps(int);

private:
    
    explicit EvaluationSettingsDialog(QWidget* parent);

private:
    QFormLayout* formLayout;
    EditorWidget* editor;
    QCheckBox* systemFailureButton;
    QCheckBox* defectiveButton;
    QCheckBox* corruptedButton;
    FancySlider* intervalSlider;
    QLabel* intervalLabel;
    QSpinBox* intervalSteps;
    QLabel* intervalStepsLabel;
};

}  // namespace widgets

#endif  // ERIS_WIDGETS_EVALUATION_SETTINGS_H