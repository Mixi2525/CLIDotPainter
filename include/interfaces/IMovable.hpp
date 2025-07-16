#pragma once

#include "core/Vector2.hpp"

class IMovable
{
public:
    virtual void setMoveLimit(Vector2& lim_min, Vector2& lim_max) = 0;
    virtual void setMoveLimit( int lim_min_x, int lim_min_y, 
                   int lim_max_x, int lim_max_y) = 0;

    virtual void move(Vector2 direction) = 0;
    virtual void moveDirect(Vector2& pos) = 0;
};
