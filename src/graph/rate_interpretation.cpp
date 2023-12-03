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

#include "rate_interpretation.h"
namespace graph
{
   

RateInterpretation::RateInterpretation(Unit unit) : mSelectedUnit(unit)
{

}

RateInterpretation::RateInterpretation(QString unit)
{ 
    mSelectedUnit = mUnitMapper.at(unit); // Is this safe?
}


RateInterpretation::~RateInterpretation()
{
    
}

bool 
RateInterpretation::operator==(const RateInterpretation comp)
{
    if (mSelectedUnit == comp.mSelectedUnit)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool 
RateInterpretation::operator!=(const RateInterpretation comp)
{
    if (mSelectedUnit != comp.mSelectedUnit)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool 
RateInterpretation::operator==(const RateInterpretation::Unit compUnit)
{
    if (mSelectedUnit == compUnit)
    {
        return true;
    }
    else
    {
        return false;
    }
}

QString
RateInterpretation::toQString()
{
    QString repr = "";
    switch (mSelectedUnit)
    {
        case hourly:
            repr = "hourly";
            break;
        case monthly:
            repr = "monthly";
            break;
        case yearly:
            repr = "yearly";
            break;
    }
    return repr;
}

double 
RateInterpretation::RateInterpretationFactor(RateInterpretation current, RateInterpretation target)
{
    double factor = 1.0;
    if (current == hourly)
    {
        if (target == monthly)
        {
            factor = 730.0;
        }
        else if (target == yearly)
        {
            factor = 8766.0;
        }
    }
    else if (current == monthly)
    {
        if (target == hourly)
        {
            factor = 1.0/730.0;
        }
        else if (target == yearly)
        {
            factor = 12.0;
        }
    }
    else if (current == yearly)
    {
        if (target == hourly)
        {
            factor = 1.0/8766.0;
        }
        else if (target == monthly)
        {
            factor = 1.0/12.0;
        }
    }
    return factor;
}
    
    
}