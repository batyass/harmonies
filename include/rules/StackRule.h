#pragma once

#include "model/BoardCell.h"
#include "model/StackType.h"

namespace harmonies
{
    namespace rules
    {
        model::StackType StackRule(const model::BoardCell &cell);
    }
}
