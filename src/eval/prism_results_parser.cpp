/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
 *
 * Authors:
 *      - 2021-2021 Mohammed Hussam Al Turjman (Mohammed_Hussam.Al_Turjman@dfki.de)
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

#include "prism_results_parser.h"
#include "checks.h"

#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <math.h>

namespace eval
{
    
static double
convertToRate(double probability, double t)
{
    if (probability == 0.0)
    {
        return 0.0;
    }
    else if (probability < 0)
    {
        PRINT_WARNING("Cannot convert to rate with negative probability! (%lf)", probability);
        return 0.0;
    }
    else if (t < 0)
    {
        PRINT_ERROR("Cannot convert to rate with negative time point! (%lf)", t);
        return 0.0;
    }
    double res = -(log(1.0-probability)/t);
    PRINT_INFO(" Module Rate %lf", res);
    return res;
}

// static
const char*
PrismResultsParser::TranslateParseError(ParseError error)
{
    switch (error)
    {
        case ParseError::FILE_IS_INVALID: return "File is invalid";
        case ParseError::FILE_NOT_FOUND: return "File not found";
        case ParseError::FILE_IS_NOT_READABLE: return "File is not readable";
        default: break;
    }

    ERIS_CHECK(0);

    return "WTF";
}

PrismResultsParser::PrismResultsParser(QObject* parent) :
    QObject(parent), mThreadPool(nullptr), mDone(true), mObservers(), mLock()
{
}

PrismResultsParser::~PrismResultsParser()
{

}

bool
PrismResultsParser::parse(const QString& path)
{
    if (!mDone.load(std::memory_order_acquire))
    {
        PRINT_ERROR("PrismResultParser is already running ");
        return false;
    }
    mDone.store(false, std::memory_order_seq_cst);

    // Wait until previous thread exists
    mThreadPool.waitForDone();
    mPath = path;

    mThreadPool.start([=] {
        OperationStartedReady();
        OperationDoneReady(doParse(true));
        mDone.store(true, std::memory_order_seq_cst);
    });
    return true;
}

bool
PrismResultsParser::parseStepResults(const QString& path, bool isLastStep)
{
    if (!mDone.load(std::memory_order_acquire))
    {
        PRINT_ERROR("PrismResultParser is already running ");
        return false;
    }
    mDone.store(false, std::memory_order_seq_cst);

    // Wait until previous thread exists
    mThreadPool.waitForDone();
    mPath = path;
    bool isFirstStep = mResults.empty();

    mThreadPool.start([=] {
            if (isFirstStep)
            {
                OperationStartedReady();
            }
            if (isLastStep)
            {
                OperationDoneReady(doParse(true));
            }
            else
            {
                doParse(false);
            }
            //doParse(true);
            mDone.store(true, std::memory_order_seq_cst);
        });
    return true;
}   

bool
PrismResultsParser::doParseHelper()
{
    ERIS_CHECK(!mPath.isEmpty());
    if (!QFileInfo::exists(mPath))
    {
        PRINT_ERROR("File not found : %s ", mPath.toStdString().c_str());

        FailedToParseReady(ParseError::FILE_NOT_FOUND);
        return false;
    }
    QFile file(mPath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        PRINT_ERROR("File is not readable : %s ", mPath.toStdString().c_str());

        FailedToParseReady(ParseError::FILE_IS_NOT_READABLE);
        return false;
    }
    QString content = file.readAll();
    QStringList lines = content.split("\n");
    QString lastProperty = "systemfailure";

    for (const auto& line : lines)
    {
        if (line.isEmpty())
            continue;

        QString token = line.simplified();

        if (token == "T Result")
            continue;

        auto reg = QRegularExpression(R"(".*?")");
        auto match = reg.match(token);
        if (match.hasMatch())
        {
            QString property = match.captured(0).remove("\"");
            lastProperty = property;
            continue;
        }
        QStringList xy = token.split(" ");

        if (xy.length() != 2)
        {
            PRINT_ERROR("File is invalid : %s ", mPath.toStdString().c_str());

            FailedToParseReady(ParseError::FILE_IS_INVALID);
            return false;
        }

        bool ok = false;
        double x = xy[0].toDouble(&ok);
        if (!ok)
        {
            PRINT_ERROR("File is invalid : %s ", mPath.toStdString().c_str());
            FailedToParseReady(ParseError::FILE_IS_INVALID);
            return false;
        }
        double y = xy[1].toDouble(&ok);
        if (!ok)
        {
            PRINT_ERROR("File is invalid : %s ", mPath.toStdString().c_str());
            FailedToParseReady(ParseError::FILE_IS_INVALID);
            return false;
        }

        bool contains = false;
        for (QPointF point : mResults[lastProperty])
        { // Required for stepwise evaluation
            if (point.x() == x)
            {
                contains = true;
            }
        }

        if (!contains)
        {
            mResults[lastProperty].append(QPointF(x, y));
        }

    }
    file.close();

    return true;
}

void
PrismResultsParser::publishResults()
{
    std::lock_guard<std::mutex> guard(mLock);
    auto iter = mObservers.begin();
    while (iter != mObservers.end())
    {
        (*iter)->propertyResults(mResults);
        iter++;
    }
    mResults.clear();
}

bool
PrismResultsParser::doParse(bool publish)
{
    if (!doParseHelper()) // parses results file and fills mResults
    {
        return false;
    }
    if (publish)
    {
        publishResults(); // publish the found results
    }

    return true;
}

PrismResultsParser*
PrismResultsParser::Get()
{
    static std::unique_ptr<PrismResultsParser> instance(new (std::nothrow) PrismResultsParser());
    return instance.get();
}
void
PrismResultsParser::RegisterObserver(PrismResultsParser::Observer* observer)
{
    std::lock_guard<std::mutex> guard(mLock);
    ERIS_CHECK(observer);
    mObservers.push_back(observer);
}
void
PrismResultsParser::UnRegisterObserver(PrismResultsParser::Observer* observer)
{
    std::lock_guard<std::mutex> guard(mLock);
    ERIS_CHECK(observer);
    mObservers.remove(observer);
}

void
PrismResultsParser::FailedToParseReady(PrismResultsParser::ParseError error)
{
    std::lock_guard<std::mutex> guard(mLock);
    auto iter = mObservers.begin();
    while (iter != mObservers.end())
    {
        (*iter)->failedToParse(error);
        iter++;
    }
}
void
PrismResultsParser::OperationStartedReady()
{
    PRINT_INFO("OperationStartedReady");
    std::lock_guard<std::mutex> guard(mLock);
    auto iter = mObservers.begin();
    while (iter != mObservers.end())
    {
        (*iter)->operationStarted();
        iter++;
    }
}
void
PrismResultsParser::OperationDoneReady(bool success)
{
    PRINT_INFO("OperationDoneReady");
    std::lock_guard<std::mutex> guard(mLock);
    auto iter = mObservers.begin();
    while (iter != mObservers.end())
    {
        (*iter)->OperationDone(success);
        iter++;
    }
}
bool
PrismResultsParser::parseForSubmodule(const QString& results_path,
                                      std::map<qreal, QString>* safetyFailure,
                                      std::map<qreal, QString>* securityFailure)
{
    ERIS_CHECK(!results_path.isEmpty());

    if (!QFileInfo::exists(results_path))
    {
        PRINT_ERROR("File not found : %s ", results_path.toStdString().c_str());
        return false;
    }

    QFile file(results_path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        PRINT_ERROR("File is not readable : %s ", results_path.toStdString().c_str());
        return false;
    }

    QString content = file.readAll();
    QStringList lines = content.split("\n");
    std::unordered_map<QString, QList<QPointF>> results;
    QString lastProperty = "__Nothing__";

    for (const auto& line : lines)
    {
         if (line.isEmpty())
            continue;

        QString token = line.simplified();

        if (token == "T Result")
            continue;

        auto reg = QRegularExpression(R"(".*?")");
        auto match = reg.match(token);
        if (match.hasMatch())
        {
            QString property = match.captured(0).remove("\"");
            lastProperty = property;
            continue;
        }
        QStringList xy = token.split(" ");

        if (xy.length() != 2)
        {
            PRINT_ERROR("File is invalid : %s ", results_path.toStdString().c_str());
            return false;
        }

        bool ok = false;
        double x = xy[0].toDouble(&ok);
        if (!ok)
        {
            PRINT_ERROR("File is invalid : %s ", results_path.toStdString().c_str());
            return false;
        }

        double y = xy[1].toDouble(&ok);
        if (!ok)
        {
            PRINT_ERROR("File is invalid : %s ", results_path.toStdString().c_str());
            return false;
        }

        results[lastProperty].append(QPointF(x, y));
    }
    file.close();

    auto iter = results.begin();

    while (iter != results.end())
    { 
        if (iter->first == "defective")
        {
            for (const auto& point : iter->second)
            {
                (*safetyFailure)[point.x()] = QString::number(convertToRate(point.y(), point.x()));
            }
        }
        else if (iter->first == "corrupted")
        {
            for (const auto& point : iter->second)
            {
                (*securityFailure)[point.x()] = QString::number(convertToRate(point.y(), point.x()));
            }
        }
        else
        {
            PRINT_ERROR("Unexpected property %s ", iter->first.toStdString().c_str());
           // return false;
        }

        iter++;
    }

    return true;
}

}  // namespace parser