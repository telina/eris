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

#ifndef ERIS_WIDGETS_WORKING_DIALOG_H
#define ERIS_WIDGETS_WORKING_DIALOG_H

#include "output_widget.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QProgressBar>

namespace widgets
{
//
// This dialog is useful for showing the current state
// of an operation that is happening in the background.
// It contains an |OutputWidget|, a |QProgressBar| and |QDialogButtonBox|
// to abort the operation when needed.
//
// Possible usage scenario:
//
// auto dialog = new WorkingDialog();
//
// dialog->ShowIt(); // GUI thread
// dialog->DoneWorking(); // background thread via queued qt signal.
//
class WorkingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WorkingDialog(QWidget* parent = nullptr);
    ~WorkingDialog() override;
    OutputWidget*
    output_widget() const;

    // Only clears the |OutputWidget|.
    void
    Clear();

    // Make the progress bar active or passive.
    void
    SetProgressBarActive(bool active)
    {
        active_ = active;
    };

signals:
    // Emitted if the user aborted the operation
    void
    WasAborted();

public slots:

    // Reset the |QProgressBar|, enable the abort button and
    // show the dialog.
    void
    ShowIt();

    // Make the dialog on the top of desktop widgets stack.
    void
    RaiseIt();

    // Show abort message as if the user clicked the abort button.
    bool
    AskForAbort();

    // Close the dialog.
    void
    DoneWorking();

    // Update the value of the progress bar.
    // Only valid if |SetProgressBarActive| was set to true. Otherwise, nothing happen.
    void
    UpdateProgressBar(int val);

    // Disable the abort button and set the progress bar to 100.
    void
    SetDoneState();

private:
    void
    reset();

private:
    QGridLayout* grid_layout_ = nullptr;
    QDialogButtonBox* button_box_ = nullptr;
    QProgressBar* progress_bar_ = nullptr;
    OutputWidget* output_widget_ = nullptr;
    bool active_ = false;
};

}  // namespace widgets

#endif  // ERIS_WIDGETS_WORKING_DIALOG_H
