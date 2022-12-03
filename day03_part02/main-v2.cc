#include <iostream>
#include <iterator>
#include <ranges>
#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <functional>

template<typename It>
It& min(It& a) { return a; }

template<typename It>
It& min(It& a, It& b)
{
    return *std::get<0>(a) < *std::get<0>(b) ? a : b;
}

template<typename T, typename... Ts>
T& min(T& a, T& b, Ts&&... rest)
{
    return min(min(a, b), std::forward<Ts>(rest)...);
}

template<typename T>
bool are_equals(T&)
{
    return true;
}

template<typename T, typename... Ts>
bool are_equals(T& pair1, T& pair2, Ts&&... pairs)
{
    return std::get<0>(pair1) == std::get<0>(pair2) && are_equals(pair2, pairs...);
}

template<typename P>
bool ended(P&& p)
{
    return std::get<0>(p) == std::get<1>(p);
}

template<typename T, typename... Ts>
auto get_ended(T& pair, Ts&&... pairs)
{
    if (ended(pair))
        return pair;
    if constexpr (sizeof...(pairs) > 0)
        return get_ended(pairs...);
    return T{};
}

template<typename... Ts>
auto search_repeated(Ts&&... pairs)
{
    while((... && !ended(pairs)))
    {
        if (are_equals(pairs...)) return (..., std::get<0>(pairs));
        std::get<0>(min(pairs...))++;
    }
    return std::get<1>(get_ended(pairs...));
}

int item_priority(char v)
{
    if (v >= 'a' && v <= 'z') return v - 'a' + 1;
    if (v >= 'A' && v <= 'Z') return v - 'A' + 27;
    return 0;
}

int main()
{
    int total_priority = 0;
    int total_group_priority = 0;
    std::vector<std::string> strings{3};
    int count = 0;
    while(!std::cin.eof())
    {
        std::string line;
        std::getline(std::cin, line);

        auto start = std::begin(line);
        auto end = std::end(line);
        auto middle = start + (end - start) / 2;

        std::ranges::sort(start, middle);
        std::ranges::sort(middle, end);
        total_priority += item_priority(*search_repeated(std::make_tuple(start, middle), std::make_tuple(middle, end)));

        std::ranges::sort(start, end);
        strings[count] = line;
        count = (count + 1) % 3;
        if (count == 0)
        {
            total_group_priority += item_priority(*search_repeated(
                std::make_tuple(std::begin(strings[0]), std::end(strings[0])),
                std::make_tuple(std::begin(strings[1]), std::end(strings[1])),
                std::make_tuple(std::begin(strings[2]), std::end(strings[2]))
            ));
        }

    }

    std::cout << "total priority = " << total_priority << std::endl;
    std::cout << "total groups priority = " << total_group_priority << std::endl;
    return 0;
}

