/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
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

#ifndef ERIS_MAIN_WINDOW_BUTTONS_GROUP_MANAGER_H
#define ERIS_MAIN_WINDOW_BUTTONS_GROUP_MANAGER_H

#include "eris_config.h"
#include <QObject>
#include <list>
#include <vector>

QT_BEGIN_NAMESPACE
class QButtonGroup;
class QToolBox;
QT_END_NAMESPACE

namespace widgets
{
    
class ERIS_EXPORT MainWindowButtonsGroupManager : public QObject
{
    Q_OBJECT
public:
    class ERIS_EXPORT Observer
    {
    public:
        
        virtual ~Observer()
        {
            
        }
        
        virtual void
        ButtonsGroupButtonClicked(int /*id*/)
        {
            
        }
        
        virtual void
        AnnotationGroupButtonClicked(int /*id*/)
        {
            
        }
    };
    
    static MainWindowButtonsGroupManager*
    getInstance();
    
    MainWindowButtonsGroupManager() = delete;
    ERIS_DISALLOW_COPY_AND_ASSIGN(MainWindowButtonsGroupManager);
    
    void
    registerObserver(Observer* observer);
    
    void
    unRegisterObserver(Observer* observer);
    
    void
    enableAll(bool enable = true);

private:
    explicit MainWindowButtonsGroupManager(QObject* parent);

    void
    slotButtonsGroupButtonClicked(int id);
    
    void
    slotAnnotationGroupButtonClicked(int id);

    std::list<Observer*> observers_;
    std::vector<QButtonGroup*> button_groups_;

public:
    QToolBox* mToolBox;
    QButtonGroup* mButtonGroup;
    QButtonGroup* mAnnotationGroup;
};

}  // namespace widgets

#endif  // ERIS_MAIN_WINDOW_BUTTONS_GROUP_MANAGER_H
