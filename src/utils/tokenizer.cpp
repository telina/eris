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


#include "tokenizer.h"

#include <algorithm>

namespace utils
{
void
split(std::vector<std::string>* out, const std::string& buffer, const std::string& delimiter)
{
    std::string s = buffer;
    size_t pos;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        token = s.substr(0, pos);

        if (!token.empty())
        {
            out->emplace_back(token);
        }

        s.erase(0, pos + delimiter.length());
    }

    if (!(s == delimiter || s.empty()))
    {
        out->emplace_back(s);
    }
}

std::string
join(const std::vector<std::string>& in, const std::string& delimiter)
{
    return join(in, in.size(), delimiter);
}

std::string
join(const std::vector<std::string>& in, size_t max, const std::string& delimiter)
{
    auto iter = in.begin();
    size_t idx = 0;
    std::string out;
    while (iter != in.end())
    {
        if (idx == max)
            break;

        out += (*iter) + delimiter;
        iter++;
        idx++;
    }
    return out;
}
std::string
removeLast(const std::string& buffer, char delimiter)
{
    std::string out;
    size_t idx = buffer.find_last_of(delimiter);
    out = buffer.substr(0, idx);
    return out;
}

}  // namespace base