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

#ifndef ERIS_WIDGETS_MODULES_TAB_WIDGET_H
#define ERIS_WIDGETS_MODULES_TAB_WIDGET_H

#include <QTabWidget>
#include "eris_config.h"
#include "graphic_scene.h"
namespace widgets
{
//
// This class represents the tab, where eris |GraphicScenes| reside.
// Every |GraphicScene| has its own sub-tab, whereby the user can close
// each tab individually. The main eris module is called Main, all other submodules
// are named to their respective file name without the extension.
// Closing the main module tab will result in closing all its submodules.
// Switching to some tab will affect the |GraphicSceneFactory| by making the current
// working module the selected tab. For more details see : graphic_scene/graphic_scene.h
//
class ModulesTabWidget : public QTabWidget, public graph::GraphicScene::Factory::Observer
{
    Q_OBJECT
public:
    static ModulesTabWidget*
    getInstance();
    ERIS_DISALLOW_COPY_AND_ASSIGN(ModulesTabWidget);
    ~ModulesTabWidget() override;

protected:
    void
    graphicSceneIsAboutToBeDeleted(graph::GraphicScene*, int /*idx*/) override;
    void
    subGraphicSceneAdded(graph::GraphicScene*, int /*idx*/) override;

private slots:
    void
    closeRequested(int index);
    void
    currentChangedSlot(int idx);

private:
    explicit ModulesTabWidget(QWidget* parent = nullptr);
};

}  // namespace widgets

#endif  // ERIS_WIDGETS_MODULES_TAB_WIDGET_H
