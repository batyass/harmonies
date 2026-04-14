#ifndef HEXCOORD_H
#define HEXCOORD_H

namespace harmonies
{
    namespace utils
    {

        class HexCoord
        {
        private:
            int q; // axial coordinate
            int r; // axial coordinate

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

    } // namespace utils
} // namespace harmonies

#endif