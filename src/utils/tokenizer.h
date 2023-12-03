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


#ifndef ERIS_TOKENIZER_H
#define ERIS_TOKENIZER_H

#include "eris_config.h"

#include <string>
#include <vector>

namespace utils
{
#if IS_POSIX
constexpr const char* pathSeparator = "/";
#elif IS_WIN
constexpr const char* pathSeparator = "\\";
#else
#error "Unsupported platform"
#endif

//
// Split a give string with a delimiter and put the result in out.
//
// Possible scenario:
// split(out,"hello;world",";");
// --> out = ["hello","world"];
void
split(std::vector<std::string>* out, const std::string& buffer, const std::string& delimiter);

std::string
join(const std::vector<std::string>& in, const std::string& delimiter);

//
// Join multiple strings with a given delimiter into single string.
// Possible scenario:
// join(["hello","world"],2,";");
// --> "hello;world";
//
// Params : max denotes at which index should the operation stop.
// If max >= in.length() this parameter is simply ignored.
//
std::string
join(const std::vector<std::string>& in, size_t max, const std::string& delimiter);

// Remove last char from a string.
std::string
removeLast(const std::string& buffer, char delimiter);

}  // namespace base

#endif  // ERIS_TOKENIZER_H