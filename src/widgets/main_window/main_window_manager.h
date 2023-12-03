/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
 *
 * Authors:
 *      - 2021-2021 Mohammed Hussam Al Turjman (Mohammed_Hussam.Al_Turjman@dfki.de)
 *      - 2021-2023 Rhea Rinaldo (Rhea@Rinaldo.biz)
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

#ifndef ERIS_WIDGETS_MAIN_WINDOW_MAIN_WINDOW_MANAGER_H
#define ERIS_WIDGETS_MAIN_WINDOW_MAIN_WINDOW_MANAGER_H

#include "eris_config.h"
#include "file_manager.h"
#include "graphic_scene.h"
#include "working_dialog.h"
#include "operational.h"

#include <QObject>
#include <QThreadPool>
#include <unordered_map>
#include <atomic>

namespace widgets
{
class ERIS_EXPORT MainWindowManager : public QObject, public utils::GraphicScene::Factory::Observer
{
    Q_OBJECT
public:

    static MainWindowManager*
    getInstance();
    MainWindowManager() = delete;
    ~MainWindowManager() override;
    void
    resetAndDisable();
    bool
    createNewSession(const QString& model = QString());
    bool
    startVerification();
    bool
    transform();
    bool
    evaluate();
    bool
    openItemSettings();
    bool
    swapNodeCriticality();
    bool
    swapNodeId();
    bool
    openFile();
    bool
    optionAddRedundancy();
    bool
    setOutFileName();
    bool
    exportAs(utils::FileManager::Extension extension);
    bool
    save();
    bool
    saveAs();
    bool
    deleteItem();
    void
    showHelpWindow();
    bool
    setNodeMode(int id);
    bool
    setAnnotationMode(int id);
    bool
    openSubmodule(const std::string& path);
    bool
    closeSubmodule(const std::string& path);
    
    void
    setModelType(graph::Model::Type type);
    
    void
    updateTransformationState(int val);

private:
signals:
    void
    doUpdateProgressBar(int val);
    void
    transformDone();
    void
    updateOutputWidget(const QString& output, Qt::GlobalColor color);

private:
    explicit MainWindowManager(QObject* parent);

protected:
    void
    graphicSceneIsAboutToBeDeleted(GraphicScene*, int /*idx*/) override;
    void
    subGraphicSceneAdded(GraphicScene*, int /*idx*/) override;

private:
    int mTotalSessions = 0;
    std::unordered_map<std::string, int> mSubmodules;
    WorkingDialog mWorkingDialog;
    QThreadPool mThreadPool;
    std::atomic_bool mDone;
};

}  // namespace widgets

#endif  // ERIS_WIDGETS_MAIN_WINDOW_MAIN_WINDOW_MANAGER_H
