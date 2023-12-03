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

#ifndef ERIS_EXPERIMENT_H
#define ERIS_EXPERIMENT_H
#include <string>

namespace eval
{
//
// Useful struct for dealing with
// the experiment interval (evaluation settings).
//
struct ExperimentInterval
{
    ExperimentInterval(int from_in, int to_in, int step_in) :
        from(from_in), to(to_in), steps(step_in)
    {
    }
    ExperimentInterval() = default;
    ~ExperimentInterval() = default;
    int from = -1;
    int to = -1;
    int steps = -1;

    std::string
    toString() const
    {
        return "T=" + std::to_string(from) + ":" + std::to_string(steps) + ":" + std::to_string(to);
    }
};
}  // namespace eval
#endif  // ERIS_EXPERIMENT_H