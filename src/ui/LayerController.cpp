#include <algorithm>
#include <cstddef>
#include <iterator>
#include <ncurses.h>
#include <optional>

#include "core/CanvasData.hpp"
#include "ui/LayerController.hpp"
#include "ui/Layer.hpp"

std::optional<std::list<Layer>::iterator> LayerController::findLayerIteratorByIndex(int index)
{
    if (index < 0 || layers.size() <= index)
    {
        return std::nullopt;
    }

    auto it = layers.begin();
    std::advance(it, index); // インデックス分だけ進める
    return it;
}


std::optional<std::list<Layer>::iterator> LayerController::findLayerIteratorByName(std::string name)
{
    auto it = std::find_if( layers.begin(), 
                  layers.end(), 
                  [&](const Layer& l){ return l.getName() == name; });

    if (it != layers.end())
        return it;
    else
        return std::nullopt;
}

void LayerController::addLayer(const CanvasData& data, std::string name)
{
    layers.emplace_back(data, name);

    // 追加したレイヤーを指し示す
    currentLayerIter = std::prev(layers.end());
}


void LayerController::addLayer(const CanvasData& data, std::string name, int index)
{
    auto it = layers.begin();
    std::advance(it, index);

    // 追加したレイヤーを指し示す
    currentLayerIter = layers.insert(it, Layer(data, name));
}

void LayerController::moveLayer(std::string name, int insertIndex)
{
    auto optTargetIter = findLayerIteratorByName(name);

    if (!optTargetIter.has_value())
    {
        mvprintw(0, 0, "not exist layer: %s", name.c_str());
        return;
    }

    auto targetIter = optTargetIter.value();


    insertIndex = std::clamp(insertIndex, 0, static_cast<int>(layers.size()));
    auto optInsIter = findLayerIteratorByIndex(insertIndex);

    if (!optInsIter.has_value())
    {
        return;
    }

    auto insIter = optInsIter.value();

    layers.splice(insIter, layers, targetIter);
    currentLayerIter = targetIter;
    return;
}

void LayerController::moveLayer(int index, int insertIndex)
{
    auto optTargetIter = findLayerIteratorByIndex(index);

    if (!optTargetIter.has_value())
    {
        mvprintw(0, 0, "not exist layer: %d", index);
        return;
    }

    auto targetIter = optTargetIter.value();

    insertIndex = std::clamp(insertIndex, 0, static_cast<int>(layers.size()));
    auto optInsIter = findLayerIteratorByIndex(insertIndex);
    if (!optInsIter.has_value())
    {
        return;
    }

    auto insIter = optInsIter.value();

    layers.splice(insIter, layers, targetIter);
    currentLayerIter = targetIter;
    return;
}

void LayerController::deleteLayer(std::string name)
{
    auto optTargetIter = findLayerIteratorByName(name);
    if (!optTargetIter.has_value())
    {
        mvprintw(0, 0, "not exist layer: %s", name.c_str());
        return;
    }
    auto targetIter = optTargetIter.value();

    currentLayerIter = layers.erase(targetIter);
    
    if (currentLayerIter == layers.end())
    {
        currentLayerIter = std::prev(layers.end());
    }
    return;
}

void LayerController::deleteLayer(int index)
{
    auto optTargetIter = findLayerIteratorByIndex(index);
    if (!optTargetIter.has_value())
    {
        mvprintw(0, 0, "not exist layer: %d", index);
        return;
    }
    auto targetIter = optTargetIter.value();

    currentLayerIter = layers.erase(targetIter);
    
    if (currentLayerIter == layers.end())
    {
        currentLayerIter = std::prev(layers.end());
    }
    return;
}

const std::list<Layer>& 
LayerController::getLayers() const
{
    return layers;
}

const std::list<Layer>::iterator& 
LayerController::getCurrentLayerIter() const
{
    return currentLayerIter;
}
