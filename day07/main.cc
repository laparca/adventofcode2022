/******************************************************************************
 * Copyright (C) 2022 Samuel R. Sevilla <laparca@laparca.es>
 *
 * This file is part of adventofcode2022.
 *
 * adventofcode2022 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 * 
 * adventofcode2022 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Foobar. If not, see <https://www.gnu.org/licenses/>.
 ******************************************************************************/
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>
#include <deque>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <iterator>

struct fs_entry;
using fs_entry_ptr = std::shared_ptr<fs_entry>;

struct fs_entry
{
    std::string name_;
    size_t size_;
    bool is_dir_;
    std::vector<fs_entry_ptr> entries_;

    fs_entry_ptr get_entry(const std::string& name)
    {
        auto res = std::find_if(std::begin(entries_), std::end(entries_), [name](const auto& entry) {
            return entry->name_ == name;
        });

        if (res == std::end(entries_))
            return nullptr;

        return *res;
    }
};

/* From: https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/ */
std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

size_t update_sizes(fs_entry_ptr& entry)
{
    if (entry->is_dir_)
        for (auto& v : entry->entries_)
            entry->size_ += update_sizes(v);

    return entry->size_;
}

std::vector<fs_entry_ptr> plain_dirs(const fs_entry_ptr& entry)
{
    std::vector<fs_entry_ptr> dirs;

    if (entry->is_dir_)
    {
        dirs.push_back(entry);

        for (const auto& e : entry->entries_)
        {
            auto vec = plain_dirs(e);
            std::copy(std::begin(vec), std::end(vec), std::back_inserter(dirs));
        }
    }

    return dirs;
}

fs_entry_ptr parse_input(std::istream& input)
{
    fs_entry_ptr root(new fs_entry{"/", 0, true, {}});
    std::deque<fs_entry_ptr> stack;

    stack.push_back(root);

    while(!input.eof())
    {
        std::string line;
        std::getline(input, line);

        auto tokens = split(line, ' ');

        if (tokens.size() == 0) continue;

        if (tokens[0] == "$")
        {
            if (tokens[1] == "cd")
            {
                if (tokens[2] == "/")
                {
                    while (stack.size() > 1)
                        stack.pop_back();
                }
                else if (tokens[2] == "..")
                {
                    stack.pop_back();
                }
                else
                {
                    auto back = stack.back();
                    auto entry = back->get_entry(tokens[2]);
                    if (entry == nullptr || !entry->is_dir_)
                        throw "ERROR";
                    stack.push_back(entry);
                }
            }
            /* It is not required to parse the 'ls' only its output
            else if (tokens[1] == "ls")
            {
            }
            */
        }
        else if (tokens[0] == "dir")
        {
            auto back = stack.back();
            back->entries_.emplace_back(new fs_entry{tokens[1], 0, true, {}});
        }
        else
        {
            auto back = stack.back();
            size_t size = std::stoul(tokens[0]);
            back->entries_.emplace_back(new fs_entry{tokens[1], size, false, {}});
        }
    }

    return root;
}

int main()
{
    /* The parser is the same for both part1 and part2 */
    fs_entry_ptr root = parse_input(std::cin);
    update_sizes(root);

    /* Part 1 */
    auto v = plain_dirs(root);
    auto last = std::remove_if(std::begin(v), std::end(v), [](const auto& entry) { return entry->size_ > 100000; });
    auto sum = std::accumulate(std::begin(v), last, 0, [](int total, const auto& entry) { return total + entry->size_; });

    std::cout << "sum " << sum << std::endl;

    /* Part 2 */
    size_t total_space    = 70000000;
    size_t space_required = 30000000;
    size_t used_space     = root->size_;
    size_t free_space     = total_space - used_space;
    size_t required_to_free = space_required - free_space;
    auto plain = plain_dirs(root);
    std::sort(std::begin(plain), std::end(plain), [](const auto& entry1, const auto& entry2) { return entry1->size_ < entry2->size_; });
    std::remove_if(std::begin(plain), std::end(plain), [required_to_free](const auto& entry){
        return entry->size_ < required_to_free;
    });

    std::cout << "Remove " << plain[0]->size_ << std::endl;
    return 0;
}
