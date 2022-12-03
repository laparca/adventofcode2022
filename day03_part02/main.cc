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

#define T(a, b, c) *a > *b && *b > *c
#define D(x) do { std::cout << " " << #x << std::endl; x} while(0)
template<typename It>
It search_reapeted(
    It start1, It end1,
    It start2, It end2,
    It start3, It end3)
{
    while(start1 != end1 && start2 != end2 && start3 != end3)
    {
        #if 0
        start1 > start2 > start3
        start1 > start3 > start2
        start2 > start1 > start3
        start2 > start3 > start1
        start3 > start1 > start2
        start3 > start2 > start1
        #endif
        std::cout << *start1 << *start2 << *start3 << std::endl;
        if (*start1 == *start2 && *start2 == *start3) return start1;
        else if (T(start1, start2, start3)) start3++;
        else if (T(start1, start3, start2)) start2++;
        else if (T(start2, start1, start3)) start3++;
        else if (T(start2, start3, start1)) start1++;
        else if (T(start3, start1, start2)) start2++;
        else start1++;
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
        std::cout << "-----" << std::endl;
        //std::cout << line << std::endl;

        auto start = std::begin(line);
        auto end = std::end(line);
        auto middle = start + (end - start) / 2;

        //std::for_each(start, middle, [](auto a){ std::cout << a;});
        //std::cout << std::endl;
        //std::for_each(middle, end, [](auto a){ std::cout << a;});
        //std::cout << std::endl;

        std::ranges::sort(start, middle);
        std::ranges::sort(middle, end);
        //std::cout << item_priority(*search_reapeted(start, middle, middle, end)) << std::endl;
        total_priority += item_priority(*search_reapeted(start, middle, middle, end));

        std::ranges::sort(start, end);
        strings[count] = line;
        std::cout << strings[count] << std::endl;
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

