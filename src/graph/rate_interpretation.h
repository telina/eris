/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
 *
 * Authors:
 *      - 2023 Rhea Rinaldo (Rhea@Rinaldo.biz)
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

#ifndef ERIS_GRAPH_RATE_INTERPRETATION_H
#define ERIS_GRAPH_RATE_INTERPRETATION_H

#include <QString>
#include <map>
/** This file holds the enum representing the selected rate interpretation (works only with CTMC 
 * mode).  
 */
namespace graph
{
class RateInterpretation
{
public:
    enum Unit
    {
        hourly,
        monthly,
        yearly 
    };
   
    RateInterpretation(Unit RateInterpretation);
    
    RateInterpretation(QString RateInterpretation);
    
    ~RateInterpretation();
    
    bool operator==(const RateInterpretation);
    
    bool operator!=(const RateInterpretation);
    
    bool operator==(const RateInterpretation::Unit);
    
    static double 
    RateInterpretationFactor(RateInterpretation current, RateInterpretation target);
    
    /**
     * Returns the string representation of this Rate Interpretations currently selected unit.
     * @return 
     */
    QString toQString();
   
private:
    
    Unit mSelectedUnit;
    
    static const inline std::map<QString, RateInterpretation::Unit> mUnitMapper =
    {
        {"hourly", RateInterpretation::hourly},
        {"monthly", RateInterpretation::monthly},
        {"yearly", RateInterpretation::yearly}
    };

};

}


#endif /* ERIS_GRAPH_RATE_INTERPRETATION_H */

