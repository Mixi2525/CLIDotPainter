#pragma once

#include <cstdint>
#include <list>
#include <optional>
#include <functional>

#include "core/Vector2.hpp"
#include "interfaces/IMovable.hpp"
#include "interfaces/IPrintable.hpp"
#include "ui/Layer.hpp"

class SidePanel : public IPrintable
{
private:
    bool isVisible = true;
    Vector2 correctWindowSize;
    Vector2 printAreaLimitMin;
    Vector2 printAreaLimitMax;
    uint16_t panelWidth;
    std::optional<std::reference_wrapper<std::list<Layer>>> layersRef;
    std::optional<std::list<Layer>::iterator> currentLayerIter;

public:

    SidePanel() {};
    SidePanel(Vector2 windowSize, Vector2 printAreaLimitMin, Vector2 printAreaLimitMax);
    ~SidePanel();

    void print() const override;
    void print(Vector2 offset) const override;

    void update(Vector2 windowSize, 
            Vector2 printAreaLimitMin, 
            Vector2 printAreaLimitMax, 
            std::list<Layer>& layers, 
            std::list<Layer>::iterator it);


    void setVisible(bool isVisible);
    bool getVisible();

};
