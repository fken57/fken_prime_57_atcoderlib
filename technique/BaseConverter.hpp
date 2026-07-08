#include "../templete.hpp"

 
string base_convert(const std::string& x, const int& from, const int& to)
{
    // Conversion table.
    // To be changed as appropriate.
    const std::string table = "0123456789abcdef";
    assert((2 <= from && from <= 16) && (2 <= to && to <= 16));

    unsigned long long sum = 0;
    for (char c : x)
    {
        sum = sum * from + table.find(c);
    }

    std::string res = "";
    do
    {
        int mod = sum % to;
        res.push_back(table[mod]);
        sum /= to;
    } while (sum);

    reverse(res.begin(),res.end());
    return res;
}