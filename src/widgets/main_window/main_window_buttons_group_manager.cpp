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

#include "main_window_buttons_group_manager.h"
#include "main_window.h"
#include "logger.h"
#include "chart_view.h"
#include "node_item.h"
#include "edge_item.h"
#include "text_item.h"
#include "file_manager.h"
#include "prism_results_parser.h"
#include "scene_status.h"
#include "graphic_scene.h"
#include "transformer.h"
#include "checks.h"

#include <QToolBox>
#include <QButtonGroup>
#include <QGridLayout>
#include <QToolButton>
#include <QLabel>

namespace widgets
{
using graph::EdgeItem;
using utils::FileManager;
using graph::NodeItem;
using graph::TextItem;
MainWindowButtonsGroupManager*
MainWindowButtonsGroupManager::getInstance()
{
    // Intentionally leaked.
    static MainWindowButtonsGroupManager* instance = new MainWindowButtonsGroupManager(nullptr);
    return instance;
}

MainWindowButtonsGroupManager::MainWindowButtonsGroupManager(QObject* parent) : QObject(parent)
{
    mButtonGroup = new QButtonGroup(this);
    mButtonGroup->setExclusive(false);
    // connect(mButtonGroup, &QButtonGroup::idClicked, this, &MainWindow::buttonGroupClicked);
    connect(mButtonGroup,
            &QButtonGroup::idClicked,
            this,
            &MainWindowButtonsGroupManager::slotButtonsGroupButtonClicked);

    QGridLayout* layout = new QGridLayout();

    // Create and Add Button for a normal node
    NodeItem normalNode(graph::ComponentType::normalNode);
    QIcon normalNodeIcon(normalNode.image());
    QToolButton* normalNodeButton = new QToolButton();
    normalNodeButton->setIcon(normalNodeIcon);
    normalNodeButton->setIconSize(QSize(50, 50));
    normalNodeButton->setCheckable(true);
    mButtonGroup->addButton(normalNodeButton, graph::ComponentType::normalNode);

    layout->addWidget(normalNodeButton, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(tr("Normal Node")), 1, 0, Qt::AlignCenter);

    // Create and Add Button for a critical node
    NodeItem criticalNode(graph::ComponentType::criticalNode);
    QIcon criticalNodeIcon(criticalNode.image());
    QToolButton* criticalNodeButton = new QToolButton;
    criticalNodeButton->setIcon(criticalNodeIcon);
    criticalNodeButton->setIconSize(QSize(50, 50));
    criticalNodeButton->setCheckable(true);
    mButtonGroup->addButton(criticalNodeButton, graph::ComponentType::criticalNode);

    layout->addWidget(criticalNodeButton, 0, 1, Qt::AlignHCenter);
    layout->addWidget(new QLabel(tr("Critical Node")), 1, 1, Qt::AlignCenter);

    // Create and Add Environment Node
    NodeItem environmentNode(graph::ComponentType::environmentNode);
    QIcon environmentNodeIcon(environmentNode.image());
    QToolButton* environmentNodeButton = new QToolButton;
    environmentNodeButton->setIcon(environmentNodeIcon);
    environmentNodeButton->setIconSize(QSize(50, 50));
    environmentNodeButton->setCheckable(true);
    mButtonGroup->addButton(environmentNodeButton, graph::ComponentType::environmentNode);

    layout->addWidget(environmentNodeButton, 2, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(tr("Environment Node")), 3, 0, Qt::AlignCenter);

    // Create and Add Access Edge
    EdgeItem reachEdge(graph::ComponentType::reachEdge);
    QIcon reachEdgeIcon(reachEdge.image());
    QToolButton* reachEdgeButton = new QToolButton;
    reachEdgeButton->setIcon(reachEdgeIcon);
    reachEdgeButton->setIconSize(QSize(50, 50));
    reachEdgeButton->setCheckable(true);
    mButtonGroup->addButton(reachEdgeButton, graph::ComponentType::reachEdge);

    layout->addWidget(reachEdgeButton, 2, 1, Qt::AlignHCenter);
    layout->addWidget(new QLabel(tr("Reach Edge")), 3, 1, Qt::AlignCenter);

    // Create and Add Functional Edge
    EdgeItem functionalEdge(graph::ComponentType::functionalEdge);
    QIcon functionalEdgeIcon(functionalEdge.image());
    QToolButton* functionalEdgeButton = new QToolButton;
    functionalEdgeButton->setIcon(functionalEdgeIcon);
    functionalEdgeButton->setIconSize(QSize(50, 50));
    functionalEdgeButton->setCheckable(true);
    mButtonGroup->addButton(functionalEdgeButton, graph::ComponentType::functionalEdge);

    layout->addWidget(functionalEdgeButton, 4, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(tr("Functional Edge")), 5, 0, Qt::AlignCenter);

    // Create and Add Combined Edge
    EdgeItem securityEdge(graph::ComponentType::securityEdge);
    QIcon securityEdgeIcon(securityEdge.image());
    QToolButton* securityEdgeButton = new QToolButton;

    securityEdgeButton->setIcon(securityEdgeIcon);
    securityEdgeButton->setIconSize(QSize(50, 50));
    securityEdgeButton->setCheckable(true);
    mButtonGroup->addButton(securityEdgeButton, graph::ComponentType::securityEdge);

    layout->addWidget(securityEdgeButton, 4, 1, Qt::AlignHCenter);
    layout->addWidget(new QLabel(tr("Security Edge")), 5, 1, Qt::AlignCenter);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);

