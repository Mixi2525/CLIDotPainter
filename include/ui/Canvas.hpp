#pragma once

#include <list>
#include <string>
#include <tuple>

#include "core/CanvasData.hpp"
#include "core/Vector2.hpp"
#include "graphics/ColorPair.hpp"
#include "interfaces/IPrintable.hpp"
#include "interfaces/IMovable.hpp"
#include "ui/Layer.hpp"
#include "ui/LayerController.hpp"

class Canvas : public IPrintable, public IMovable
{
    Vector2 size;
    Vector2 pos;
    Vector2 lim_min, lim_max;
    Vector2 lim_area_min, lim_area_max;
    std::list<Layer>::iterator currentLayerIter;
    Layer backgroundLayer;

    LayerController layerController;

    void clampToWindowSize(Vector2& v);
    CanvasData makeCurrentCanvasData();
    void syncData();

    void printBackgroundLayer();
public:
    std::list<Layer> layers;

    Canvas(int w, int h);

    std::list<Layer>::iterator getCurrentLayerIter();
    Vector2 getCanvasSize();

    // Movable
    void setMoveLimit(Vector2& lim_min, Vector2& lim_max) override;
    void setMoveLimit( int lim_min_x, int lim_min_y, 
                       int lim_max_x, int lim_max_y) override;

    void move(Vector2 direction) override;
    void moveDirect(Vector2& direct_pos) override;

    // Printable Area
    void setPrintAreaLimit(const Vector2& lim_min, const Vector2& lim_max);
    void setPrintAreaLimit( int lim_min_x, int lim_min_y, 
                            int lim_max_x, int lim_max_y);

    std::tuple<Vector2, Vector2> getPrintAreaLimitMinMax();

    // LayerControl
    void addLayer(std::string name);
    void addLayer(std::string name, int index);
    void moveLayer(std::string name, int insertIndex);
    void moveLayer(int index, int insertIndex);
    void deleteLayer(std::string name);
    void deleteLayer(int index);

    ColorPair::NcPairID getTopVisibleColorPair(int y, int x);
    ColorPair::NcPairID getTopVisibleColorPair(Vector2 pos);

    // Printable
    void print() const override;
    void print(Vector2 offset) const override;

    void printBackgroundLayerInfo() const;

    void printLayersName() const;
};


