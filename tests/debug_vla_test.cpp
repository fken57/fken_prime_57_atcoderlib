#define LOCAL
#include "../debug.hpp"

#include <atcoder/modint>

#include <cassert>
#include <sstream>
#include <string>

int main(int argc, char**) {
    using Mint = atcoder::modint998244353;

    const int size = argc + 1;
    Mint dp[size][2][3][2]{};
    dp[0][0][0][0] = 1;
    dp[size - 1][1][2][1] = -1;

    std::ostringstream captured;
    std::streambuf* original = std::cerr.rdbuf(captured.rdbuf());

    FIB_DEBUG(dp);
    FIB_DEBUG(FIB_VLA(dp), size);

    std::cerr.rdbuf(original);

    const std::string output = captured.str();
    assert(output.find("dp = [\n  [\n    [\n      [1, 0]") !=
           std::string::npos);
    assert(output.find("998244352") != std::string::npos);
    assert(output.find("FIB_VLA(dp), size =") != std::string::npos);
}