    layout->setRowStretch(6, 10);

    QWidget* itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    // ---------------------------------------------------------------------------------------------

    mAnnotationGroup = new QButtonGroup(this);
    mAnnotationGroup->setExclusive(false);
    connect(mAnnotationGroup,
            &QButtonGroup::idClicked,
            this,
            &MainWindowButtonsGroupManager::slotAnnotationGroupButtonClicked);

    /*connect(mAnnotationGroup,
            QOverload<int>::of(&QButtonGroup::buttonClicked),
            this,
            &MainWindow::annotationGroupClicked);*/
    QGridLayout* annotLayout = new QGridLayout;

    // Create and Add Button for a text widget
    QToolButton* textButton = new QToolButton;
    textButton->setIcon(QIcon(":/text"));
    textButton->setIconSize(QSize(50, 50));
    textButton->setCheckable(true);
    mAnnotationGroup->addButton(textButton);

    annotLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    annotLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
    annotLayout->setRowStretch(6, 10);

    QWidget* annotWidget = new QWidget;
    annotWidget->setLayout(annotLayout);

    // ---------------------------------------------------------------------------------------------

    mToolBox = new QToolBox();
    mToolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    mToolBox->setMinimumWidth(itemWidget->sizeHint().width());
    // mToolBox->setMaximumHeight(itemWidget->sizeHint().height());
    mToolBox->addItem(itemWidget, tr("Nodes and Edges"));
    mToolBox->addItem(annotWidget, tr("Annotations"));

    button_groups_.emplace_back(mButtonGroup);
    button_groups_.emplace_back(mAnnotationGroup);
}

void
MainWindowButtonsGroupManager::registerObserver(Observer* observer)
{
    ERIS_DCHECK(observer);
    observers_.emplace_back(observer);
}

void
MainWindowButtonsGroupManager::unRegisterObserver(Observer* observer)
{
    ERIS_DCHECK(observer);
    observers_.remove(observer);
}

void
MainWindowButtonsGroupManager::slotButtonsGroupButtonClicked(int id)
{
    auto iter = observers_.begin();
    while (iter != observers_.end())
    {
        (*iter)->ButtonsGroupButtonClicked(id);
        iter++;
    }
}

void
MainWindowButtonsGroupManager::slotAnnotationGroupButtonClicked(int id)
{
    auto iter = observers_.begin();
    while (iter != observers_.end())
    {
        (*iter)->AnnotationGroupButtonClicked(id);
        iter++;
    }
}

void
MainWindowButtonsGroupManager::enableAll(bool enable)
{
    for (int i = 0; i < button_groups_.size(); ++i)
    {
        QList<QAbstractButton*> buttons = button_groups_[i]->buttons();
        for (int j = 0; j < buttons.size(); ++j)
        {
            buttons[j]->setEnabled(enable);
        }
    }
}

}  // namespace widgets