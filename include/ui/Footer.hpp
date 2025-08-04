#pragma once

#include "graphics/Color.hpp"
#include "interfaces/IPrintable.hpp"
#include "ui/FooterData.hpp"

class Footer : public IPrintable
{
private:
    FooterData data;
    int windowSizeY;

    void initNcursesCurrentColor(Color::RGB rgb);
    void updateNcursesCurrentColor(Color::RGB rgb);
    void setData(FooterData data, int windowSizeY);

public:
    Footer();
    Footer(FooterData data, int windowSizeY);

    void print() const override;
    void print(Vector2 offset) const override;

    void update(FooterData data, int windowSizeY);


};
