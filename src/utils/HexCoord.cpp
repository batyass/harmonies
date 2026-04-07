#include "utils/HexCoord.h"

namespace harmonies {
namespace utils {

// Constructor
HexCoord::HexCoord(int q, int r) : q(q), r(r) {}

// Equality check
bool HexCoord::operator==(const HexCoord& other) const {
    return (q == other.q) && (r == other.r);
}

// Strict weak ordering for std::map
bool HexCoord::operator<(const HexCoord& other) const {
    if (q != other.q) {
        return q < other.q;
    }
    return r < other.r;
}

} // namespace utils
} // namespace harmonies