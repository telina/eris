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


#include "command.h"

#include "logger.h"
#include "task.h"

namespace utils
{
    
Command::~Command() = default;

Command::Command(QObject* parent, const QString& programName) : QProcess(parent)
{
    setProgram(programName);
}
bool
Command::runDetached(qint64* pid)
{
    startDetached(pid);
    waitForStarted();
    return true;
}
bool
Command::run(QIODevice::OpenMode mode)
{
    QProcess::start(mode);
    waitForStarted();

    QProcess::ProcessState _state = state();
    QString args = program() + " ";
    for (auto& arg : arguments())
    {
        args += arg + " ";
    }

    switch (_state)
    {
        case QProcess::ProcessState::NotRunning: PRINT_WARNING("Not running "); break;
        case QProcess::ProcessState::Running:
            PRINT_INFO("Running : %s ", args.toStdString().c_str());
            break;
        case QProcess::ProcessState::Starting: PRINT_INFO("Starting"); break;
        default: break;
    }
    return _state == QProcess::ProcessState::Running;
}
bool
Command::asyncRun(QIODevice::OpenMode mode)
{
    // TODO Use QRunnable instead
    return asyncTask(&Command::run, this, mode);
}
bool
Command::isRunning()
{
    return state() == QProcess::Running;
}

}  // namespace base