#include <ncurses.h>
#include <algorithm>
#include <tuple>

#include "core/CanvasData.hpp"
#include "core/Vector2.hpp"
#include "graphics/Color.hpp"
#include "graphics/ColorConstants.hpp"
#include "graphics/ColorPair.hpp"
#include "ui/Canvas.hpp"
#include "ui/Layer.hpp"
#include "ui/LayerController.hpp"

Canvas::Canvas(int w, int h) : size(w, h), layerController(layers, currentLayerIter)
{

    // 背景も独立したレイヤーとして扱う
    backgroundLayer = Layer(makeCurrentCanvasData());
    backgroundLayer.setPixelText("▀▄");
    backgroundLayer.setName("Background Layer");

    Color::setBackgroundColor(100, 100, 100);
    ColorPair::setBackgroundPairColor();

    ColorPair::NcPairID pair = static_cast<ColorPair::NcPairID>(ReservedPairNumber::BACKGROUND);
    // 背景用の色を設定
    for (auto& col : backgroundLayer.pixels)
    {
        for (auto& px : col)
        {
            px = pair;
        }
    }
}


void Canvas::clampToWindowSize(Vector2& v)
{
    int width, height;
    getmaxyx(stdscr, height, width);
    v.x = std::clamp(v.x, 0, width);
    v.y = std::clamp(v.y, 0, height);
}

CanvasData Canvas::makeCurrentCanvasData()
{
    return CanvasData( size, pos, 
                       lim_min, lim_max,
                       lim_area_min, lim_area_max);
}

void Canvas::syncData()
{
    const CanvasData data = makeCurrentCanvasData();
    // キャンバスデータをレイヤーデータに反映
    
    backgroundLayer.syncCanvasData(data);

    for (Layer& l : layers)
    {
        l.syncCanvasData(data);
    }
}



std::list<Layer>::iterator Canvas::getCurrentLayerIter()
{
    return currentLayerIter;
}

Vector2 Canvas::getCanvasSize()
{
    return size;
}


Vector2 Canvas::getPos()
{
    return pos;
}

void Canvas::setMoveLimit(Vector2& lim_min, Vector2& lim_max)
{
    this->lim_min = lim_min;
    this->lim_max = lim_max;

    syncData();
}

void Canvas::setMoveLimit( int lim_min_x, int lim_min_y, 
                          int lim_max_x, int lim_max_y)
{
    lim_min = Vector2(lim_min_x, lim_min_y);
    lim_max = Vector2(lim_max_x, lim_max_y);

    syncData();
}


void Canvas::move(Vector2 direction)
{
    direction.x *= 2;
    pos += direction;

    pos.x = std::clamp(pos.x, lim_min.x, lim_max.x);
    pos.y = std::clamp(pos.y, lim_min.y, lim_max.y);

    syncData();
}

void Canvas::moveDirect(Vector2& direct_pos)
{
    direct_pos.x = direct_pos.x * 2;

    pos.x = std::clamp(direct_pos.x, lim_min.x, lim_max.x);
    pos.y = std::clamp(direct_pos.y, lim_min.y, lim_max.y);
    syncData();
}

void Canvas::setPrintAreaLimit(const Vector2& lim_min, const Vector2& lim_max)
{
    lim_area_min = lim_min;
    lim_area_max = lim_max;
    clampToWindowSize(lim_area_min);
    clampToWindowSize(lim_area_max);

    syncData();
}

void Canvas::setPrintAreaLimit( int lim_min_x, int lim_min_y, 
                                int lim_max_x, int lim_max_y)
{
    lim_area_min = Vector2(lim_min_x, lim_min_y);
    lim_area_max = Vector2(lim_max_x, lim_max_y);
    clampToWindowSize(lim_area_min);
    clampToWindowSize(lim_area_max);

    syncData();
}

std::tuple<Vector2, Vector2> Canvas::getPrintAreaLimitMinMax()
{
    return std::make_tuple(lim_area_min, lim_area_max);
}

// Layer Control
void Canvas::addLayer(std::string name)
{
    CanvasData data = makeCurrentCanvasData();
    layerController.addLayer(data, name);
}

void Canvas::addLayer(std::string name, int index)
{
    CanvasData data = makeCurrentCanvasData();
    layerController.addLayer(data, name, index);
}

void Canvas::moveLayer(std::string name, int insertIndex)
{
    layerController.moveLayer(name, insertIndex);
}

void Canvas::moveLayer(int index, int insertIndex)
{
    layerController.moveLayer(index, insertIndex);
}

void Canvas::deleteLayer(std::string name)
{
    layerController.deleteLayer(name);
}

void Canvas::deleteLayer(int index)
{
    layerController.deleteLayer(index);
}


ColorPair::NcPairID Canvas::getTopVisibleColorPair(int y, int x)
{
    if (0 <= y && y < size.y &&
        0 <= x && x < size.x)
    {
        for (auto it = layers.rbegin(); it != layers.rend(); it++)
        {
            ColorPair::NcPairID p = it->pixels[y][x];

            if (ColorPair::COLOR_PAIR_START_USER <= p && 
                p <= ColorPair::COLOR_PAIR_MAX)
            {
                return p;
            }
        }
    }
    return 0;
}

ColorPair::NcPairID Canvas::getTopVisibleColorPair(Vector2 pos)
{
    return getTopVisibleColorPair(pos.y, pos.x);
}

// Printable
void Canvas::print() const
{
    print(Vector2::ZERO);
}

void Canvas::print(Vector2 offset) const
{
    backgroundLayer.print();

    for (Layer l : layers)
    {
        l.print();
    }
}

void Canvas::printBackgroundLayerInfo() const
{
    backgroundLayer.showValue();
};



void Canvas::printLayersName() const
{
    int i = 0;
    for(auto it = layers.begin(); it != layers.end(); it++)
    {
        char currentSymbol = (currentLayerIter == it) ? '*' : ' ';
        mvprintw(i, 0, "[%c]%s", currentSymbol, (*it).getName().c_str());
        i++;
    }
}
