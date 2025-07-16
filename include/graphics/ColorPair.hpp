#pragma once

#include <cstdint>
#include <deque>
#include <ncurses.h>
#include <map>
#include <set>

#include "graphics/Color.hpp"

class ColorPair
{
public:
    using NcPairID = uint16_t;

    struct Pair
    {
        Color::NcColorID fg = 0;
        Color::NcColorID bg = 0;

        Pair(Color::NcColorID fg, Color::NcColorID bg) : fg(fg), bg(bg){};

        bool operator < (const Pair& other) const
        {
            int fgColorNumber = fg;
            int bgColorNumber = bg;
            int otherfgColorNumber = other.fg;
            int otherbgColorNumber = other.bg;

            if(fgColorNumber != otherfgColorNumber) return fgColorNumber < otherfgColorNumber;
            return bgColorNumber < otherbgColorNumber;
        }

        bool operator == (const Pair& other) const
        {
            int fgColorNumber = fg;
            int bgColorNumber = bg;
            int otherfgColorNumber = other.fg;
            int otherbgColorNumber = other.bg;

            return fgColorNumber == otherfgColorNumber &&
                   bgColorNumber == otherbgColorNumber;
        }
    };

private:
    static std::map<NcPairID, Pair> allocatedColorPairs;
    static std::map<Pair, NcPairID> pairToPairNumber;
    static std::set<NcPairID> availableColorPairNumbers;
    static std::deque<NcPairID> colorPalletes;
    static bool isInitialized;

    static bool initNcursesColorPair(NcPairID colorPairNumber, Color::NcColorID fg, Color::NcColorID bg);


public:
    // 環境ごとに最大値が違う可能性があるので、MAX_COLOR コンパイルor実行時に可変にできたらいいかも
    static NcPairID const COLOR_PAIR_MAX = 65535;

    // ユーザが定義できる色番号の開始位置
    static NcPairID const COLOR_PAIR_START_USER = 10;

    static void Initialize();
    static NcPairID allocateNcursesColorPair(Color::NcColorID fg, Color::NcColorID bg);
    static void deleteNcursesColorPair(NcPairID targetIndex);

    static const std::deque<NcPairID>& getColorPalletes();

    static Color::NcColorID getPairFromColorPair(NcPairID colorPair);

    static void setBackgroundPairColor();
    static void setCursorPairColor(bool isDefault = false);
};
