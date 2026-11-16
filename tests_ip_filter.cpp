#include <gtest/gtest.h>

// Подключаем наш main.cpp, main() там выключен через UNIT_TESTING
#include "main.cpp"

// --- split ---
TEST(SplitTest, HandlesEdgeCases)
{
    EXPECT_EQ(split("",     '.'), (std::vector<std::string>{""}));
    EXPECT_EQ(split("11",   '.'), (std::vector<std::string>{"11"}));
    EXPECT_EQ(split("..",   '.'), (std::vector<std::string>{"", "", ""}));
    EXPECT_EQ(split("11.",  '.'), (std::vector<std::string>{"11", ""}));
    EXPECT_EQ(split(".11",  '.'), (std::vector<std::string>{"", "11"}));
    EXPECT_EQ(split("11.22",'.'), (std::vector<std::string>{"11", "22"}));
}

// --- сортировка: 1.10.1.1 > 1.2.1.1 > 1.1.1.1 ---
TEST(SortTest, ReverseLexicographicNumericOrder)
{
    std::vector<std::vector<std::string>> pool = {
        {"1", "1",  "1", "1"},
        {"1", "10", "1", "1"},
        {"1", "2",  "1", "1"},
    };
    sort_reverse_lexicographically(pool);

    ASSERT_EQ(pool.size(), 3u);
    EXPECT_EQ(pool[0], (std::vector<std::string>{"1", "10", "1", "1"}));
    EXPECT_EQ(pool[1], (std::vector<std::string>{"1", "2",  "1", "1"}));
    EXPECT_EQ(pool[2], (std::vector<std::string>{"1", "1",  "1", "1"}));
}

// --- фильтр по первому байту == 1 ---
TEST(FilterTest, FirstByteIsOne)
{
    std::vector<std::vector<std::string>> pool = {
        {"1", "1", "1", "1"},
        {"2", "1", "1", "1"},
        {"1", "2", "3", "4"},
    };
    auto result = filter(pool, [](const auto& ip) { return ip[0] == "1"; });
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0], (std::vector<std::string>{"1", "1", "1", "1"}));
    EXPECT_EQ(result[1], (std::vector<std::string>{"1", "2", "3", "4"}));
}

// --- фильтр по 46.70.*.* ---
TEST(FilterTest, First46Second70)
{
    std::vector<std::vector<std::string>> pool = {
        {"46", "70", "1", "1"},
        {"46", "71", "1", "1"},
        {"45", "70", "1", "1"},
    };
    auto result = filter(pool, [](const auto& ip) {
        return ip[0] == "46" && ip[1] == "70";
    });
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0], (std::vector<std::string>{"46", "70", "1", "1"}));
}

// --- фильтр по любому байту == 46 ---
TEST(FilterTest, AnyByteIs46)
{
    std::vector<std::vector<std::string>> pool = {
        {"46", "0", "0", "0"},
        {"0", "46", "0", "0"},
        {"0", "0", "46", "0"},
        {"0", "0", "0", "46"},
        {"1", "2", "3", "4"},
    };
    auto result = filter(pool, [](const auto& ip) {
        return std::any_of(ip.begin(), ip.end(),
                           [](const std::string& p) { return p == "46"; });
    });
    EXPECT_EQ(result.size(), 4u);
}