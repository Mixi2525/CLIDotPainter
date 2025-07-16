#pragma once

#include <list>
#include <optional>
#include <string>

#include "core/CanvasData.hpp"
#include "ui/Layer.hpp"

class LayerController
{
    std::list<Layer>& layers;
    std::list<Layer>::iterator& currentLayerIter;

    std::optional<std::list<Layer>::iterator> findLayerIteratorByIndex(int index);
    std::optional<std::list<Layer>::iterator> findLayerIteratorByName(std::string name);



public:
    LayerController(std::list<Layer>& layers, std::list<Layer>::iterator& it) : layers(layers), currentLayerIter(it) {};

    // Layer Control
    void addLayer(const CanvasData& data, std::string name);
    void addLayer(const CanvasData& data, std::string name, int index);
    void moveLayer(std::string name, int insertIndex);
    void moveLayer(int index, int insertIndex);
    void deleteLayer(std::string name);
    void deleteLayer(int index);

    const std::list<Layer>& getLayers() const;
    const std::list<Layer>::iterator& getCurrentLayerIter() const;
};
