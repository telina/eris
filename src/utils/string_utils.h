// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// This file defines utility functions for working with strings.

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

#ifndef ERIS_BASE_STRING_UTILS_H
#define ERIS_BASE_STRING_UTILS_H

#include "eris_config.h"

#include <initializer_list>
#include <string>
#include <type_traits>
#include <vector>

namespace utils
{
// Equality for ASCII case-insensitive comparisons.
ERIS_EXPORT bool
EqualsCaseInsensitiveASCII(const std::string& a, const std::string& b);

// Converts the given string to it's ASCII-lowercase equivalent.
ERIS_EXPORT std::string
ToLowerASCII(std::string str);

}  // namespace base

#endif  // ERIS_BASE_STRING_UTILS_H
