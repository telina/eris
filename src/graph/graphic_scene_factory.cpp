/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Odlanir.de).
 *
 * Authors:
 *      - 2020-2021 Rhea Rinaldo (Rhea@Odlanir.de)
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

#include "graphic_scene.h"
#include "logger.h"
#include "checks.h"
namespace graph
{
    
GraphicScene::Factory*
GraphicScene::Factory::getInstance()
{
    static std::unique_ptr<Factory> instance(new Factory());
    return instance.get();
}

GraphicScene*
GraphicScene::Factory::current()
{
    std::lock_guard<std::recursive_mutex> guard(lock_);

    if (scenes_.empty())
    {
        return nullptr;
    }
        
    return scenes_[idx_].get();
}

GraphicScene*
GraphicScene::Factory::at(int i)
{
    std::lock_guard<std::recursive_mutex> guard(lock_);

    if (i < scenes_.size() && i >= 0)
    {
        return scenes_[i].get();
    }

    return nullptr;
}

GraphicScene::Factory::Factory() : scenes_(), observers_(), lock_(), idx_(-1)
{
    
}

bool
GraphicScene::Factory::setCurrentGraphicScene(int idx)
{
    std::lock_guard<std::recursive_mutex> guard(lock_);

    if (idx < scenes_.size() && idx >= 0)
    {
        idx_ = idx;
        return true;
    }

    return false;
}

bool
GraphicScene::Factory::addSubGraphicScene()
{
    int out = 0;
    return addSubGraphicScene(&out);
}

bool
GraphicScene::Factory::addSubGraphicScene(int* idx_out, const QString& model_path)
{
    std::lock_guard<std::recursive_mutex> guard(lock_);

    int idx;
    auto graphic_scene = GraphicScene::Create(nullptr);
    if (!graphic_scene)
    {
        PRINT_ERROR("There is not enough memory to create another graphics scene");
        return false;
    }
    scenes_.emplace_back(std::move(graphic_scene));
    idx = static_cast<int>(scenes_.size()) - 1;
    if (!model_path.isEmpty())
    {
        ERIS_CHECK(scenes_[idx]->load(model_path));
    }
    notifyAdded(scenes_[idx].get(), idx);
    *idx_out = idx;
    return true;
}

bool
GraphicScene::Factory::deleteSubGraphicScene(int idx)
{
    std::lock_guard<std::recursive_mutex> guard(lock_);
    if (idx >= scenes_.size())
        return false;

    auto iter = scenes_.begin();
    for (int i = 0; i < scenes_.size(); ++i)
    {
        if (i == idx)
        {
            notifyDeleted(iter->get(), idx);
            iter->reset();
            scenes_.erase(iter);
            return true;
        }
        iter++;
    }
    return true;
}

void
GraphicScene::Factory::clear()
{
    std::lock_guard<std::recursive_mutex> guard(lock_);

    for (int i = 0; i < scenes_.size(); ++i)
    {
        ERIS_CHECK(deleteSubGraphicScene(i));
    }
    scenes_.clear();
    idx_ = -1;
}

void
GraphicScene::Factory::registerObserver(GraphicScene::Factory::Observer* o)
{
    std::lock_guard<std::recursive_mutex> guard(lock_);

    auto iter = std::find(observers_.begin(), observers_.end(), o);
    if (iter != observers_.end())
    {
        PRINT_PANIC("Observer %p already exist ", o);
        return;
    }

    observers_.emplace_back(o);
}

void
GraphicScene::Factory::unregisterObserver(GraphicScene::Factory::Observer* o)
{
    std::lock_guard<std::recursive_mutex> guard(lock_);

    auto iter = std::find(observers_.begin(), observers_.end(), o);
    if (iter == observers_.end())
    {
        PRINT_PANIC("Observer %p does not exist ", o);
        return;
    }
    observers_.remove(o);
}

void
GraphicScene::Factory::notifyDeleted(GraphicScene*g, int idx)
{
    auto iter = observers_.begin();
    while (iter != observers_.end())
    {
        (*iter)->graphicSceneIsAboutToBeDeleted(g, idx);
        iter++;
    }
}
void
GraphicScene::Factory::notifyAdded(GraphicScene*g, int idx)
{
    auto iter = observers_.begin();
    while (iter != observers_.end())
    {
        (*iter)->subGraphicSceneAdded(g, idx);
        iter++;
    }
}

int
GraphicScene::Factory::getIdx()
{
    std::lock_guard<std::recursive_mutex> guard(lock_);
    return idx_;
}

size_t
GraphicScene::Factory::size()
{
    std::lock_guard<std::recursive_mutex> guard(lock_);

    return scenes_.size();
}

bool
GraphicScene::Factory::isEmpty()
{
    std::lock_guard<std::recursive_mutex> guard(lock_);

    return scenes_.empty();
}

GraphicScene::Factory::~Factory() = default;

}  // namespace graph