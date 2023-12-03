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


#ifndef ERIS_COMMAND_H
#define ERIS_COMMAND_H

#include <QProcess>
namespace utils
{
//
// Small wrapper around |QProcess| that allows asynchronous execution.
//
class Command : public QProcess
{
    Q_OBJECT
public:
    explicit Command(QObject* parent = nullptr, const QString& programName = "");
    ~Command() override;

    /// Will block until the new process is Started.
    bool
    run(OpenMode mode = ReadWrite);
    bool
    runDetached(qint64* pid = nullptr);
    bool
    asyncRun(OpenMode mode = ReadWrite);

    virtual bool
    isRunning() final;
};

}  // namespace base

#endif  // ERIS_COMMAND_H