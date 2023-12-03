// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// This file defines utility functions for working with strings.

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


#include "string_utils.h"

#include <utility>
#include <type_traits>

namespace utils
{
// ASCII-specific tolower.  The standard library's tolower is locale sensitive,
// so we don't want to use it here.
template <typename CharT, typename = std::enable_if<std::is_integral<CharT>::value>>
CharT
ToLowerASCII(CharT c)
{
    return (c >= 'A' && c <= 'Z') ? (c + ('a' - 'A')) : c;
}

// ASCII-specific toupper.  The standard library's toupper is locale sensitive,
// so we don't want to use it here.
template <typename CharT, typename = std::enable_if<std::is_integral<CharT>::value>>
CharT
ToUpperASCII(CharT c)
{
    return (c >= 'a' && c <= 'z') ? (c + ('A' - 'a')) : c;
}

template <class CharT>
int
CompareCaseInsensitiveASCIIT(std::basic_string<CharT> a, std::basic_string<CharT> b)
{
    // Find the first characters that aren't equal and compare them.  If the end
    // of one of the strings is found before a nonequal character, the lengths
    // of the strings are compared.
    size_t i = 0;
    while (i < a.length() && i < b.length())
    {
        CharT lower_a = ToLowerASCII(a[i]);
        CharT lower_b = ToLowerASCII(b[i]);
        if (lower_a < lower_b)
            return -1;
        if (lower_a > lower_b)
            return 1;
        i++;
    }

    // End of one string hit before finding a different character. Expect the
    // common case to be "strings equal" at this point so check that first.
    if (a.length() == b.length())
        return 0;

    if (a.length() < b.length())
        return -1;
    return 1;
}

template <typename CharT>
std::basic_string<CharT>
ToLowerASCIIImpl(std::basic_string<CharT> str)
{
    std::basic_string<CharT> ret;
    ret.reserve(str.size());
    for (size_t i = 0; i < str.size(); i++)
        ret.push_back(ToLowerASCII(str[i]));
    return ret;
}

bool
EqualsCaseInsensitiveASCII(const std::string& a, const std::string& b)
{
    return a.size() == b.size() && utils::CompareCaseInsensitiveASCIIT(a, b) == 0;
}
std::string
ToLowerASCII(std::string str)
{
    return utils::ToLowerASCIIImpl(std::move(str));
}
}  // namespace base