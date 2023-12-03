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

#ifndef ERIS_EVAL_OCTAVE_H
#define ERIS_EVAL_OCTAVE_H

#include "eris_config.h"
#include "experiment.h"

#include <QObject>

#include <memory>
#include <mutex>

namespace eval
{
    
/**
 * This class represents a wrapper for the tool octave.
 * 
 */
class ERIS_EXPORT Octave
{
    
public:
    
    static Octave* getInstance();
    
    ERIS_DISALLOW_COPY_AND_ASSIGN(Octave);
    
    ~Octave();
    
    /**
     * Runs the simulation of a module node and extracts the results. The obtained probabilities
     * are converted to raits and stored in the module node's rate map.
     * At the current state, it is expected that the provided simulation model is specified
     * to hold the information on to be performed steps/interval. 
     * In the future, this should be updated so that this command forwards the step/interval
     * information to the simulation model.
     * @param simulationPath path to simulation model
     * @param interval interval
     * @param safetyFailure module nodes map ptr to store defective rate
     * @param securityFailure module nodes map ptr to store corrupted rate
     * @return 
     */
    bool extractSimulationFailureRates(const QString& simulationPath,
                                    ExperimentInterval interval,
                                    std::map<qreal, QString>* safetyFailure,
                                    std::map<qreal, QString>* securityFailure);
    
private:
    Octave();
    
    std::mutex mLock;
};

}

#endif /* ERIS_EVAL_OCTAVE_H */

