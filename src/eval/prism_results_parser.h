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

#ifndef ERIS_PRISM_RESULTS_PARSER_H
#define ERIS_PRISM_RESULTS_PARSER_H

#include "eris_config.h"

#include <QList>
#include <QMap>
#include <QObject>
#include <QPointF>
#include <QThread>
#include <QThreadPool>

#include <unordered_map>
#include <mutex>
#include <memory>
#include <atomic>

namespace eval
{

/**
 * Parser for PRISM generated output.
 * This class is meant to parse experiment generated results with the goal
 * of providing submodule failure/attack rates and/or displaying results
 * in via the plot.
 */
class ERIS_EXPORT PrismResultsParser : public QObject
{
    Q_OBJECT
public:
    enum class ParseError
    {
        FILE_NOT_FOUND,
        FILE_IS_INVALID,
        FILE_IS_NOT_READABLE,
    };
    class ERIS_EXPORT Observer
    {
    public:
        virtual ~Observer() {};
        // Emitted for each property.
        virtual void
        propertyResults(const QMap<QString, QList<QPointF>> results) {};
        
        virtual void
        failedToParse(ParseError error) {};
        // Emitted before the actual parsing starts.
        virtual void
        operationStarted() {};
        virtual void
        OperationDone(bool /*success*/)
        {
        }
    };

    static const char*
    TranslateParseError(ParseError error);
    static PrismResultsParser*
    Get();
    void
    RegisterObserver(Observer* observer);
    void
    UnRegisterObserver(Observer* observer);

    /**
     * Starts a thread to parse PRISM results. This function is only meant for
     * entire, non-stepwise executiom.
     * An absoulte path is required.
     * Makes use of helper function doParse().
     * @return true of the parser could be started successfully, false otherwise
     */
    bool
    parse(const QString& path);

    /**
     * Similar to the normal parse method, a thread for parsing the results is 
     * started. However, here a stepwise exection is handled, where only one
     * result at a time is parsed (needed for submodule evaluation).
     * An absoulte path is required.
     * Makes use of helper function doParse()
     * @return true of the parser could be started successfully, false otherwise
     */
    bool 
    parseStepResults(const QString& path, bool isLastStep);

    /**
     * Parses the results of a submodule PRISM evaluation and stores them in the
     * provided maps via ptr.
     * @return true if parsing seemed successful, false otherwise
     */
    bool
    parseForSubmodule(const QString& results_path,
                      std::map<qreal, QString>* safetyFailure,
                      std::map<qreal, QString>* securityFailure);

    ~PrismResultsParser() override;

private:
    explicit PrismResultsParser(QObject* parent = nullptr);

    bool
    doParse(bool publish);

    bool 
    doParseHelper();

    void
    publishResults();

    void
    FailedToParseReady(ParseError error);
    void
    OperationStartedReady();
    void
    OperationDoneReady(bool success);

    QThreadPool mThreadPool;

    /** Path to the PRISM-computed results */
    QString mPath;

    std::atomic_bool mDone;
    std::list<Observer*> mObservers;
    std::mutex mLock;
    QMap<QString, QList<QPointF>> mResults;

};

}  // namespace parser

#endif  // ERIS_PRISM_RESULTS_PARSER_H