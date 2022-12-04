#include <iostream>
#include <ranges>
#include <algorithm>
#include <iterators.hh>
#include <sstream>
namespace std
{
    laparca::read_line_iterator<istream> begin(istream& stream)
    {
        return {stream};
    }

    laparca::read_line_iterator<istream> end(istream&)
    {
        return {};
    }
}

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

bool is_contained(std::vector<int> a, std::vector<int> b)
{
    return a[0] >= b[0] && a[1] <= b[1];
}

int main()
{
    int count = 0;
    int count_overlaps = 0;

    for (auto line : std::cin)
    {
        auto sections = split(line, ',');

        auto section1_str = split(sections[0], '-');
        auto section2_str = split(sections[1], '-');

        std::vector<int> section1;
        std::vector<int> section2;

        std::transform(std::begin(section1_str), std::end(section1_str),
                       std::back_insert_iterator(section1), [](auto& v) { return std::stoi(v); });
        std::transform(std::begin(section2_str), std::end(section2_str),
                       std::back_insert_iterator(section2), [](auto& v) { return std::stoi(v); });

        if (is_contained(section1, section2) || is_contained(section2, section1))
            count++;
        if ((section1[0] >= section2[0] && section1[0] <= section2[1]) ||
            (section1[1] >= section2[0] && section1[1] <= section2[1]) ||
            (section2[0] >= section1[0] && section2[0] <= section1[1]) ||
            (section2[1] >= section1[0] && section2[1] <= section1[1]))
            count_overlaps++;
    }

    std::cout << "sections fully contained count is " << count << std::endl;
    std::cout << "total overlaps is " << count_overlaps << std::endl;
    return 0;
}
