#pragma once

#include "graphics/Color.hpp"
#include "graphics/ColorPair.hpp"

enum class ReservedColorNumber : Color::NcColorID
{
    BACKGROUND = 2,
    CURRENTCOLOR = 3,
    CURSOR_FG = 4,
    CURSOR_BG = 5
};

enum class ReservedPairNumber : ColorPair::NcPairID
{
    BACKGROUND = 2,
    CURRENTCOLOR = 3,
    CURSOR = 4
};
