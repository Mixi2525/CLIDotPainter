#pragma once

#include "core/Vector2.hpp"

class CanvasData
{
public:
    Vector2 size;
    Vector2 pos;
    Vector2 lim_min, lim_max;
    Vector2 lim_area_min, lim_area_max;

    CanvasData() {};

    CanvasData( Vector2 size, Vector2 pos, 
                Vector2 lim_min, Vector2 lim_max,
                Vector2 lim_area_min, Vector2 lim_area_max)
              : size(size), pos(pos), 
                lim_min(lim_min), lim_max(lim_max), 
                lim_area_min(lim_area_min), lim_area_max(lim_area_max)
                {};

};
