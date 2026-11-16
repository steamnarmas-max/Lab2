#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

// Печать одного IP (тот же самый цикл, что был в вашем main)
void print_ip(const std::vector<std::string>& ip)
{
    for (auto ip_part = ip.cbegin(); ip_part != ip.cend(); ++ip_part)
    {
        if (ip_part != ip.cbegin())
        {
            std::cout << ".";
        }
        std::cout << *ip_part;
    }
    std::cout << std::endl;
}

// Обратная лексикографическая (числовая) сортировка
void sort_reverse_lexicographically(std::vector<std::vector<std::string>>& pool)
{
    auto ip_key = [](const std::vector<std::string>& ip) {
        return std::make_tuple(std::stoi(ip[0]), std::stoi(ip[1]),
                               std::stoi(ip[2]), std::stoi(ip[3]));
    };
    std::sort(pool.begin(), pool.end(),
              [&ip_key](const auto& a, const auto& b) {
                  return ip_key(a) > ip_key(b);
              });
}

// decltype + auto + лямбда — фильтр по предикату с сохранением порядка
template <typename Predicate>
auto filter(const std::vector<std::vector<std::string>>& pool, Predicate pred)
{
    // std::decay_t убирает const и & из типа, полученного через decltype
    std::decay_t<decltype(pool)> result; 
    
    for (const auto& ip : pool) {
        if (pred(ip)) {
            result.push_back(ip);
        }
    }
    return result;
}
#ifndef UNIT_TESTING
int main(int argc, char const *argv[])
{
    try
    {
        std::vector<std::vector<std::string> > ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.push_back(split(v.at(0), '.'));
        }

        // TODO reverse lexicographically sort
        sort_reverse_lexicographically(ip_pool);

        // Полный список после сортировки
        for (const auto& ip : ip_pool)
        {
            print_ip(ip);
        }

        // TODO filter by first byte and output
        auto pool_1 = filter(ip_pool, [](const auto& ip) {
            return ip[0] == "1";
        });
        for (const auto& ip : pool_1) print_ip(ip);

        // TODO filter by first and second bytes and output
        auto pool_46_70 = filter(ip_pool, [](const auto& ip) {
            return ip[0] == "46" && ip[1] == "70";
        });
        for (const auto& ip : pool_46_70) print_ip(ip);

        // TODO filter by any byte and output
        auto pool_any_46 = filter(ip_pool, [](const auto& ip) {
            return std::any_of(ip.begin(), ip.end(),
                               [](const std::string& part) { return part == "46"; });
        });
        for (const auto& ip : pool_any_46) print_ip(ip);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
#endif