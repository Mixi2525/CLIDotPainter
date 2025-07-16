#pragma once

#include <cstdint>
#include <ncurses.h>
#include <map>
#include <set>

class Color
{
public:
    using NcColorID = uint8_t;
    struct RGB
    {
        int r = -1;
        int g = -1;
        int b = -1;

        RGB() {};
        RGB(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {};

        bool operator < (const RGB& other) const
        {
            if (r != other.r) return r < other.r;
            if (g != other.g) return g < other.g;
            return b < other.b;
        }

        bool operator == (const RGB& other) const
        {
            return r == other.r &&
                   g == other.g &&
                   b == other.b;
        }

        RGB getNcursesColorScaleRGB()
        {
            float scale = (1.0 / 255.0) * 1000;
            return RGB(r * scale, g * scale, b * scale);
        }
    };

    // 環境ごとに最大値が違う可能性があるので、MAX_COLOR コンパイルor実行時に可変にできたらいいかも
    static NcColorID const COLOR_MAX = 255;

    // ユーザが定義できる色番号の開始位置
    static NcColorID const COLOR_START_USER = 10;

private:
    static std::map<NcColorID, RGB> allocatedColors;
    static std::map<RGB, NcColorID> rgbToColorNumber;
    static std::set<NcColorID> availableColorNumbers;
    static bool isInitialized;


    static bool initNcursesColor(NcColorID colorNumber, uint8_t r, uint8_t g, uint8_t b);

public:

    static void Initialize();
    static NcColorID allocateNcursesColor(uint8_t r, uint8_t g, uint8_t b);
    static void deleteNcursesColor(NcColorID targetIndex);

    static RGB getRGBFromColor(NcColorID color);

    static void setBackgroundColor(uint8_t r, uint8_t g, uint8_t b);
    static bool setCursorColor(uint8_t fgr, uint8_t fgg, uint8_t fgb, uint8_t bgr, uint8_t bgg, uint8_t bgb);
};
