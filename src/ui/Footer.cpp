#include <ncurses.h>
#include "ui/Footer.hpp"
#include "core/Vector2.hpp"
#include "graphics/Color.hpp"
#include "ui/FooterData.hpp"
#include "graphics/ColorPair.hpp"
#include "graphics/ColorConstants.hpp"


Footer::Footer()
{
    initNcursesCurrentColor(Color::RGB(0, 0, 0));
}

Footer::Footer(FooterData data, int windowSizeY)
{
    setData(data, windowSizeY);
    initNcursesCurrentColor(data.currentRgb);

}

void Footer::print() const
{
    print(Vector2::ZERO);
}


void Footer::print(Vector2 offset) const
{
    int posY = windowSizeY - 1;
    attr_set(A_NORMAL, static_cast<ColorPair::NcPairID>(ReservedPairNumber::CURRENTCOLOR), NULL);
    mvprintw(posY, 0, "  ");

    attr_set(A_NORMAL, 0, NULL);

    mvprintw(posY, 3, "(%d, %d, %d)", 
             data.currentRgb.r, 
             data.currentRgb.g,
             data.currentRgb.b);
    
    mvprintw(windowSizeY - 1, 20, "%d,%d", data.cursorPos.x, data.cursorPos.y);
}

void Footer::update(FooterData data, int windowSizeY)
{
    setData(data, windowSizeY);
    updateNcursesCurrentColor(data.currentRgb);
    

}

void Footer::initNcursesCurrentColor(Color::RGB rgb)
{
    ColorPair::NcPairID p = static_cast<ColorPair::NcPairID>(ReservedPairNumber::CURRENTCOLOR);
    Color::NcColorID c = static_cast<Color::NcColorID>(ReservedColorNumber::CURRENTCOLOR);

    Color::initNcursesColor(c, rgb.r, rgb.g, rgb.b);
    ColorPair::initNcursesColorPair(p, c, c);
}

void Footer::updateNcursesCurrentColor(Color::RGB rgb)
{
    Color::NcColorID c = static_cast<Color::NcColorID>(ReservedColorNumber::CURRENTCOLOR);

    Color::initNcursesColor(c, rgb.r, rgb.g, rgb.b);
}

void Footer::setData(FooterData data, int windowSizeY)
{
    this->data = data;
    this->windowSizeY = windowSizeY;
}
