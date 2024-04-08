/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
 *      - 2021-2021 Mohammed Hussam Al Turjman (Mohammed_Hussam.Al_Turjman@dfki.de)
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

#ifndef ERIS_MAIN_WINDOW_TOOL_BUTTONS_MANAGER_H
#define ERIS_MAIN_WINDOW_TOOL_BUTTONS_MANAGER_H

#include "eris_config.h"

#include <QObject>
#include <list>
#include <vector>

QT_BEGIN_NAMESPACE
class QToolButton;
QT_END_NAMESPACE

namespace widgets
{
class ERIS_EXPORT MainWindowToolButtonsManager : public QObject
{
    Q_OBJECT
public:

    static MainWindowToolButtonsManager* getInstance();
    
    ERIS_DISALLOW_COPY_AND_ASSIGN(MainWindowToolButtonsManager);
    
    MainWindowToolButtonsManager() = delete;
    
    ~MainWindowToolButtonsManager() override;
    
    void enableAll(bool enable = true);
    
    void enableNodeSwapIdButton(bool enable=true);
    
    void disableVerificationButtons();

private:
    
    explicit MainWindowToolButtonsManager(QObject* parent);

    void 
    initToolButton(QToolButton* button, QString iconpath, QString tooltip, const char* slot);

private slots:
    
    void startVerificationButClicked();
    
    void transformButClicked();
    
    void experimentButClicked();
    
    void itemSettingsButClicked();
    
    void swapCriticalityButClicked();
    
    void swapIdButClicked();
    
    
private:

    /** Vector to hold all toolbuttons for convenience functions (enabling etc) */
    std::vector<QToolButton*> mToolButtons;
    
    /** Available Tool Buttons */
    QToolButton* mStartVerificationBut;
    QToolButton* mTransformBut;
    QToolButton* mExperimentBut;
    QToolButton* mItemSettingsBut;
    QToolButton* mNodeCriticalityBut;
    QToolButton* mNodeSwapIdBut;

};
}  // namespace widgets

#endif  // ERIS_MAIN_WINDOW_TOOL_BUTTONS_MANAGER_H
