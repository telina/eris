/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
 *
 * Authors:
 *      - 2021-2021 Mohammed Hussam Al Turjman (Mohammed_Hussam.Al_Turjman@dfki.de)
 *      - 2023  Rhea Rinaldo (Rhea@Rinaldo.biz)
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

#include "prism.h"

#include "checks.h"
#include "logger.h"
#include "memory.h"
#include "tokenizer.h"
#include "working_dialog.h"
#include "command.h"
#include "experiment.h"
#include "prism_results_parser.h"
#include "evaluation_tab.h"
#include "chart_view.h"

#include <QDir>
#include <sstream>
#include <QFile>
#include <QWidget>
#include <QProcess>
#include <QThreadPool>
#include <QDateTime>

#define CWD_PATH (QDir::currentPath() + utils::pathSeparator)
#define EXPERIMENT_PATH (CWD_PATH + kExperimentFileName)
#define EXPERIMENT_RESULTS_PATH (CWD_PATH + kExperimentResultsFileName)
#define SUBMODULE_PROPERTIES_PATH (CWD_PATH + kSubmodulePropertiesFileName)
#define SUBMODULE_PROPERTIES_RESULTS_PATH (CWD_PATH + kSubmodulePropertiesResultsFileName)

namespace 
{
// Those files are created in the working directory where eris was first
// executed, and they will be removed on a graceful exit.
const char kExperimentFileName[] = ".__eris_experiment__.pctl";
const char kExperimentResultsFileName[] = ".__eris_results__.txt";
const char kSubmodulePropertiesFileName[] = ".__eris_submodule_properties__.txt";
const char kSubmodulePropertiesResultsFileName[] = ".__eris_submodule_properties_results__.txt";

const char*
translateQProcessError(QProcess::ProcessError state)
{
    switch (state)
    {
        case QProcess::FailedToStart: 
            return "FailedToStart";
        case QProcess::Crashed: 
            return "Crashed";
        case QProcess::Timedout: 
            return "Timeout";
        case QProcess::ReadError: 
            return "ReadError";
        case QProcess::WriteError: 
            return "WriteError";
        case QProcess::UnknownError: 
            return "UnknownError";
        default: 
            return "WTF";
    }
}
}  // namespace

