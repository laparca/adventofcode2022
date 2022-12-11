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
#include <io_util.hh>
#include <algorithm.hh>
#include <conversions.hh>
#include <functional.hh>
#include <deque>
#include <functional>
#include <string>
#include <cmath>

using mint = int64_t;

std::function<mint(mint)> operation_value_generator(const std::string& v)
{
    if (v == "old")
        return {
            [](mint a) -> mint { return a; }
        };
    return {
        [v](mint) -> mint { return std::stoll(v);}
    };
}

struct monkey
{
    std::deque<mint> items;
    std::function<mint(mint, mint)> operation;
    std::function<mint(mint)> operation_value;
    mint divisor;
    int on_true;
    int on_false;
    mint items_inspected;
};

std::ostream& operator<<(std::ostream& s, const monkey& m)
{
    s << "- Items: ";
    for (auto i : m.items) s << i << ", ";
    s << std::endl;


    return s;
}

monkey parse_monkey_info(const std::vector<std::string>& monkey_info)
{
    using namespace std::literals::string_literals;
    /*
    Monkey 0:                        monkey_info[0]
      Starting items: 79, 98         monkey_info[1]
      Operation: new = old * 19      monkey_info[2]
      Test: divisible by 23          monkey_info[3]
        If true: throw to monkey 2   monkey_info[4]
        If false: throw to monkey 3  monkey_info[5]
    */
    //monkey_info[1] | laparca::trans::split(":"s);

//    monkey_info[2] | laparca::trans::split(' ') | laparca::trans::for_each([](const auto& v) { std::cout << "'" << v << "', ";});
//    std::cout << std::endl;
    return {
        (monkey_info[1] | laparca::trans::split(':'))[1] | laparca::trans::split(',') | laparca::trans::transform<std::deque>(laparca::str_converter<mint>{}),
        ((monkey_info[2] | laparca::trans::split(':'))[1] | laparca::trans::split(' '))[3] == "*" ? std::function<mint(mint, mint)>{laparca::functional::mult{}} : std::function<mint(mint, mint)>{laparca::functional::plus{}},
        operation_value_generator((monkey_info[2] | laparca::trans::split(' '))[5]),
        std::stoll((monkey_info[3] | laparca::trans::split(' '))[3]),
        std::stoi((monkey_info[4] | laparca::trans::split(' '))[5]),
        std::stoi((monkey_info[5] | laparca::trans::split(' '))[5]),
        0
    };
}

#define D(x) do { if (debug) { x; } } while(0)
int main(int argc, char** argv)
{
    int rounds = 20;
    bool divide_by_3 = true;
    bool debug = false;
    using namespace std::literals::string_literals;
    auto monkeys = laparca::istream_to_container(std::cin)
                    | laparca::trans::convert_to<std::vector>()
                    | laparca::trans::split(""s)
                    | laparca::trans::transform(parse_monkey_info)
                    ;

    for (int i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "--rounds" && i+1 < argc)
            rounds = std::stoi(argv[++i]);
        else if (std::string(argv[i]) == "--no-div-3")
            divide_by_3 = false;
        else if (std::string(argv[i]) == "--debug")
            debug = true;
    }

//    monkeys | laparca::trans::for_each([](const auto& m){ std::cout << m << "------------" << std::endl;});
    auto cmn = monkeys | laparca::trans::transform([](const auto& m){return m.divisor;}) | laparca::trans::fold(1, [](mint r, mint d){ return r*d;});
    for (int i = 0; i < rounds; ++i)
    {
        int mnk_num = 0;
        for (auto& monkey : monkeys)
        {
            D(std::cout << "Monkey " << mnk_num << ":" << std::endl);
            while(monkey.items.size())
            {
                auto item = monkey.items.front();
                monkey.items.pop_front();
                monkey.items_inspected ++;

                D(std::cout << "  Monkey inspects and item with a worry level of " << item << "." << std::endl);

                auto new_worry_level = monkey.operation(item, monkey.operation_value(item));

                D(std::cout << "    Worry level it's modified to " << new_worry_level << "." << std::endl);
                
                if (divide_by_3)
                    new_worry_level = std::floor(new_worry_level / 3.0f);
                else
                    new_worry_level %= cmn;

                D(std::cout << "    Monkey gets bored with item. Worry level is divided by 3 to " << new_worry_level << "." << std::endl);
                
                if (new_worry_level % monkey.divisor)
                {
                    D(std::cout << "    Current worry level is not divisible by " << monkey.divisor << "." << std::endl;
                    std::cout << "    Item with worry level " << new_worry_level << " is thrown to monkey " << monkey.on_false << "." << std::endl);
                    monkeys[monkey.on_false].items.push_back(new_worry_level);
                }
                else
                {
                    D(std::cout << "    Current worry level is divisible by " << monkey.divisor << "." << std::endl;
                    std::cout << "    Item with worry level " << new_worry_level << " is thrown to monkey " << monkey.on_false << "." << std::endl);
                    monkeys[monkey.on_true].items.push_back(new_worry_level);
                }
            }

            mnk_num++;
        }
    }
//    monkeys | laparca::trans::for_each([](const auto& m){ std::cout << m << "------------" << std::endl;});

    auto sort_inspected = monkeys | laparca::trans::transform([](const auto& m) { return m.items_inspected; }) | laparca::trans::for_each([](const auto& i) { std::cout << i << ",";}) | laparca::trans::sort(laparca::functional::greater{});
    std::cout << std::endl;
    std::cout << sort_inspected[0] * sort_inspected[1] << std::endl;
    return 0;
}