#ifndef HEXCOORD_H
#define HEXCOORD_H

namespace harmonies
{
    namespace utils
    {
        struct CubeCoord
        {
            int x;
            int y;
            int z;
        };

        enum class HexDirection
        {
            Up,
            UpRight,
            DownRight,
            Down,
            DownLeft,
            UpLeft
        };

        class HexCoord
        {
        private:
            int q; // column in even-q offset coordinates
            int r; // row in even-q offset coordinates

        public:
            HexCoord(int q = 0, int r = 0);
            int getQ() const { return q; }
            int getR() const { return r; }
            int getS() const { return -q - r; }

            // Comparison for logic
            bool operator==(const HexCoord &other) const;

            // MANDATORY for std::map keys
            bool operator<(const HexCoord &other) const;
        };

        // Converts an even-q offset coordinate to an invariant cube coordinate.
        CubeCoord toCube(const HexCoord &coord);

        // Converts a cube coordinate back to the project's even-q offset convention.
        HexCoord fromCube(const CubeCoord &coord);

        // Returns the adjacent cell reached by taking one step in a board direction.
        HexCoord neighborInDirection(const HexCoord &coord, HexDirection direction);

        // Returns the coordinate reached after following the same board direction several times.
        HexCoord advanceInDirection(const HexCoord &coord, HexDirection direction, int distance);

        // Applies a geometric offset to an anchor without relying on raw q/r addition.
        HexCoord addPatternOffset(const HexCoord &anchor, const HexCoord &offset);

        // Rotates a geometric offset around the origin by 60° clockwise per step.
        HexCoord rotateOffset(const HexCoord &offset, int steps);

    } // namespace utils
} // namespace harmonies

#endif
