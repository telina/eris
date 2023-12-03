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

#include "logger.h"

#if HAS_UNISTD
#include <unistd.h>
#endif
#include <memory>
#include <sstream>

namespace utils
{
Logger&
Logger::getInstance()
{
    static Logger globalLogger;

    return globalLogger;
}

const char*
Logger::levelName(Level level)
{
    switch (level)
    {
        case Level::LOG_WARNING: return "WARNING";
        case Level::LOG_INFO: return "INFO";
        case Level::LOG_ERROR: return "ERROR";
        case Level::LOG_CRITICAL: return "CRITICAL";
        case Level::LOG_PANIC: return "PANIC";
        case Level::LOG_DEBUG: return "DEBUG";

        default: return "UNKNOWN";
    }
}

Logger::Logger() : lock_()
{
}

void
Logger::log(Level level,
            const char* file,
            const char* function,
            int line,
            const char* format,
            va_list args)
{
    std::lock_guard<std::mutex> guard(lock_);

    FILE* stream = stdout;

    switch (level)
    {
        case Level::LOG_ERROR:
        case Level::LOG_CRITICAL:
        case Level::LOG_WARNING:
        case Level::LOG_PANIC: stream = stderr; break;
        default: break;
    }

    if (level == Level::LOG_DEBUG)
    {
        fprintf(stream, "[%-8s][%s-%s-%d] ", levelName(level), file, function, line);
    }
    else
    {
        std::stringstream ss;

#if IS_POSIX
        ss << "\033[1;";
        switch (level)
        {
            case Level::LOG_INFO: 
                ss << "32"; 
                break;
            case Level::LOG_WARNING: ss << "33"; break;
            case Level::LOG_ERROR:
            case Level::LOG_CRITICAL:
            case Level::LOG_PANIC: ss << "31"; break;

            default: ss << "37"; break;
        }
        ss << "m";

#endif
        ss << levelName(level);

#if IS_POSIX
        ss << "\033[0m";
#endif

#if HAS_UNISTD
        fprintf(stream,
                "[%-8s-%d-%ld](%s-%d) ",
                ss.str().c_str(),
                getpid(),
                pthread_self(),
                file,
                line);
#else
        fprintf(stream, "[%-8s](%s-%d) ", ss.str().c_str(), file, line);

#endif
    }

    vfprintf(stream, format, args);

#ifdef IS_WIN
    fprintf(stream, "\n\r");
#else
    fprintf(stream, "\n");
#endif

    if (level == Level::LOG_PANIC)
        abort();
}

void
log(Logger::Level level, const char* file, const char* function, int line, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);

    Logger::getInstance().log(level, file, function, line, format, ap);

    va_end(ap);
}

}  // namespace base