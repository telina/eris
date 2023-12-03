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

#ifndef ERIS_PRISM_XPRISM_H
#define ERIS_PRISM_XPRISM_H

#include "eris_config.h"
#include "command.h"

namespace eval
{
//
// This class represents the xprism command line tool as a |QProcess|.
//
class ERIS_EXPORT XPrism : public utils::Command
{
public:
    static XPrism*
    getInstance();
    ERIS_DISALLOW_COPY_AND_ASSIGN(XPrism);
    ~XPrism() override;

private:
    XPrism();
};

}  // namespace commands

#endif  // ERIS_PRISM_XPRISM_H
