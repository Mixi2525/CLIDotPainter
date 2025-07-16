#include <list>
#include <ncurses.h>

#include "core/Vector2.hpp"
#include "graphics/ColorPair.hpp"
#include "ui/Layer.hpp"
#include "ui/SidePanel.hpp"

SidePanel::SidePanel(Vector2 windowSize, Vector2 printAreaLimitMin, Vector2 printAreaLimitMax): 
        correctWindowSize(windowSize), 
        printAreaLimitMin(printAreaLimitMin),
        printAreaLimitMax(printAreaLimitMax)
{

}

SidePanel::~SidePanel()
{

}

void SidePanel::print() const
{
    print(Vector2::ZERO);
}

void SidePanel::print(Vector2 offset) const
{
    if (!isVisible)
        return;

    if (!currentLayerIter.has_value())
        return;
    if (!layersRef.has_value())
        return;

    auto& layers = layersRef.value().get();
    auto currentlayerIter = currentLayerIter.value();

    auto colorPallete = ColorPair::getColorPalletes();
    auto palleteIter = colorPallete.begin();

    auto layerIter = layers.begin();

    for (int y = printAreaLimitMin.y; y <= printAreaLimitMax.y; y++)
    {
        if (palleteIter != colorPallete.end())
        {
            attr_set(A_NORMAL, *palleteIter, NULL);
            mvprintw(y, correctWindowSize.x - 1, "%3d", *palleteIter);
            attr_set(A_NORMAL, 0, NULL);
            palleteIter++;
        }

        if (layerIter != layers.end())
        {
            bool isCurrent = (&(*currentlayerIter) == &(*layerIter));
            mvprintw(y, correctWindowSize.x - 20, "%c%s", isCurrent ? '>' : ' ', layerIter->getName().c_str());
            layerIter++;
        }
    }
}

void SidePanel::update(Vector2 windowSize, Vector2 printAreaLimitMin, Vector2 printAreaLimitMax, std::list<Layer>& layers, std::list<Layer>::iterator it)
{
    this->correctWindowSize = windowSize;
    this->printAreaLimitMin = printAreaLimitMin;
    this->printAreaLimitMax = printAreaLimitMax;
    this->layersRef = layers;
    this->currentLayerIter = it;
}

void SidePanel::setVisible(bool isVisible)
{
    this->isVisible = isVisible;
}

bool SidePanel::getVisible()
{
    return isVisible;
}
