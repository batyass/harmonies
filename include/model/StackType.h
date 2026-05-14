#pragma once

namespace harmonies
{
    namespace model
    {

        enum class StackType
        {
            Empty,      // Vide
            Incomplete, // Non scorant
            Tree,       // Arbre
            Mountain,   // Montagne
            Field,      // Champs
            Water,      // Eau
            Building    // Bâtiment
        };

    } // namespace model
} // namespace harmonies
