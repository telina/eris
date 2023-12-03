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

#ifndef ERIS_BASE_LOGGER_H
#define ERIS_BASE_LOGGER_H

#include "eris_config.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <mutex>

#if IS_POSIX
#define __ERIS_FILENAME__ (::strrchr(__FILE__, '/') ? ::strrchr(__FILE__, '/') + 1 : __FILE__)
#elif IS_WIN
#define __ERIS_FILENAME__ (::strrchr(__FILE__, '\\') ? ::strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#error "Unsupported platform"
#endif

#define PRINT_LOG(level, format, ...)                                                       \
    do                                                                                      \
    {                                                                                       \
        utils::log(level, __ERIS_FILENAME__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__); \
    } while (0)

#define PRINT_INFO(format, ...)                  \
    do                                           \
    {                                            \
        utils::log(utils::Logger::Level::LOG_INFO, \
                  __ERIS_FILENAME__,             \
                  __FUNCTION__,                  \
                  __LINE__,                      \
                  format,                        \
                  ##__VA_ARGS__);                \
    } while (0)

#define PRINT_ERROR(format, ...)                  \
    do                                            \
    {                                             \
        utils::log(utils::Logger::Level::LOG_ERROR, \
                  __ERIS_FILENAME__,              \
                  __FUNCTION__,                   \
                  __LINE__,                       \
                  format,                         \
                  ##__VA_ARGS__);                 \
    } while (0)

#define PRINT_WARNING(format, ...)                  \
    do                                              \
    {                                               \
        utils::log(utils::Logger::Level::LOG_WARNING, \
                  __ERIS_FILENAME__,                \
                  __FUNCTION__,                     \
                  __LINE__,                         \
                  format,                           \
                  ##__VA_ARGS__);                   \
    } while (0)

#define PRINT_PANIC(format, ...)                  \
    do                                            \
    {                                             \
        utils::log(utils::Logger::Level::LOG_PANIC, \
                  __ERIS_FILENAME__,              \
                  __FUNCTION__,                   \
                  __LINE__,                       \
                  format,                         \
                  ##__VA_ARGS__);                 \
    } while (0)

#if ERIS_DEBUG_MODE
#define DPRINT_ERROR(format, ...) PRINT_ERROR(format, ##__VA_ARGS__)
#define DPRINT_WARNING(format, ...) PRINT_WARNING(format, ##__VA_ARGS__)
#define DPRINT_INFO(format, ...) PRINT_INFO(format, ##__VA_ARGS__)
#else
#define DPRINT_ERROR(format, ...)
#define DPRINT_WARNING(format, ...)
#define DPRINT_INFO(format, ...)
#endif

namespace utils
{
//
// Eris internal logger.
// It is a thread-safe logger with simple api.
// Note: do not use Logger directly, use the above defined macros instead.
//
class Logger final
{
public:
    enum class Level : int
    {
        LOG_ERROR,
        LOG_WARNING,
        LOG_CRITICAL,
        LOG_INFO,
        LOG_DEBUG,
        LOG_PANIC
    };
    static Logger&
    getInstance();
    ~Logger() = default;

    void
    log(Level level,
        const char* file,
        const char* function,
        int line,
        const char* format,
        va_list args);

    ERIS_DISALLOW_COPY_AND_ASSIGN(Logger);

private:
    Logger();
    static const char*
    levelName(Level level);

private:
    std::mutex lock_;
};

void
log(Logger::Level level, const char* file, const char* function, int line, const char* format, ...);

}  // namespace base

#endif  // ERIS_BASE_LOGGER_H