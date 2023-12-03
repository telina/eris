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


#ifndef ERIS_TASK_H
#define ERIS_TASK_H

#include "logger.h"

#include <memory>
#include <system_error>
#include <thread>

namespace utils
{
template <typename _Callable, typename... _Args>
bool
asyncTask(_Callable&& __fn, _Args&&... __args)
{
    try
    {
        std::thread task(__fn, __args...);
        task.detach();
    }
    catch (std::system_error& error)
    {
        PRINT_ERROR("Failed to create new thread");
        return false;
    }
    return true;
}

template <typename _Callable, typename... _Args>
std::thread*
createTask(_Callable&& __fn, _Args&&... __args)
{
    std::unique_ptr<std::thread> task(nullptr);
    try
    {
        task.reset(new (std::nothrow) std::thread(__fn, __args...));
    }
    catch (std::system_error& error)
    {
        PRINT_ERROR("Failed to create new thread");
        return nullptr;
    }

    return task.release();
}
}  // namespace base

#endif  // ERIS_TASK_H