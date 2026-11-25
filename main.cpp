#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
#include <functional>
#include <fstream> 
#include <ranges> 
#include <algorithm>
#include <stdexcept>
#ifdef _WIN32
#include <windows.h>
#endif

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
int parse_octet(const std::string& octet_str) {
    try {
        int val = std::stoi(octet_str);
        if (val < 0 || val > 255) {
            throw std::out_of_range("Октет вне диапазона (0-255): " + octet_str);
        }
        return val;
    } catch (const std::invalid_argument&) {
        throw std::invalid_argument("Октет не является числом: " + octet_str);
    }
}

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

void sort_reverse_lexicographically(std::vector<std::vector<std::string>>& pool)
{
    auto ip_key = [](const std::vector<std::string>& ip) {
        if (ip.size() != 4) {
            throw std::invalid_argument("Неверный формат IP: ожидается 4 октета");
        }
        return std::make_tuple(parse_octet(ip[0]), parse_octet(ip[1]), 
                               parse_octet(ip[2]), parse_octet(ip[3]));
    };
    
    std::ranges::sort(pool, [&ip_key](const auto& a, const auto& b) {
        return ip_key(a) > ip_key(b);
    });
}

auto filter(const std::vector<std::vector<std::string>>& pool, std::function<bool(const std::vector<std::string>&)> pred)
{
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
    #ifdef _WIN32
    SetConsoleCP(CP_UTF8);       // ввод
    SetConsoleOutputCP(CP_UTF8); // вывод
    #endif
    try
    {
        std::string filepath;

        if (argc > 1) {
            filepath = argv[1];
        } else {
            std::cout << "Введите путь к файлу с IP-адресами: ";
            std::getline(std::cin, filepath);
        }

        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Не удалось открыть файл: " + filepath);
        }

        std::vector<std::vector<std::string> > ip_pool;

        for(std::string line; std::getline(file, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            if (v.empty() || v[0].empty()) continue;
            ip_pool.push_back(split(v.at(0), '.'));
        }

        if (ip_pool.empty()) {
            throw std::runtime_error("Файл пуст или не содержит IP-адресов.");
        }

        // TODO reverse lexicographically sort
        sort_reverse_lexicographically(ip_pool);

        for (const auto& ip : ip_pool)
        {
            print_ip(ip);
        }

        // TODO filter by first byte and output
        auto view_1 = ip_pool | std::views::filter([](const auto& ip) {
            return ip[0] == "1";
        });
        for (const auto& ip : view_1) {
            print_ip(ip);
        }

        // TODO filter by first and second bytes and output
        auto view_46_70 = ip_pool | std::views::filter([](const auto& ip) {
            return ip[0] == "1" && ip[1] == "70";
        });
        for (const auto& ip : view_1) {
            print_ip(ip);
        }

        // TODO filter by any byte and output
        auto view_any_46 = ip_pool | std::views::filter([](const auto& ip) {
            return std::any_of(ip.begin(), ip.end(),
                               [](const std::string& part) { return part == "46"; });
        });
        for (const auto& ip : view_any_46) {
            print_ip(ip);
        }
    }
    catch(const std::exception &e)
    {
        std::cerr << "Ошибка выполнения: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
#endif