/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
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

#include "modules_tab_widget.h"
#include "graphics_view.h"
#include "graphic_scene.h"
#include "checks.h"
#include "main_window_manager.h"
#include <QFileInfo>

namespace widgets
{
using graph::GraphicScene;

ModulesTabWidget::ModulesTabWidget(QWidget* parent) : QTabWidget(parent)
{
    setTabsClosable(true);
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeRequested(int)));
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(currentChangedSlot(int)));

    GRAPHIC_SCENE_FACTORY()->registerObserver(this);
}
ModulesTabWidget::~ModulesTabWidget() = default;

void
ModulesTabWidget::closeRequested(int idx)
{
    if (idx == 0)
    {
        GRAPHIC_SCENE_FACTORY()->unregisterObserver(this);
        clear();
        MainWindowManager::getInstance()->resetAndDisable();
        GRAPHIC_SCENE_FACTORY()->registerObserver(this);
    }
    else
    {
        ERIS_CHECK(GRAPHIC_SCENE_FACTORY()->deleteSubGraphicScene(idx));
    }
}
ModulesTabWidget*
ModulesTabWidget::getInstance()
{
    // Intentionally leaked.
    static ModulesTabWidget* instance = new ModulesTabWidget();
    return instance;
}
void
ModulesTabWidget::graphicSceneIsAboutToBeDeleted(graph::GraphicScene*, int idx)
{
    removeTab(idx);
}
void
ModulesTabWidget::subGraphicSceneAdded(graph::GraphicScene* g, int idx)
{
    if (idx == 0)
    {
        const int i = insertTab(idx, g->graphics_view(), tr("Main "));
        setTabToolTip(i, g->path());
    }
    else
    {
        QFileInfo fileInfo(g->path());
        const int i = insertTab(idx, g->graphics_view(), fileInfo.fileName());
        setTabToolTip(i, g->path());
    }
}
void
ModulesTabWidget::currentChangedSlot(int idx)
{
    if (idx < 0)
    {
        return;
    }

    ERIS_CHECK(GRAPHIC_SCENE_FACTORY()->setCurrentGraphicScene(idx));
    MainWindowActionsManager::getInstance()->toggleRateInterpretation(GRAPHIC_SCENE_FACTORY()
            ->current()->getCurrentRateInterpretation());
}
}  // namespace widgets