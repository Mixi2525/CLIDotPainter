#include <cstdint>
#include <cstdlib>
#include <ncurses.h>
#include <algorithm>
#include <string>
#include <vector>

#include "core/CanvasData.hpp"
#include "core/Vector2.hpp"
#include "graphics/ColorPair.hpp"
#include "ui/Layer.hpp"

Layer::Layer()
{

}

Layer::Layer(CanvasData cd, std::string layerName)
{
    size = cd.size;
    pos = cd.pos;

    lim_min = cd.lim_min;
    lim_max = cd.lim_max;

    lim_area_min = cd.lim_area_min;
    lim_area_max = cd.lim_area_max;

    canvasData = cd;

    name = layerName;

    pixels = std::vector<std::vector<uint16_t>>
        (cd.size.x, 
         std::vector<uint16_t>(cd.size.y, 0));
}


void Layer::setName(const std::string& name)
{
    this->name = name;
}

std::string Layer::getName() const
{
    return this->name;
}

void Layer::setPixelText(std::string str)
{
    pixelText = str;
}


void Layer::syncCanvasData(const CanvasData& c)
{
    pos = c.pos;

    lim_min = c.lim_min;
    lim_max = c.lim_max;

    lim_area_min = c.lim_area_min;
    lim_area_max = c.lim_area_max;
}

void Layer::setpx(int y, int x, ColorPair::NcPairID pair)
{
    if (0 <= y && y < size.y &&
        0 <= x && x < size.x)
        pixels[y][x] = pair;
}

void Layer::print() const
{
    Layer::print(Vector2::ZERO);
}

void Layer::print(Vector2 offset) const
{
    int offset_x = offset.x;
    int offset_y = offset.y;

    int pair;

    // beginとendは実際のウィンドウ上での絶対位置の制限範囲を示す
    // より値の大きい方が右と上の制限範囲になる
    Vector2 begin(std::max(pos.x, lim_area_min.x),
                  std::max(pos.y, lim_area_min.y));

    // より値の小さい方が左と下の制限範囲になる
    Vector2 end(std::min(pos.x + 2 * size.x, lim_area_max.x),
                std::min(pos.y +size.y, lim_area_max.y));

    Vector2 start(pos.x - begin.x, pos.y - begin.y);

    start.x = (start.x < 0) ? abs(start.x) : 0;
    start.y = (start.y < 0) ? abs(start.y) : 0;

    for (int y = start.y; y < size.y && y + pos.y < end.y; y++)
    {
        for (int x = start.x; x < size.x && x + pos.x < end.x; x++)
        {
            pair = pixels[y][x];
            if (pair == 0)
                continue;

            attr_set(A_NORMAL, pixels[y][x], NULL);
            mvprintw(y + pos.y, 2 * x + pos.x, "%s", pixelText.c_str());
            // mvprintw(y + pos.y, 2 * x + pos.x, "%2d", pair);
        }
    }
    attr_set(A_NORMAL, 0, NULL);
}


// debug
void Layer::showValue() const
{
    mvprintw(0, 0, "size: %d, %d", size.x, size.y);
    mvprintw(1, 0, "pos: %d, %d", pos.x, pos.y);
    mvprintw(2, 0, "areaLimit(min|max): %d, %d | %d, %d", lim_area_min.x, lim_area_min.y, lim_area_max.x, lim_area_max.y);
    mvprintw(3, 0, "pixels[0][0]: %d", pixels[0][0]);
}
