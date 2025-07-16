#pragma once

#include <string>

#include "core/Vector2.hpp"
#include "interfaces/IPrintable.hpp"
#include "interfaces/IMovable.hpp"

class Cursor : public IPrintable, public IMovable
{
    Vector2 pos;
    std::string str = "><";
    Vector2 lim_min, lim_max;
public:

    Cursor();
    Cursor(Vector2 lim_min, Vector2 lim_max);
    Cursor( int lim_min_x, int lim_min_y, 
            int lim_max_x, int lim_max_y);

    Vector2 getCursorPos() const;

    // Movable
    void setMoveLimit(Vector2& lim_min, Vector2& lim_max) override;
    void setMoveLimit( int lim_min_x, int lim_min_y, 
                   int lim_max_x, int lim_max_y) override;

    void move(Vector2 direction) override;
    void moveDirect(Vector2& direct_pos) override;

    // Pointer string
    void setStr(std::string str);

    // Printable
    void print() const override;
    void print(Vector2) const override;
};
