/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
 *      - 2020-2021 Rhea Rinaldo (Rhea@Odlanir.de)
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

#include "error_handler.h"

namespace widgets
{
void
ErrorHandler::setError(const std::pair<Errors::Type, QString>& error)
{
    clear();
    mCurrentError = error;
    mErrorCollection += error.second;
}

QString
ErrorHandler::getLatestError()
{
    return mCurrentError.second;
}

QString
ErrorHandler::getErrorCollection()
{
    QString ret = mErrorCollection;
    mErrorCollection.clear();
    return ret;
}

void
ErrorHandler::clear()
{
    mCurrentError.first = Errors::Type::ERROR;  // default
    mCurrentError.second.clear();
}

void
ErrorHandler::clearCollection()
{
    mErrorCollection.clear();
}

void
ErrorHandler::show()
{
    emit showSignal();
}

void
ErrorHandler::showErrorCollection()
{
    emit showErrorCollectionSignal();
}

void
ErrorHandler::onShow()
{
    if (!mCurrentError.second.isEmpty())
    {
        if (mCurrentError.first == Errors::Type::WARNING)
        {
            QMessageBox::warning(nullptr, "Warning", mCurrentError.second, nullptr, nullptr);
        }
        else
        {
            QMessageBox::critical(nullptr, "Error", mCurrentError.second, nullptr, nullptr);
        }

        clear();            // clear the error string
        clearCollection();  // and the error collection
    }
}

void
ErrorHandler::onShowErrorCollection()
{
    if (!mErrorCollection.isEmpty())
    {
        QMessageBox::critical(nullptr, "ErrorCollection", mErrorCollection, nullptr, nullptr);
        clear();
        clearCollection();
    }
}

ErrorHandler::ErrorHandler() : QObject()
{
    connect(this, &ErrorHandler::showSignal, this, &ErrorHandler::onShow, Qt::QueuedConnection);
    connect(this,
            &ErrorHandler::showErrorCollectionSignal,
            this,
            &ErrorHandler::onShowErrorCollection,
            Qt::QueuedConnection);
}

}  // namespace widgets
