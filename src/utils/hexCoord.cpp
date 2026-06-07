#include "utils/hexCoord.h"

namespace harmonies
{
    namespace utils
    {
        namespace
        {
            CubeCoord rotateCubeClockwise(const CubeCoord &coord)
            {
                return CubeCoord{-coord.z, -coord.x, -coord.y};
            }
        }

        // Constructor
        HexCoord::HexCoord(int q, int r) : q(q), r(r) {}

        // Equality check
        bool HexCoord::operator==(const HexCoord &other) const
        {
            return (q == other.q) && (r == other.r);
        }

        // Strict weak ordering for std::map
        bool HexCoord::operator<(const HexCoord &other) const
        {
            if (q != other.q)
            {
                return q < other.q;
            }
            return r < other.r;
        }

        CubeCoord toCube(const HexCoord &coord)
        {
            int q = coord.getQ();
            int r = coord.getR();
            int z = r - (q + (q & 1)) / 2;
            int x = q;
            int y = -x - z;
            return CubeCoord{x, y, z};
        }

        HexCoord fromCube(const CubeCoord &coord)
        {
            int q = coord.x;
            int r = coord.z + (coord.x + (coord.x & 1)) / 2;
            return HexCoord(q, r);
        }

        HexCoord neighborInDirection(const HexCoord &coord, HexDirection direction)
        {
            const bool isEvenColumn = (coord.getQ() % 2 == 0);

            switch (direction)
            {
            case HexDirection::Up:
                return HexCoord(coord.getQ(), coord.getR() - 1);
            case HexDirection::Down:
                return HexCoord(coord.getQ(), coord.getR() + 1);
            case HexDirection::UpRight:
                return isEvenColumn
                           ? HexCoord(coord.getQ() + 1, coord.getR())
                           : HexCoord(coord.getQ() + 1, coord.getR() - 1);
            case HexDirection::DownRight:
                return isEvenColumn
                           ? HexCoord(coord.getQ() + 1, coord.getR() + 1)
                           : HexCoord(coord.getQ() + 1, coord.getR());
            case HexDirection::DownLeft:
                return isEvenColumn
                           ? HexCoord(coord.getQ() - 1, coord.getR() + 1)
                           : HexCoord(coord.getQ() - 1, coord.getR());
            case HexDirection::UpLeft:
            default:
                return isEvenColumn
                           ? HexCoord(coord.getQ() - 1, coord.getR())
                           : HexCoord(coord.getQ() - 1, coord.getR() - 1);
            }
        }

        HexCoord advanceInDirection(const HexCoord &coord, HexDirection direction, int distance)
        {
            HexCoord current = coord;
            for (int step = 0; step < distance; ++step)
            {
                current = neighborInDirection(current, direction);
            }
            return current;
        }

        HexCoord addPatternOffset(const HexCoord &anchor, const HexCoord &offset)
        {
            CubeCoord anchorCube = toCube(anchor);
            CubeCoord offsetCube = toCube(offset);

            CubeCoord targetCube{
                anchorCube.x + offsetCube.x,
                anchorCube.y + offsetCube.y,
                anchorCube.z + offsetCube.z};

            return fromCube(targetCube);
        }

        HexCoord rotateOffset(const HexCoord &offset, int steps)
        {
            steps = ((steps % 6) + 6) % 6;

            CubeCoord rotated = toCube(offset);
            for (int i = 0; i < steps; ++i)
            {
                rotated = rotateCubeClockwise(rotated);
            }

            return fromCube(rotated);
        }

    } // namespace utils
} // namespace harmonies
