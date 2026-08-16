#include "../input.hpp"

#include <atcoder/modint>

#include <array>
#include <cassert>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

struct AutomaticRecord {
    int id;
    std::string name;
};

struct RegisteredRecord {
    int left;
    int right;
};

FIB_INPUT_FIELDS(RegisteredRecord, value.right, value.left)

struct StreamRecord {
    int doubled;
};

std::istream& operator>>(std::istream& input, StreamRecord& value) {
    int raw = 0;
    if (input >> raw) value.doubled = raw * 2;
    return input;
}

enum class Number : int {
    zero = 0,
    one = 1,
    two = 2,
};

template <class Function>
void with_stdin(const std::string& text, Function function) {
    std::istringstream source(text);
    std::streambuf* original_buffer = std::cin.rdbuf(source.rdbuf());
    const std::ios::iostate original_state = std::cin.rdstate();
    std::cin.clear();

    function();

    std::cin.rdbuf(original_buffer);
    std::cin.clear(original_state);
}

void test_scalars_and_failure() {
    with_stdin("3 4 token 1 0 2", [] {
        int n = 0;
        long long m = 0;
        std::string word;
        bool yes = false;
        bool no = true;
        Number number = Number::zero;
        assert(input(n, m, word, yes, no, number));
        assert(n == 3 && m == 4 && word == "token");
        assert(yes && !no && number == Number::two);
    });

    with_stdin("2 99", [] {
        bool value = false;
        int untouched = 7;
        assert(!input(value, untouched));
        assert(untouched == 7);
        assert(std::cin.fail());
    });

    with_stdin("", [] {
        int value = 0;
        assert(!input(value));
    });

    with_stdin("", [] { assert(input()); });
}

void test_existing_containers() {
    with_stdin("1 2 3 4 5 6", [] {
        std::vector<int> values(3);
        std::array<int, 3> fixed{};
        assert(input(values, fixed));
        assert((values == std::vector<int>{1, 2, 3}));
        assert((fixed == std::array<int, 3>{4, 5, 6}));
    });

    with_stdin("1 2 3 4", [] {
        std::vector<std::vector<int>> matrix(2, std::vector<int>(2));
        assert(input(matrix));
        assert((matrix == std::vector<std::vector<int>>{{1, 2}, {3, 4}}));
    });

    with_stdin("abc def", [] {
        std::vector<char> word(3);
        std::vector<std::string> words(1);
        assert(input(word, words));
        assert((word == std::vector<char>{'a', 'b', 'c'}));
        assert((words == std::vector<std::string>{"def"}));
    });

    with_stdin("abcd", [] {
        char raw[4]{};
        assert(input(raw));
        assert(raw[0] == 'a' && raw[1] == 'b' && raw[2] == 'c' &&
               raw[3] == 'd');
    });

    with_stdin("1 0 1", [] {
        std::vector<bool> bits(3);
        assert(input(bits));
        assert(bits[0] && !bits[1] && bits[2]);
    });
}

void test_sized_and_shaped() {
    with_stdin("3 10 20 30", [] {
        int size = 0;
        std::vector<int> values;
        assert(input(size, sized(values, size)));
        assert(size == 3);
        assert((values == std::vector<int>{10, 20, 30}));
    });

    with_stdin("7 8 9", [] {
        std::vector<int> values;
        assert(input(sized(values, 3)));
        assert((values == std::vector<int>{7, 8, 9}));
    });

    with_stdin("2 3 1 2 3 4 5 6", [] {
        int height = 0;
        int width = 0;
        std::vector<std::vector<int>> matrix;
        assert(input(height, width, shaped(matrix, height, width)));
        assert((matrix ==
                std::vector<std::vector<int>>{{1, 2, 3}, {4, 5, 6}}));
    });

    with_stdin("2 1 2 10 11 20 21", [] {
        int x = 0;
        int y = 0;
        int z = 0;
        std::vector<std::vector<std::vector<int>>> cube;
        assert(input(x, y, z, shaped(cube, x, y, z)));
        assert(cube.size() == 2 && cube[0].size() == 1 &&
               cube[0][0].size() == 2);
        assert(cube[0][0][0] == 10 && cube[0][0][1] == 11);
        assert(cube[1][0][0] == 20 && cube[1][0][1] == 21);
    });

    with_stdin("", [] {
        int negative = -1;
        std::vector<int> values{9};
        assert(!input(sized(values, negative)));
        assert((values == std::vector<int>{9}));
    });

    with_stdin("", [] {
        std::vector<int> values{9};
        assert(input(sized(values, 0)));
        assert(values.empty());
    });

    with_stdin("", [] {
        int height = 2;
        int width = -1;
        std::vector<std::vector<int>> matrix{{9}};
        assert(!input(shaped(matrix, height, width)));
        assert((matrix == std::vector<std::vector<int>>{{9}}));
    });
}

void test_character_grids() {
    with_stdin("2 3 abc def", [] {
        int height = 0;
        int width = 0;
        std::vector<std::vector<char>> grid;
        assert(input(height, width, shaped(grid, height, width)));
        assert((grid == std::vector<std::vector<char>>{
                            {'a', 'b', 'c'}, {'d', 'e', 'f'}}));
    });

    with_stdin("2 3 abc def", [] {
        int height = 0;
        int width = 0;
        std::vector<std::string> grid;
        assert(input(height, width, shaped_lines(grid, height, width)));
        assert((grid == std::vector<std::string>{"abc", "def"}));
    });

    with_stdin("2 3 abc xy", [] {
        int height = 0;
        int width = 0;
        std::vector<std::string> grid;
        assert(!input(height, width, shaped_lines(grid, height, width)));
        assert(std::cin.fail());
    });

    with_stdin("2 0", [] {
        int height = 0;
        int width = 1;
        std::vector<std::string> grid;
        assert(input(height, width, shaped_lines(grid, height, width)));
        assert((grid == std::vector<std::string>{"", ""}));
    });
}

void test_records_and_modint() {
    with_stdin("1 alice 20 10 7 998244353", [] {
        AutomaticRecord automatic{};
        RegisteredRecord registered{};
        StreamRecord streamed{};
        atcoder::modint998244353 mint{};
        assert(input(automatic, registered, streamed, mint));
        assert(automatic.id == 1 && automatic.name == "alice");
        assert(registered.left == 10 && registered.right == 20);
        assert(streamed.doubled == 14);
        assert(mint.val() == 0);
    });

    with_stdin("1 2 word 3", [] {
        std::pair<int, int> pair;
        std::tuple<std::string, int> tuple;
        assert(input(pair, tuple));
        assert(pair == std::make_pair(1, 2));
        assert(tuple == std::make_tuple(std::string("word"), 3));
    });
}

void test_lines() {
    with_stdin("hello world\n", [] {
        std::string line;
        assert(input_line(line));
        assert(line == "hello world");
    });

    with_stdin("3\nhello world\n", [] {
        int count = 0;
        std::string remainder;
        std::string line;
        assert(input(count));
        assert(input_line(remainder));
        assert(remainder.empty());
        assert(input_line(line));
        assert(line == "hello world");
    });

    with_stdin("3\n  hello world\n", [] {
        int count = 0;
        std::string line;
        assert(input(count));
        assert(input_line_ws(line));
        assert(line == "hello world");
    });
}

int main() {
    test_scalars_and_failure();
    test_existing_containers();
    test_sized_and_shaped();
    test_character_grids();
    test_records_and_modint();
    test_lines();
}
