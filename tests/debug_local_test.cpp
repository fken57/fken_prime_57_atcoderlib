#define LOCAL
#include "../debug.hpp"

#include <atcoder/modint>

#include <cassert>
#include <array>
#include <chrono>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

struct Point {
    int x;
    std::string name;
};

FIB_DEBUG_FIELDS(Point, value.x, value.name)

struct Streamed {
    int value;
};

std::ostream& operator<<(std::ostream& out, const Streamed& value) {
    return out << "Streamed(" << value.value << ')';
}

struct Recursive {};

auto fib_debug_fields(const Recursive& value) {
    return std::tie(value);
}

struct Unprintable {
    explicit Unprintable(int value) : value_(value) {}

private:
    int value_;
};

struct AutomaticStruct {
    int id;
    std::string label;
};

class CerrCapture {
public:
    CerrCapture() : original_(std::cerr.rdbuf(buffer_.rdbuf())) {}
    ~CerrCapture() { std::cerr.rdbuf(original_); }

    std::string str() const { return buffer_.str(); }

private:
    std::ostringstream buffer_;
    std::streambuf* original_;
};

void test_formatting() {
    using fib::local_debug::FormatOptions;
    using fib::local_debug::to_debug_string;

    const std::vector<std::vector<int>> matrix{{1, 2}, {3, 4}};
    assert(to_debug_string(matrix) ==
           "[\n  [1, 2],\n  [3, 4]\n]");
    const int raw_matrix[2][2]{{5, 6}, {7, 8}};
    assert(to_debug_string(raw_matrix) ==
           "[\n  [5, 6],\n  [7, 8]\n]");
    const std::vector<std::vector<std::vector<int>>> cube{
        {{1, 2}, {3, 4}},
        {{5, 6}, {7, 8}},
    };
    assert(to_debug_string(cube) ==
           "[\n"
           "  [\n"
           "    [1, 2],\n"
           "    [3, 4]\n"
           "  ],\n"
           "  [\n"
           "    [5, 6],\n"
           "    [7, 8]\n"
           "  ]\n"
           "]");
    assert(to_debug_string(std::array<int, 2>{9, 10}) == "[9, 10]");
    assert(to_debug_string(std::set<int>{3, 1, 2}) == "[1, 2, 3]");
    assert(to_debug_string(std::make_pair(1, std::string("x"))) ==
           "(1, \"x\")");
    assert(to_debug_string(std::make_tuple(true, '\n')) == "(true, '\\n')");
    assert(to_debug_string(std::optional<int>(3)) == "optional(3)");
    assert(to_debug_string(std::optional<int>{}) == "nullopt");
    assert(to_debug_string(std::variant<int, std::string>(std::string("v"))) ==
           "variant[1](\"v\")");
    assert(to_debug_string(Point{7, "node"}) ==
           "{\n  \"x\": 7,\n  \"name\": \"node\"\n}");
    assert(to_debug_string(AutomaticStruct{4, "auto"}) ==
           "{\n  \"_0\": 4,\n  \"_1\": \"auto\"\n}");
    assert(to_debug_string(Streamed{9}) == "Streamed(9)");

    using Mint = atcoder::modint998244353;
    assert(to_debug_string(Mint(-1)) == "998244352");
    assert(to_debug_string(std::vector<Mint>{Mint(1), Mint(2)}) == "[1, 2]");

    using DynamicMint = atcoder::modint;
    DynamicMint::set_mod(13);
    assert(to_debug_string(DynamicMint(-1)) == "12");

    const std::map<int, std::string> mapping{{1, "one"}, {2, "two"}};
    assert(to_debug_string(mapping) ==
           "[(1, \"one\"), (2, \"two\")]");

    FormatOptions short_output;
    short_output.max_elements = 2;
    assert(to_debug_string(std::vector<int>{1, 2, 3}, short_output) ==
           "[1, 2, ...]");

    FormatOptions shallow_output;
    shallow_output.max_depth = 2;
    assert(to_debug_string(Recursive{}, shallow_output).find("...") !=
           std::string::npos);

    assert(to_debug_string(Unprintable(1)).find("<unprintable:") == 0);
}

void test_stderr_output() {
    CerrCapture capture;
    fib::local_debug::print(1, std::string("line\nvalue"));
    assert(capture.str() == "1 \"line\\nvalue\"\n");

    const std::vector<int> values{1, 2};
    FIB_DEBUG(values, Point{3, "p"});
    int pointed_value = 5;
    int* pointer = &pointed_value;
    FIB_DEBUG(pointer);
    const std::string output = capture.str();
    assert(output.find("[debug ") != std::string::npos);
    assert(output.find("values, Point{3, \"p\"} = [1, 2] {") !=
           std::string::npos);
    assert(output.find("\"x\": 3") != std::string::npos);
    assert(output.find("pointer = <ptr:") != std::string::npos);
}

void test_iteration_limit() {
    CerrCapture capture;
    bool detected = false;
    try {
        FIB_LOOP_GUARD(loop, 2, 1000);
        for (int i = 0; i < 3; ++i) FIB_LOOP_TICK(loop, i);
    } catch (const fib::local_debug::LoopDetected& error) {
        detected = true;
        assert(std::string(error.what()).find("iteration limit exceeded") !=
               std::string::npos);
        assert(std::string(error.what()).find("state=2") != std::string::npos);
    }
    assert(detected);
    assert(capture.str().find("[loop-detected]") != std::string::npos);
}

void test_same_state_limit() {
    CerrCapture capture;
    bool detected = false;
    try {
        FIB_LOOP_GUARD(stagnant_loop, 100, 1000, 2);
        FIB_LOOP_TICK(stagnant_loop, 5);
        FIB_LOOP_TICK(stagnant_loop, 5);
        FIB_LOOP_TICK(stagnant_loop, 5);
    } catch (const fib::local_debug::LoopDetected& error) {
        detected = true;
        assert(std::string(error.what()).find("same state repeated") !=
               std::string::npos);
    }
    assert(detected);
    assert(capture.str().find("state=5") != std::string::npos);
}

void test_time_limit() {
    CerrCapture capture;
    bool detected = false;
    try {
        FIB_LOOP_GUARD(slow_loop, 100, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
        FIB_LOOP_TICK(slow_loop, "waiting");
    } catch (const fib::local_debug::LoopDetected& error) {
        detected = true;
        assert(std::string(error.what()).find("time limit exceeded") !=
               std::string::npos);
    }
    assert(detected);
    assert(capture.str().find("state=\"waiting\"") != std::string::npos);
}

int main() {
    static_assert(FIB_DEBUG_ENABLED == 1, "LOCAL debug must be enabled");
    test_formatting();
    test_stderr_output();
    test_iteration_limit();
    test_same_state_limit();
    test_time_limit();
}
