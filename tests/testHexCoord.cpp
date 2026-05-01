#include <iostream>
#include <string>

#include "utils/hexCoord.h"

using harmonies::utils::HexCoord;

namespace
{
    void check(bool condition, const std::string &message, int &failures)
    {
        if (condition)
        {
            std::cout << "[PASS] " << message << '\n';
        }
        else
        {
            std::cout << "[FAIL] " << message << '\n';
            ++failures;
        }
    }
}

int main()
{
    int failures = 0;

    HexCoord origin;
    HexCoord a(1, -1);
    HexCoord sameA(1, -1);
    HexCoord b(0, 1);

    check(origin.getQ() == 0 && origin.getR() == 0 && origin.getS() == 0,
          "Origin coordinates should be (0, 0, 0)",
          failures);
    check(a.getS() == 0,
          "Axial coordinates should produce the expected s coordinate",
          failures);
    check(a == sameA,
          "Two identical coordinates should be equal",
          failures);
    check(!(a == b),
          "Two different coordinates should not be equal",
          failures);
    check((b < a) || (a < b),
          "Different coordinates should be orderable for std::map",
          failures);

    std::cout << "\nFailures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
