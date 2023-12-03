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


#ifndef ERIS_PRISM_PRISM_H
#define ERIS_PRISM_PRISM_H

#include "eris_config.h"
#include "prism_results_parser.h"
#include "experiment.h"

#include <QObject>
#include <QProcess>
#include <QThreadPool>

#include <memory>
#include <sstream>
#include <functional>
#include <atomic>
#include <mutex>

namespace utils
{
class Command;
}
namespace widgets
{
class WorkingDialog;
}

namespace eval
{
class PrismResultsParser;


/**
 * This class represents a wrapper for the prism command line tool.
 * 
 * Usage scenario:
 * Prism prism;
 * prism.Reset(); must be rested after each call of execute()
 * prism << "arguments that prism can consume";
 * prism.Execute("working directory");
 * Once the execution is complete the signal |doneWorking| will be emitted.
 * This command is coupled with the workingDialog meaning it controls and can be controlled by it.
 * A user might for example click on abort to terminate the execution
 * of the prism tool or when the execution is finished the dialog will be raised
 * so the user can be notified that prism is done.
 * In addition, it controls the |EvaluationTab| to update the plot with the evaluation results.
 * 
 */
class ERIS_EXPORT Prism : public QObject, public PrismResultsParser::Observer
{
    Q_OBJECT

public:
    ERIS_DISALLOW_COPY_AND_ASSIGN(Prism);
    static Prism*
    getInstance();

    Prism() = delete;
    ~Prism() override;

    /**
     * Clears arguments and resets working dialogue.
     */
    void
    reset();

    /**
     * Appends given argument to argument string used in prism command.
     */
    void
    addArgument(const QString& arg);

    /**
     * Starts the submodule evaluation process and parses the results.
     * Note that his method *always* checks the properties safetyfailure and
     * corrupted, regardless of the user input for the experiment!
     * The obtained results for either property are stored in the provided
     * maps.
     * @return true if seemingly successful, false otherwise
     */
    bool
    extractSubmoduleFailureRates(const QString& prismModel,
                                    ExperimentInterval interval,
                                    std::map<qreal, QString>* securityFailureRates,
                                    std::map<qreal, QString>* safetyFailureRates);
    /**
     * Runs the prism command line tool in background in terms of an
     * experiment.
     * @param with_evaluation will lead to displaying the results in the evaluation
     * tab
     * @param stepwise indicates a single step execution
     * @param lastStep indicates its the last step of the stepwise execution. This is
     * needed to publish evaluation results at the end
     */
    bool
    execute(bool with_evaluation, bool stepwise=false, bool lastStep=false,
        const QString& working_directory = "");

    /**
     * Sets the parent widget of the working dialog.
     */
    void
    setParentWidget(QWidget* parent_widget);

    /**
     * Checks whether the background prism process is running and shows
     * working dialogue if so.
     * @return true if running, false otherwise
     */
    bool
    isRunning() const;

    // Ask the user if the background process should be terminated.
    // Normally you wouldn't need to call this method by your self
    // since it will be called when the user clicks the |abort| button.
    // Nevertheless, when the program is about to close and the background
    // process is still running, this method can be called before |closeEvent| in
    // the |MainWindow| returns.
    bool
    askForAbort();

public slots:

    /** 
     * Terminats the command.
     */
    void
    terminate();

protected:
    void
    failedToParse(PrismResultsParser::ParseError error) override;
    void
    OperationDone(bool success) override;

public:
signals:
    void
    DoneWorking(bool);


private slots:
    void
    errorOccurred(QProcess::ProcessError error);
    void
    finished(int exitCode, QProcess::ExitStatus exitStatus);
    void
    started();
    void
    readyReadStandardError();
    void
    readyReadStandardOutput();
    void
    stateChanged(QProcess::ProcessState newState);

private:
signals:
    void
    WriteOutput(const QString& output, Qt::GlobalColor color);

private:
    void
    writeExperimentFile();
    explicit Prism(QObject* parent, QWidget* parentWidget);

    std::stringstream mStdoutBuffer;
    std::stringstream mStderrBuffer;

    QStringList mArgs;
    QThreadPool mThreadPool;
    std::atomic_bool mDone;
    std::mutex mLock;
    std::unique_ptr<utils::Command> mCommand;
    std::unique_ptr<widgets::WorkingDialog> mWorkingDialog;
    
    QString mStartTime;
    QString mEndTime;

    bool mDoEvaluate;

    /** Flag to indicate that a stepwise execution (with submodule) is performed */
    bool mStepwiseExecution;

    /** Ugly way to indicate that this step is the last step (only required if stepwise is true) */
    bool mLastStep;

public:
    QString experimentDoc;
    std::unique_ptr<eval::ExperimentInterval> mExperimentInterval;
};

}  // namespace eval

#endif  // ERIS_PRISM_PRISM_H