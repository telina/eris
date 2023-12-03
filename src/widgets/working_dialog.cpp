/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
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

#include "working_dialog.h"
#include "logger.h"
#include "checks.h"

#include <QMessageBox>
#include <QPushButton>
#include <QSizePolicy>

namespace widgets
{
WorkingDialog::WorkingDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle(tr("Wait Working"));
    grid_layout_ = new QGridLayout();
    progress_bar_ = new QProgressBar();
    progress_bar_->setMinimumWidth(400);
    button_box_ = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Abort);
    output_widget_ = new OutputWidget();
    output_widget_->setMinimumSize(860, 600);
    setCursor(Qt::WaitCursor);
    button_box_->setCursor(Qt::PointingHandCursor);
    connect(button_box_, &QDialogButtonBox::rejected, this, &WorkingDialog::AskForAbort);
    connect(button_box_, &QDialogButtonBox::accepted, this, &WorkingDialog::accept);

    grid_layout_->addWidget(output_widget_);
    grid_layout_->addWidget(progress_bar_);
    grid_layout_->addWidget(button_box_);

    setLayout(grid_layout_);
}
void
WorkingDialog::ShowIt()
{
    setCursor(Qt::WaitCursor);
    button_box_->button(QDialogButtonBox::Abort)->setEnabled(true);
    setWindowTitle(tr("Wait Working"));
    if (!active_)
    {
        progress_bar_->setRange(0, 0);
    }
    else
    {
        progress_bar_->setRange(0, 100);
    }
    show();
}

bool
WorkingDialog::AskForAbort() 
{
    QMessageBox::StandardButton y = QMessageBox::question(
            this, tr("Abort"), tr("Are you sure ? valuable information might be lost !"));
    if (y == QMessageBox::No)
    {
        return false;
    }

    emit WasAborted();
    close();
    return true;
}
void
WorkingDialog::DoneWorking()
{
    reset();
}
void
WorkingDialog::reset()
{
    close();
}
OutputWidget*
WorkingDialog::output_widget() const
{
    return output_widget_;
}
void
WorkingDialog::Clear()
{
    output_widget_->clear();
}
void
WorkingDialog::SetDoneState()
{
    setCursor(Qt::ArrowCursor);
    button_box_->button(QDialogButtonBox::Abort)->setEnabled(false);

    setWindowTitle(tr("Done"));

    progress_bar_->setRange(0, 100);
    progress_bar_->setValue(100);
}
void
WorkingDialog::RaiseIt()
{
    show();
    raise();
}
void
WorkingDialog::UpdateProgressBar(int val)
{
    if (!active_)
    {
        PRINT_ERROR("Progress bar is passive cannot set progress status");
    }
    else
    {
        ERIS_CHECK(val >= 0 && val <= 100);
        progress_bar_->setValue(val);
        PRINT_INFO("Setting progress bar to %d ", val);
        if (val == 100)
        {
            // SetDoneState();
        }
    }
}
WorkingDialog::~WorkingDialog() = default;
}  // namespace widgets