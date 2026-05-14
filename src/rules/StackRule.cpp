#include "rules/StackRule.h"

namespace harmonies
{
    namespace rules
    {
        model::StackType StackRule(const model::BoardCell &cell)
        {
            if (cell.getHeight() == 0)
            {
                return model::StackType::Empty;
            }

            model::TokenType token = cell.getTokenStack().back();

            if (token == model::TokenType::GreenTree)
                return model::StackType::Tree;
            if (token == model::TokenType::GrayStone)
                return model::StackType::Mountain;
            if (token == model::TokenType::YellowField)
                return model::StackType::Field;
            if (token == model::TokenType::BlueWater)
                return model::StackType::Water;
            if (token == model::TokenType::RedBuilding)
                return model::StackType::Building;

            return model::StackType::Incomplete;
        }
    }
}
