#pragma once

#include "core/Vector2.hpp"
#include "graphics/Color.hpp"
#include "ui/Canvas.hpp"
#include "ui/Cursor.hpp"
#include <tuple>


class ApplicationContext
{
    bool isApplicationClose{false};
    Color::RGB currentRgb{0, 0, 0};
    Canvas* canvas;
    Cursor* cursor;

    /* 範囲データ */
    Vector2 windowSize;
    Vector2 correctWindowSize;
    Vector2 moveLimitMin, moveLimitMax;
    Vector2 printAreaLimitMin, printAreaLimitMax;

    void updateCorrectWindowSize();

public:
    ApplicationContext();
    ApplicationContext(Canvas* canvas, Cursor* cursor);

    bool getIsApplicationClose() const;
    void setIsApplicationClose(bool state);

    Color::RGB& getCurrentRgb();
    void setCurrentRgb(Color::RGB c);
    void setCurrentRgb(int r, int g, int b);

    Canvas& getCanvas() const;
    Cursor& getCursor() const;

    void updateWindowSize(Vector2 area);
    void updateWindowSize(float x, float y);
    Vector2 getWindowSize() const;

    Vector2 getCorrectWindowSize() const;

    void updateMoveLimit(Vector2 marginLeftUp, Vector2 marginRightBottom);
    void updateMoveLimit(float marginLeft, float marginUp,
                         float marginRight, float marginBottom);
    std::tuple<Vector2, Vector2> getMoveLimit() const;


    void updatePrintAreaLimit(Vector2 marginLeftUp, Vector2 marginRightBottom);
    void updatePrintAreaLimit(float marginLeft, float marginUp,
                              float marginRight, float marginBottom);
    std::tuple<Vector2, Vector2> getPrintAreaLimit() const;

};
