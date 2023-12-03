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


#ifndef UTILS_H
#define UTILS_H

#include "eris_config.h"
#if HAS_UNISTD
#include <unistd.h>
#else
#if _WIN32
#include <filesystem>
#define S_IXOTH static_cast<unsigned int>(std::filesystem::perms::others_exec)
#else
#error "Unable to find S_IXOTH"
#endif
#endif

#include <sys/stat.h>
#include <sys/types.h>

#include <stdlib.h>

namespace utils
{
static inline bool
commandExists(const std::string& command)
{  // Idea: https://stackoverflow.com/a/22786903/1972848
    struct stat sb;
#ifdef __linux__
    std::string delimiter = ":";
#elif _WIN32
    std::string delimiter = ";";
#endif

    std::string path = std::string(getenv("PATH") + delimiter);
    size_t start_pos = 0, end_pos = 0;

    while ((end_pos = path.find(':', start_pos)) != std::string::npos)
    {
        std::string current_path = path.substr(start_pos, end_pos - start_pos) + "/" + command;
        // alternatively: if (access(current_path, X_OK) == 0)
        if ((stat(current_path.c_str(), &sb) == 0) && (sb.st_mode & S_IXOTH))
        {
            return true;
        }

        start_pos = end_pos + 1;
    }
    return false;
}

// Check if a string contains only white spaces
static inline bool
isOnlyWhiteSpaces(const std::string& val)
{
    return val.find_first_not_of(' ') == std::string::npos;
}

}  // namespace base

#endif /* UTILS_H */
