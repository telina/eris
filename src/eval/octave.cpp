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

#include "octave.h"
#include "checks.h"
#include "memory.h"
#include "tokenizer.h"
#include "command.h"
#include "prism_results_parser.h"

#include <QDir>
#include <QFile>

#define CWD_PATH (QDir::currentPath() + utils::pathSeparator)
#define SIMULATION_RESULTS_PATH (CWD_PATH + kSimulationResultsFileName)

namespace {
const char kSimulationResultsFileName[] = ".__octave_results__.txt";
}

namespace eval
{
    
Octave::Octave() : mLock()
{

}

Octave* Octave::getInstance()
{
    static std::unique_ptr<Octave> instance(new Octave());
    
    return instance.get();
}

Octave::~Octave()
{
    QFile simulationResults(SIMULATION_RESULTS_PATH);
    if(simulationResults.exists())
    {
        PRINT_INFO("Destruct Octave");
        //ERIS_CHECK(simulationResults.remove());
    }
}

bool
Octave::extractSimulationFailureRates(const QString& simulationPath,
                                    ExperimentInterval interval,
                                    std::map<qreal, QString>* safetyFailure,
                                    std::map<qreal, QString>* securityFailure)
{
    std::lock_guard<std::mutex> guard(mLock);
    
    //QFile::remove(SIMULATION_RESULTS_PATH);
    auto octave = utils::allocateMemoryBlock<utils::Command>(nullptr, "octave");
    
    // ignoring the interval for now and write results to correct file
    octave->setArguments(QStringList() << "-q" << simulationPath);
    octave->setStandardOutputFile(SIMULATION_RESULTS_PATH, QIODevice::Truncate | QIODevice::WriteOnly);
    
    QStringList args = octave->arguments();

    PRINT_INFO("Arguments : %s ", args.join(' ').toStdString().c_str());
    
    octave->run();
    
    octave->waitForFinished(-1);
    
    if (octave->exitCode() != 0)
    {
        PRINT_ERROR("Octave exited with a status code  : %d ", octave->exitCode());
        auto output = octave->readAll().toStdString();
        PRINT_ERROR("%s", output.c_str());

        return false;
    }
    
    // with the presumption that the octave script uses the same output format as Prism
    return eval::PrismResultsParser::Get()->parseForSubmodule(
            SIMULATION_RESULTS_PATH, safetyFailure, securityFailure);
}
}