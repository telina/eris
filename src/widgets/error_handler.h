/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
 *
 * Authors:
 *      - 2020-2021 Rhea Rinaldo (Rhea@Rinaldo.biz)
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

#ifndef ERIS_WIDGETS_ERROR_HANDLER_H
#define ERIS_WIDGETS_ERROR_HANDLER_H

#include "eris_config.h"
#include "errors.h"

#include <QMessageBox>

namespace widgets
{
class ERIS_EXPORT ErrorHandler : public QObject
{
    Q_OBJECT
public:
    ERIS_DISALLOW_COPY_AND_ASSIGN(ErrorHandler);

    static ErrorHandler&
    getInstance()
    {
        static ErrorHandler instance;
        return instance;
    }

    /**
     * Sets the error as the current error and appends the error collection.
     * @param error
     * @return
     */
    void
    setError(const std::pair<Errors::Type, QString>& error);

    /**
     * Returns the current error message. Needed when the error shall be displayed in a window,
     * other from the error message box.
     * @return
     */
    QString
    getLatestError();

    /**
     * Returns the collection of errors and clears it.
     * @return
     */
    QString
    getErrorCollection();

    /**
     * Clears the current error.
     */
    void
    clear();

    /**
     * Clears the error collection.
     */
    void
    clearCollection();

    /**
     * Displays the currently set error or warning in a QMessageBox.
     * @return
     */
    void
    show();

    /**
     * Displays the error collection in an error message box.
     */
    void
    showErrorCollection();

private:
signals:
    void
    showSignal();
    void
    showErrorCollectionSignal();

private slots:
    void
    onShow();
    void
    onShowErrorCollection();

private:
    ErrorHandler();

    std::pair<Errors::Type, QString> mCurrentError;

    QString mErrorCollection;
};

}  // namespace widgets

#endif /* ERIS_WIDGETS_ERROR_HANDLER_H */
