#include <ncurses.h>
#include <limits>
#include <algorithm>

#include "core/Vector2.hpp"
#include "graphics/ColorConstants.hpp"
#include "graphics/ColorPair.hpp"
#include "ui/Cursor.hpp"

Cursor::Cursor()
{
    int int_min = std::numeric_limits<int>::min();
    int int_max = std::numeric_limits<int>::max();

    Cursor::setMoveLimit(int_min, int_min,
                     int_max, int_max);
}

Cursor::Cursor(Vector2 lim_min, Vector2 lim_max)
{
    Cursor::setMoveLimit(lim_min, lim_max);
}

Cursor::Cursor( int lim_min_x, int lim_min_y, 
                int lim_max_x, int lim_max_y)
{
    Cursor::setMoveLimit( lim_min_x, lim_min_y, 
                      lim_max_x, lim_max_y);
}

Vector2 Cursor::getCursorPos() const
{
    return Vector2(pos.x / 2.0f, pos.y);
}

void Cursor::setMoveLimit(Vector2& lim_min, Vector2& lim_max)
{

    this->lim_min = lim_min;
    this->lim_max = lim_max;
}

void Cursor::setMoveLimit( int lim_min_x, int lim_min_y, 
                       int lim_max_x, int lim_max_y)
{
    lim_min = Vector2(lim_min_x, lim_min_y);
    lim_max = Vector2(lim_max_x, lim_max_y);
}

void Cursor::move(Vector2 direction)
{
    direction.x *= 2;
    pos += direction;

    pos.x = std::clamp(pos.x, lim_min.x, lim_max.x);
    pos.y = std::clamp(pos.y, lim_min.y, lim_max.y);
}

void Cursor::moveDirect(Vector2& direct_pos)
{
    direct_pos.x = direct_pos.x * 2;

    pos.x = std::clamp(direct_pos.x, lim_min.x, lim_max.x);
    pos.y = std::clamp(direct_pos.y, lim_min.y, lim_max.y);
}

void Cursor::setStr(std::string str)
{
    this->str = str;
}


void Cursor::print() const
{
    Cursor::print(Vector2::ZERO);
}

void Cursor::print(Vector2 offset) const
{
    ColorPair::NcPairID p = static_cast<ColorPair::NcPairID>(ReservedPairNumber::CURSOR);
    attr_set(A_NORMAL, p, NULL);
    mvprintw(pos.y + offset.y, pos.x + offset.x, "%s", str.c_str());
    attr_set(A_NORMAL, 0, NULL);
}
