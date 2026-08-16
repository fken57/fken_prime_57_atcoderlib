#include "../print.hpp"

#include <atcoder/modint>

#include <array>
#include <cassert>
#include <iomanip>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

struct Point {
    int x;
    int y;
};

FIB_PRINT_FIELDS(Point, value.x, value.y)

struct Streamed {
    int value;
};

std::ostream& operator<<(std::ostream& out, const Streamed& value) {
    return out << "stream-" << value.value;
}

template <class Function>
std::string capture_stdout(Function function) {
    std::ostringstream captured;
    std::streambuf* original = std::cout.rdbuf(captured.rdbuf());
    function();
    std::cout.rdbuf(original);
    return captured.str();
}

void test_scalars_and_one_dimensional_ranges() {
    assert(capture_stdout([] { print(3, 4); }) == "3 4\n");
    assert(capture_stdout([] { print(std::string("hello")); }) == "hello\n");
    assert(capture_stdout([] { print(); }) == "\n");

    const std::vector<int> values{10, 20, 30};
    assert(capture_stdout([&] { print(values); }) == "10 20 30\n");
    assert(capture_stdout([&] { print(3, values); }) == "3 10 20 30\n");

    const std::array<int, 3> fixed{1, 2, 3};
    const int raw[3]{4, 5, 6};
    assert(capture_stdout([&] { print(fixed); }) == "1 2 3\n");
    assert(capture_stdout([&] { print(raw); }) == "4 5 6\n");
    assert(capture_stdout([&] { print(values, fixed); }) ==
           "10 20 30 1 2 3\n");
    assert(capture_stdout([] { print({7, 8, 9}); }) == "7 8 9\n");

    assert(capture_stdout([] { print(std::vector<int>{}); }) == "\n");
    assert(capture_stdout([] { print(true, false); }) == "1 0\n");
}

void test_multidimensional_ranges() {
    const std::vector<std::vector<int>> matrix{{1, 2}, {3, 4}};
    assert(capture_stdout([&] { print(matrix); }) == "1 2\n3 4\n");

    const int raw_matrix[2][2]{{5, 6}, {7, 8}};
    assert(capture_stdout([&] { print(raw_matrix); }) == "5 6\n7 8\n");

    const std::vector<std::vector<std::vector<int>>> cube{
        {{1, 2}, {3, 4}},
        {{5, 6}, {7, 8}},
    };
    assert(capture_stdout([&] { print(cube); }) ==
           "1 2\n3 4\n5 6\n7 8\n");

    const std::vector<std::vector<int>> with_empty_row{{}, {1, 2}};
    assert(capture_stdout([&] { print(with_empty_row); }) == "\n1 2\n");

    assert(capture_stdout([&] { print(9, matrix); }) == "9\n1 2\n3 4\n");
}

void test_character_ranges_and_strings() {
    const std::vector<char> word{'a', 'b', 'c'};
    assert(capture_stdout([&] { print(word); }) == "abc\n");

    const std::vector<std::vector<char>> grid{
        {'a', 'b', 'c'},
        {'d', 'e', 'f'},
    };
    assert(capture_stdout([&] { print(grid); }) == "abc\ndef\n");

    const char raw_grid[2][3]{{'g', 'h', 'i'}, {'j', 'k', 'l'}};
    assert(capture_stdout([&] { print(raw_grid); }) == "ghi\njkl\n");

    const std::vector<std::string> lines{"first", "second"};
    assert(capture_stdout([&] { print(lines); }) == "first\nsecond\n");
    assert(capture_stdout([&] { print(10, lines); }) ==
           "10\nfirst\nsecond\n");
    assert(capture_stdout([] { print(10, std::vector<std::string>{"only"}); }) ==
           "10\nonly\n");
}

void test_records_and_standard_wrappers() {
    assert(capture_stdout([] { print(std::make_pair(1, 2)); }) == "1 2\n");
    assert(capture_stdout([] { print(std::make_tuple(1, 2, 3)); }) ==
           "1 2 3\n");

    const std::vector<std::pair<int, int>> pairs{{1, 2}, {3, 4}};
    assert(capture_stdout([&] { print(pairs); }) == "1 2\n3 4\n");

    const std::map<int, std::string> mapping{{1, "one"}, {2, "two"}};
    assert(capture_stdout([&] { print(mapping); }) == "1 one\n2 two\n");

    assert(capture_stdout([] { print(Point{5, 6}); }) == "5 6\n");
    const std::vector<Point> points{{1, 2}, {3, 4}};
    assert(capture_stdout([&] { print(points); }) == "1 2\n3 4\n");
    assert(capture_stdout([] { print(Streamed{7}); }) == "stream-7\n");

    assert(capture_stdout([] { print(std::optional<int>{}); }) == "null\n");
    assert(capture_stdout([] { print(std::optional<int>{8}); }) == "8\n");
    assert(capture_stdout([] {
               print(std::variant<int, std::string>{std::string("variant")});
           }) == "variant\n");

    using Mint = atcoder::modint998244353;
    assert(capture_stdout([] { print(Mint(-1)); }) == "998244352\n");
}

void test_stream_formatting() {
    const std::ios::fmtflags original_flags = std::cout.flags();
    const std::streamsize original_precision = std::cout.precision();

    std::cout << std::fixed << std::setprecision(2);
    assert(capture_stdout([] { print(1.234); }) == "1.23\n");

    std::cout.flags(original_flags);
    std::cout.precision(original_precision);
}

int main() {
    test_scalars_and_one_dimensional_ranges();
    test_multidimensional_ranges();
    test_character_ranges_and_strings();
    test_records_and_standard_wrappers();
    test_stream_formatting();
}
