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
#include <algorithm.hh>
#include <type_traits>
#include <iterator>
#include <functional.hh>

enum node_type { Leaf, List };
struct node {
    node(node_type type) : type_{type} {}
    node(const node&) = default;
    node(node&&) = default;
    virtual ~node() = default;
    virtual std::ostream& show(std::ostream&) const = 0;

    node_type type_;
};

using node_ptr = std::shared_ptr<node>;

std::ostream& operator<<(std::ostream& s, const node& n)
{
    return n.show(s);
}

std::ostream& operator<<(std::ostream& s, const node_ptr& node)
{
    if (node)
        node->show(s);
    else
        s << "(null)";
    return s;
}

struct node_leaf : public node
{
    node_leaf(int value) : node{Leaf}, value_{value} {}
    node_leaf() : node{Leaf}, value_{0} {}
    node_leaf(const node_leaf&) = default;
    node_leaf(node_leaf&&) = default;

    node_leaf& operator=(const node_leaf&) = default;
    node_leaf& operator=(node_leaf&&) = default;

    virtual std::ostream& show(std::ostream& s) const override
    {
        return (s << value_);
    }

    int value_;
};

struct node_list : public node
{
    node_list() : node{List} {}
    node_list(const node_list&) = default;
    node_list(node_list&&) = default;

    node_list& operator=(const node_list& n)
    {
        list_ = n.list_;
        type_ = n.type_;

        return *this;
    }

    virtual std::ostream& show(std::ostream& s) const override
    {
        s << "[";
        for (size_t i = 0; i < list_.size(); ++i)
            s << (i != 0 ? ", " : "") << list_[i];
        s << "]";
        return s;
    }
    std::vector<std::shared_ptr<node>> list_;
};

bool operator<(const node_ptr& a, const node_ptr& b);
bool operator==(const node_ptr& a, const node_ptr& b);

bool operator<(const node_leaf& a, const node_leaf& b)
{
    return a.value_ < b.value_;
}
bool operator==(const node_leaf& a, const node_leaf& b)
{
    return a.value_ == b.value_;
}

bool operator<(const node_list& a, const node_list& b)
{
    auto it_a = std::begin(a.list_);
    auto end_a = std::end(a.list_);
    auto it_b = std::begin(b.list_);
    auto end_b = std::end(b.list_);

    for (; it_a != end_a && it_b != end_b; ++it_a, ++it_b)
    {
        
        if (*it_a < *it_b)
            return true;
        else if (*it_b < *it_a)
            return false;
    }

    if (it_a == end_a)
        return true;
    
    return false;
}

bool operator==(const node_list& a, const node_list& b)
{
    if (a.list_.size() != b.list_.size())
        return false;
    
    auto it_a = std::begin(a.list_);
    auto end_a = std::end(a.list_);
    auto it_b = std::begin(b.list_);
    
    for (; it_a != end_a; ++it_a, ++it_b)
    {
        if (!(*it_a == *it_b))
            return false;
    }

    return true;
}
bool operator<(const node_ptr& a, const node_ptr& b)
{
    if (a->type_ == Leaf && b->type_ == Leaf)
        return *(dynamic_cast<node_leaf*>(&(*a))) < *(dynamic_cast<node_leaf*>(&(*b)));
    else /* at least one node is a list */
    {
        node_list list_a;
        node_list list_b;

        if (a->type_ == List)
            list_a = *(dynamic_cast<node_list*>(a.get()));
        else
            list_a.list_.emplace_back(a);
        
        if (b->type_ == List)
            list_b = *(dynamic_cast<node_list*>(b.get()));
        else
            list_b.list_.emplace_back(b);

        return list_a < list_b;
    }
}

bool operator==(const node_ptr& a, const node_ptr& b)
{
    if (a->type_ == Leaf && b->type_ == Leaf)
        return *(dynamic_cast<node_leaf*>(&(*a))) == *(dynamic_cast<node_leaf*>(&(*b)));
    else if (a->type_ == List && b->type_ == List)
        return *(dynamic_cast<node_list*>(&(*a))) == *(dynamic_cast<node_list*>(&(*b)));
    else
        return false;
}

template<typename It>
node_ptr create_number_from(It& start, It end)
{
    int num = *start - '0';
    ++start;
    while (*start != ',' && *start != ']' && start != end)
    {
        num = num * 10 + (*start - '0');
        ++start;
    }

    --start;
    return node_ptr{new node_leaf(num)};
}

template<typename It>
node_ptr create_list_from(It& start, It end)
{
    node_list list;
    start++;
    while(*start != ']' && start != end)
    {
        switch(*start)
        {
            case ',':
            case ']':
                break;
            case '[':
                list.list_.emplace_back(create_list_from(start, end));
                break;
            default:
                list.list_.emplace_back(create_number_from(start, end));
                break;
        }
        start++;
    }

    return node_ptr{new node_list(list)};
}

template<typename It>
node_ptr create_node_from(It start, It end)
{
    return create_list_from(start, end);
}

node_ptr create_node_from(const std::string& str)
{
    return create_node_from(std::begin(str), std::end(str));
}

int main()
{
    auto pairs = std::string{std::istreambuf_iterator<char>(std::cin), std::istreambuf_iterator<char>()}
        | laparca::trans::split('\n', true) /* convert to std::vector<std::string> */
        | laparca::trans::split("") /* convert to std::vector<std::vector<std::string>> */
        | laparca::trans::transform(laparca::trans::map((node_ptr(*)(const std::string&))create_node_from))
        ;

    auto correct_order = pairs | laparca::trans::map([](const std::vector<node_ptr>& v) { return v[0] < v[1]; })
        ;

    int sum = 0;
    for (size_t i = 0; i < correct_order.size(); ++i)
    {
        if (correct_order[i])
            sum += i + 1;
    }

    std::cout << "Result part 1 = " << sum << std::endl;


    auto all_input = pairs | laparca::trans::concat();

    using namespace std::string_literals;

    auto two_node = create_node_from("[[2]]"s);
    auto six_node = create_node_from("[[6]]"s);

    all_input.push_back(two_node);
    all_input.push_back(six_node);

    auto sorted_input = all_input | laparca::trans::sort(laparca::functional::less{});
    int two_index = 0;
    int six_index = 0;
    for (size_t i = 0; i < sorted_input.size(); ++i)
    {
        if (two_node == sorted_input[i])
        {
            two_index = i + 1;
        }
        if (six_node == sorted_input[i])
        {
            six_index = i + 1;
        }
    }

    std::cout << "Result part 2 = " << (two_index * six_index) << std::endl;
    return 0;
}