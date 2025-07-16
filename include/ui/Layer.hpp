#pragma once

#include <string>
#include <vector>

#include "core/CanvasData.hpp"
#include "core/Vector2.hpp"
#include "graphics/ColorPair.hpp"
#include "interfaces/IPrintable.hpp"

class Layer : public IPrintable
{
    std::string name = "layer";
    Vector2 size;
    Vector2 pos;
    Vector2 lim_min, lim_max;
    Vector2 lim_area_min, lim_area_max;
    std::string pixelText = "  ";

    CanvasData canvasData;


public:
    // レイヤー画素データ
    std::vector<std::vector<ColorPair::NcPairID>> pixels;

    Layer();
    Layer(CanvasData cd, std::string name = "new layer");

    void setName(const std::string& name);
    std::string getName() const;

    void setPixelText(std::string str);

    void syncCanvasData(const CanvasData& c);

    void setpx(int y, int x, ColorPair::NcPairID pair);

    // Printable
    void print() const override;
    void print(Vector2 offset) const override;

    //debug
    void showValue() const;
};
