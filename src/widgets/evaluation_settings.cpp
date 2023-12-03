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

#include "evaluation_settings.h"

#include "logger.h"
#include "tokenizer.h"
#include "editor_widget.h"

#include <QGridLayout>
#include <QRegularExpression>
#include <QStyleOptionSlider>
#include <QToolTip>
#include <QFormLayout>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>

#define SYSTEMFAILURE "P=? [ F[T,T] \"systemfailure\" ]"
#define DEFECTIVE "P=? [ F[T,T] \"defective\" ]"
#define CORRUPTED "P=? [ F[T,T] \"corrupted\" ]"

static widgets::EvaluationSettingsDialog* instance = nullptr;

namespace widgets
{
    
FancySlider::FancySlider(QWidget* parent) : QSlider(parent)
{
    
}

FancySlider::FancySlider(Qt::Orientation orientation, QWidget* parent) :
    QSlider(orientation, parent)
{
    
}

void
FancySlider::sliderChange(QAbstractSlider::SliderChange change)
{
    QSlider::sliderChange(change);

    if (change == QAbstractSlider::SliderValueChange)
    {
        QStyleOptionSlider opt;
        initStyleOption(&opt);

        QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
        QPoint bottomRightCorner = sr.bottomLeft();

        QToolTip::showText(mapToGlobal(QPoint(bottomRightCorner.x(), bottomRightCorner.y())),
                           QString::number(value()),
                           this);
    }
}

EvaluationSettingsDialog::EvaluationSettingsDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle("Evaluation Settings");
    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint
                   | Qt::WindowCloseButtonHint);
    formLayout = new QFormLayout();
    editor = new EditorWidget();

    systemFailureButton = new QCheckBox("systemfailure");
    defectiveButton = new QCheckBox("defective");
    corruptedButton = new QCheckBox("corrupted");
    intervalSlider = new FancySlider(Qt::Horizontal);
    intervalSlider->setTickInterval(1);
    intervalSlider->setMinimum(1000);
    intervalSlider->setMinimum(0);

    systemFailureButton->setChecked(true);
    defectiveButton->setChecked(true);
    corruptedButton->setChecked(true);

    intervalLabel = new QLabel("Steps Interval [0,-1]");
    intervalSteps = new QSpinBox();
    intervalSteps->setMinimum(0);
    intervalSteps->setMaximum(1000);
    intervalStepsLabel = new QLabel("Steps [-1]");

    auto hboxLayout = new QHBoxLayout();
    hboxLayout->addWidget(systemFailureButton);
    hboxLayout->addWidget(defectiveButton);
    hboxLayout->addWidget(corruptedButton);

    formLayout->addRow("Experiment Properties  ", hboxLayout);
    formLayout->addWidget(editor);
    formLayout->addRow(intervalLabel, intervalSlider);
    formLayout->addRow(intervalStepsLabel, intervalSteps);

    QString defaultContent;
    defaultContent = "const double T;\n" SYSTEMFAILURE "\n" DEFECTIVE "\n"
                     "\n" CORRUPTED "\n";

    editor->setPlainText(defaultContent);

    QFont font = editor->font();
    font.setPointSize(14);
    editor->setFont(font);

    connect(systemFailureButton,
            &QCheckBox::stateChanged,
            this,
            &EvaluationSettingsDialog::updateEditor);
    connect(defectiveButton,
            &QCheckBox::stateChanged,
            this,
            &EvaluationSettingsDialog::updateEditor);
    connect(corruptedButton,
            &QCheckBox::stateChanged,
            this,
            &EvaluationSettingsDialog::updateEditor);
    connect(this, &QDialog::finished, this, &EvaluationSettingsDialog::dialogClosed);
    connect(intervalSlider,
            &QSlider::valueChanged,
            this,
            &EvaluationSettingsDialog::updateInterval);
    connect(intervalSteps, SIGNAL(valueChanged(int)), this, SLOT(updateIntervalSteps(int)));

    setLayout(formLayout);
    intervalSlider->setValue(2);
    intervalSteps->setValue(1);
}

EvaluationSettingsDialog::~EvaluationSettingsDialog() = default;

void
EvaluationSettingsDialog::updateEditor(int)
{
    editor->markAsUnchecked("systemfailure", !systemFailureButton->isChecked());
    editor->markAsUnchecked("defective", !defectiveButton->isChecked());
    editor->markAsUnchecked("corrupted", !corruptedButton->isChecked());
}

void
EvaluationSettingsDialog::experimentDocument(QString& output,
                                             eval::ExperimentInterval* interval)
{
    output.clear();
    QString input = editor->toPlainText();
    QStringList list = input.split("\n");
    QRegularExpression systemFailureReg(".*systemfailure.*");
    QRegularExpression defectiveReg(".*defective.*");
    QRegularExpression corruptedReg(".*corrupted.*");

    for (int i = 0; i < list.size(); ++i)
    {
        if (!systemFailureButton->isChecked() && systemFailureReg.match(list[i]).hasMatch())
        {
            continue;
        }
        if (!defectiveButton->isChecked() && defectiveReg.match(list[i]).hasMatch())
        {
            continue;
        }
        if (!corruptedButton->isChecked() && corruptedReg.match(list[i]).hasMatch())
        {
            continue;
        }
        output += list[i] + "\n";
    }

    interval->from = 0;
    interval->to = intervalSlider->value();
    interval->steps = intervalSteps->value();
}

void
EvaluationSettingsDialog::dialogClosed(int)
{
    /* QString doc;
     ExperimentInterval interval;
     experimentDocument(doc,interval);
     PRINT_INFO("\n%s",doc.toStdString().c_str());
     PRINT_INFO("Interval %s",interval.toString().c_str());*/
}

void
EvaluationSettingsDialog::updateInterval(int value)
{
    intervalLabel->setText(tr("Steps Interval [0,%1]").arg(value));
}

void
EvaluationSettingsDialog::updateIntervalSteps(int value)
{
    intervalStepsLabel->setText(tr("Steps [%1]").arg(value));
}

EvaluationSettingsDialog*
EvaluationSettingsDialog::Create(QWidget* parent)
{
    delete instance;
    instance = new EvaluationSettingsDialog(parent);
    return Get();
}

EvaluationSettingsDialog*
EvaluationSettingsDialog::Get()
{
    return instance;
}

}  // namespace widgets