namespace eval
{
    
using utils::Command;
using eval::PrismResultsParser;
using widgets::ChartView;
using widgets::EvaluationTab;
using widgets::OutputWidget;
using widgets::WorkingDialog;

Prism::Prism(QObject* parent, QWidget* parentWidget) :
    QObject(parent),
    mStdoutBuffer(),
    mStderrBuffer(),
    mArgs(),
    mThreadPool(),
    mDone(true),
    mLock(),
    mCommand(nullptr),
    mWorkingDialog(nullptr),
    mDoEvaluate(false),
    mLastStep(false),
    mStepwiseExecution(false),
    mExperimentInterval(new eval::ExperimentInterval()),
    mStartTime(""),
    mEndTime("")
{
    mCommand = utils::allocateMemoryBlock<Command>(nullptr, "prism");
    mWorkingDialog = utils::allocateMemoryBlock<WorkingDialog>(parentWidget);
    connect(mCommand.get(), &Command::errorOccurred, this, &Prism::errorOccurred);
    // connect(command.get(),&Command::finished,this,&Prism::finished);
    connect(mCommand.get(),
            SIGNAL(finished(int, QProcess::ExitStatus)),
            this,
            SLOT(finished(int, QProcess::ExitStatus)));

    connect(mCommand.get(), &Command::stateChanged, this, &Prism::stateChanged);
    connect(mCommand.get(), &Command::started, this, &Prism::started);
    connect(mCommand.get(), &Command::readyReadStandardError, this, &Prism::readyReadStandardError);

    connect(mCommand.get(),
            &Command::readyReadStandardOutput,
            this,
            &Prism::readyReadStandardOutput);

    connect(this,
            &Prism::WriteOutput,
            mWorkingDialog->output_widget(),
            &OutputWidget::writeOutput,
            Qt::QueuedConnection);

    connect(mWorkingDialog.get(), &WorkingDialog::WasAborted, this, &Prism::terminate);

    eval::PrismResultsParser::Get()->RegisterObserver(this);
}

Prism*
Prism::getInstance()
{
    static std::unique_ptr<Prism> instance(new Prism(nullptr, nullptr));
    return instance.get();
}

Prism::~Prism()
{
    QFile experimentFile(EXPERIMENT_PATH);
    QFile experimentResults(EXPERIMENT_RESULTS_PATH);
    if (experimentFile.exists())
    {
        ERIS_CHECK(experimentFile.remove());
    }

    if (experimentResults.exists())
    {
        ERIS_CHECK(experimentResults.remove());
    }
}

void
Prism::reset()
{
    mArgs.clear();
    mWorkingDialog->Clear();
}

bool
Prism::execute(bool with_evaluation, bool stepwise, bool lastStep,
        const QString& working_directory)
{
    ERIS_CHECK(!mArgs.isEmpty());
    mStepwiseExecution = stepwise;
    mLastStep = lastStep;

    if (mCommand->state() == QProcess::Running)
    {
        PRINT_WARNING("Previous call of prism did not finish yet");
        return false;
    }
    mDoEvaluate = with_evaluation; // TODO check if this is needed?
    mCommand->setWorkingDirectory(working_directory);

    // This is needed to clear the plot
    // when executing prism without an experiment.
    if (!mStepwiseExecution)
    {
        EvaluationTab::Get()->view()->clear();
    }

    if (mDoEvaluate)
    {
        addArgument(EXPERIMENT_PATH);
        addArgument("-const");
        addArgument(tr(mExperimentInterval->toString().c_str()));
        addArgument("-javamaxmem");
        addArgument("4g");
        addArgument("-cuddmaxmem");
        addArgument("4g");
        addArgument("-exportresults");
        addArgument(EXPERIMENT_RESULTS_PATH);
        writeExperimentFile();
    }
    
    mCommand->setArguments(mArgs);

    mStdoutBuffer.str("");
    mStdoutBuffer.clear();

    mStderrBuffer.str("");
    mStderrBuffer.clear();
    if (mStartTime.isEmpty())
    {
        mStartTime = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");    
    }
    emit WriteOutput("Operation Started \n--------------------- \n", Qt::green);
    
    return mCommand->run();
}

void
Prism::addArgument(const QString& arg)
{
    mArgs << arg;
}

void
Prism::errorOccurred(QProcess::ProcessError error)
{
    // TODO Handle errors
    PRINT_ERROR("An error occurred while executing prism command line tool : %s ",
                translateQProcessError(error));
}

void
Prism::finished(int exitCode, QProcess::ExitStatus /*exitStatus*/ exitStatus)
{
    PRINT_INFO("Prism Finished with the exit code %d ", exitCode);

    if ((!mStepwiseExecution) || mLastStep)
    {
        mWorkingDialog->SetDoneState();
    }

    if (exitCode == 0)
    { // success, probably

        if (mStepwiseExecution)
        {// submodule stepwise mode
            PrismResultsParser::Get()->parseStepResults(EXPERIMENT_RESULTS_PATH, mLastStep);
            if (!mLastStep)
            {
                return; // avoid emitting work done, before it is actually done   
            }
        }
        else
        {// Normal evaluation mode
            PrismResultsParser::Get()->parse(EXPERIMENT_RESULTS_PATH);
        }

    }
    else
    {
        emit WriteOutput(mStdoutBuffer.str().c_str(), Qt::GlobalColor::red);
        emit WriteOutput(mStderrBuffer.str().c_str(), Qt::GlobalColor::red);
        std::stringstream ss;
        ss << "Executing prism was not successful. Exit code : " << exitCode;
        emit WriteOutput(ss.str().c_str(), Qt::red);
        PRINT_WARNING("Prism Finished with the exit code %d ", exitCode);
        DPRINT_INFO("STDOUT FROM PRISM : \n%s", mStdoutBuffer.str().c_str());
        DPRINT_INFO("STDERR FROM PRISM : \n%s", mStderrBuffer.str().c_str());
    }

    emit DoneWorking(true);
    mWorkingDialog->RaiseIt();
}

void
Prism::started()
{
    mWorkingDialog->ShowIt();
}

void
Prism::readyReadStandardError()
{
    mCommand->setReadChannel(QProcess::ProcessChannel::StandardError);
    std::string buffer = mCommand->read(1024).toStdString();
    mStderrBuffer << buffer;
}

void
Prism::readyReadStandardOutput()
{
    mCommand->setReadChannel(QProcess::ProcessChannel::StandardOutput);
    std::string buffer = mCommand->read(1024).toStdString();
    mStdoutBuffer << buffer;
}

void
Prism::stateChanged(QProcess::ProcessState newState)
{
    switch (newState)
    {
        case QProcess::ProcessState::Starting: PRINT_INFO("Prism is starting"); break;
        case QProcess::ProcessState::NotRunning:
            PRINT_INFO("Prism still not / no longer running");
            break;
        case QProcess::ProcessState::Running: PRINT_INFO("Prism is now running"); break;
        default: break;
    }
}

void
Prism::setParentWidget(QWidget* parentWidget)
{
    mWorkingDialog->setParent(parentWidget);
}

void
Prism::terminate()
{
    mCommand->terminate();
    mWorkingDialog->DoneWorking();
}

bool
Prism::isRunning() const
{
    const bool running = mCommand->state() == QProcess::Running;

    if (running)
        mWorkingDialog->show();

    return running;
}

void
Prism::writeExperimentFile()
{
    QFile experimentFile(EXPERIMENT_PATH);
    ERIS_CHECK(experimentFile.open(QFile::WriteOnly | QFile::Text | QFile::Truncate));
    experimentFile.write(experimentDoc.toStdString().c_str());
    experimentFile.close();
}

bool
Prism::askForAbort()
{
    const bool ret = mWorkingDialog->AskForAbort();
    if (ret)
    {
        mCommand->waitForFinished(10000);
    }
    return ret;
}

void
Prism::failedToParse(eval::PrismResultsParser::ParseError error)
{
    PRINT_ERROR("Failed to parse");
    QString message =
            "Parse error : " + tr(eval::PrismResultsParser::TranslateParseError(error)) + "\n\n";

    emit WriteOutput("Failed to parse the results from prism\n---------------------\n", Qt::red);

    emit WriteOutput(message, Qt::red);
    QFile f(EXPERIMENT_RESULTS_PATH);
    if (f.open(QFile::ReadOnly))
    {
        emit WriteOutput(f.readAll().toStdString().c_str(), Qt::red);
        f.close();
    }
    emit WriteOutput("", Qt::red);
}

void
Prism::OperationDone(bool success)
{
    if (success)
    {
        mEndTime = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");    
        emit WriteOutput("Operation ended successfully \n", Qt::green);
        emit WriteOutput("Start was: " +mStartTime + "\n", Qt::green);
        emit WriteOutput("End was:  " +mEndTime + "\n", Qt::green);
        emit WriteOutput("---------------------\n", Qt::green);
        emit WriteOutput("", Qt::green);
    }
    mStartTime = ""; // reset start time so it will be set on the next execute call
}

bool
Prism::extractSubmoduleFailureRates(const QString& prismModel,
                                    ExperimentInterval interval,
                                    std::map<qreal, QString>* safetyFailure,
                                    std::map<qreal, QString>* securityFailure)
{
    std::lock_guard<std::mutex> guard(mLock);

    QFile::remove(SUBMODULE_PROPERTIES_RESULTS_PATH);
    static QString propertiesPctl = "const double T;\n"
                                             "\n"
                                             "P=? [ F[T,T] \"corrupted\" ] // security failure F=T\n"
                                             "\n"
                                             "P=? [ F[T,T] \"defective\" ] // safety failure F=T";

    // TODO remove this file on exit
    QFile tmp(SUBMODULE_PROPERTIES_PATH);
    ERIS_CHECK(tmp.open(QFile::WriteOnly | QFile::Text | QFile::Truncate));
    const qint64 written_bytes = tmp.write(propertiesPctl.toStdString().c_str());
    ERIS_CHECK(written_bytes == propertiesPctl.size());
    tmp.close();

    auto localPrism = utils::allocateMemoryBlock<Command>(nullptr, "prism");

    localPrism->setArguments(QStringList() << prismModel << SUBMODULE_PROPERTIES_PATH << "-const"
                                            << tr(interval.toString().c_str()) << "-exportresults"
                                            << SUBMODULE_PROPERTIES_RESULTS_PATH);
    QStringList args = localPrism->arguments();

    PRINT_INFO("Arguments : %s ",
                    args.join(' ').toStdString().c_str());
    if (!localPrism->run())
    {
        QStringList args = localPrism->arguments();
        PRINT_ERROR("Failed to start prism with the arguments : %s ",
                    args.join(' ').toStdString().c_str());
        return false;
    }

    localPrism->waitForFinished(-1);

    if (localPrism->exitCode() != 0)
    {
        PRINT_ERROR("Prism exited with a status code  : %d ", localPrism->exitCode());
        auto output = localPrism->readAll().toStdString();
        PRINT_ERROR("%s", output.c_str());

        return false;
    }

    return eval::PrismResultsParser::Get()->parseForSubmodule(
            SUBMODULE_PROPERTIES_RESULTS_PATH, safetyFailure, securityFailure);
}

}  // namespace commands