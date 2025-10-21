#include "core/ApplicationContext.hpp"
#include "core/Vector2.hpp"
#include "graphics/Color.hpp"
#include "ui/Canvas.hpp"
#include "ui/Cursor.hpp"
#include <tuple>

ApplicationContext::ApplicationContext() {}

ApplicationContext::ApplicationContext(Canvas* canvas, Cursor* cursor)
    : canvas(canvas), cursor(cursor)
{
}

void ApplicationContext::updateCorrectWindowSize()
{
    // このソフトの座標は半角2文字で1マスだから、window.xは偶数に直してから1マス分引く
    correctWindowSize.x = windowSize.x - (windowSize.x % 2) - 2;
    correctWindowSize.y = windowSize.y - 1;
}

bool ApplicationContext::getIsApplicationClose() const
{
    return isApplicationClose;
}

void ApplicationContext::setIsApplicationClose(bool state)
{
    isApplicationClose = state;
}

Color::RGB& ApplicationContext::getCurrentRgb()
{
    return currentRgb;
}

void ApplicationContext::setCurrentRgb(Color::RGB c)
{
    currentRgb = Color::RGB(c);
}

void ApplicationContext::setCurrentRgb(int r, int g, int b)
{
    currentRgb = Color::RGB(r, g, b);
}

Canvas& ApplicationContext::getCanvas() const 
{
    return *canvas;
}

Cursor& ApplicationContext::getCursor() const
{
    return *cursor;
}

void ApplicationContext::updateWindowSize(Vector2 area)
{
    windowSize = area;
    updateCorrectWindowSize();
}

void ApplicationContext::updateWindowSize(float x, float y)
{
    windowSize = Vector2(x, y);
    updateCorrectWindowSize();
}

Vector2 ApplicationContext::getWindowSize() const
{
    return windowSize;
}

Vector2 ApplicationContext::getCorrectWindowSize() const
{ 
    return correctWindowSize;
}

void ApplicationContext::updateMoveLimit(Vector2 marginLeftUp, Vector2 marginRightBottom)
{
    moveLimitMin = marginLeftUp;
    moveLimitMax = correctWindowSize - marginRightBottom;
}

void ApplicationContext::updateMoveLimit(float marginLeft, float marginUp,
                     float marginRight, float marginBottom)
{
    moveLimitMin = Vector2(marginUp, marginLeft);
    moveLimitMax = correctWindowSize - Vector2(marginBottom, marginRight);
}

std::tuple<Vector2, Vector2> ApplicationContext::getMoveLimit() const
{
    return std::make_tuple(moveLimitMin, moveLimitMax);
}


void ApplicationContext::updatePrintAreaLimit(Vector2 marginLeftUp, Vector2 marginRightBottom)
{
    printAreaLimitMin = marginLeftUp;
    printAreaLimitMax = correctWindowSize - marginRightBottom;
}

void ApplicationContext::updatePrintAreaLimit(float marginLeft, float marginUp,
                     float marginRight, float marginBottom)
{
    printAreaLimitMin = Vector2(marginLeft, marginUp);
    printAreaLimitMax = Vector2(marginRight, marginBottom);
}

std::tuple<Vector2, Vector2> ApplicationContext::getPrintAreaLimit() const
{
    return std::make_tuple(printAreaLimitMin, printAreaLimitMax);
}

