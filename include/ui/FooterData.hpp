#pragma once

#include "core/Vector2.hpp"
#include "graphics/Color.hpp"

class FooterData
{
public:
    Vector2 cursorPos;
    Vector2 canvasPos;
    Color::RGB currentRgb;

    FooterData() {};
    FooterData(Vector2 cursorPos, 
               Vector2 canvasPos, 
               Color::RGB currentRgb)
        : cursorPos(cursorPos), 
          canvasPos(canvasPos), 
          currentRgb(currentRgb) {};
};
