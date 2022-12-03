#include <iostream>
#include <iterator>
#include <ranges>
#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <functional>

template<typename It>
It search_reapeted(It start1, It end1, It start2, It end2)
{
    while(start1 != end1 && start2 != end2)
    {
        if (*start1 == *start2) return start1;
        if (*start1 > *start2) start2++;
        else start1++;
    }

    return end1;
}

template<typename It>
It& min(It& a) { return a; }

template<typename It>
It& min(It& a, It& b)
{
    return *a < *b ? a : b;
}

template<typename T, typename... Ts>
T& min(T& a, T& b, Ts&&... rest)
{
    return min(min(a, b), rest...);
}

template<typename It>
It search_reapeted(
    It start1, It end1,
    It start2, It end2,
    It start3, It end3)
{
    while(start1 != end1 && start2 != end2 && start3 != end3)
    {
        if (*start1 == *start2 && *start2 == *start3) return start1;
        min(start1, start2, start3)++;
    }

    return end1;
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
        total_priority += item_priority(*search_reapeted(start, middle, middle, end));

        std::ranges::sort(start, end);
        strings[count] = line;
        count = (count + 1) % 3;
        if (count == 0)
        {
            total_group_priority += item_priority(*search_reapeted(
                std::begin(strings[0]), std::end(strings[0]),
                std::begin(strings[1]), std::end(strings[1]),
                std::begin(strings[2]), std::end(strings[2])
            ));
        }

    }

    std::cout << "total priority = " << total_priority << std::endl;
    std::cout << "total groups priority = " << total_group_priority << std::endl;
    return 0;
}

