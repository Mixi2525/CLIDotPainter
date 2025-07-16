#include <ncurses.h>
#include <algorithm>

#include "graphics/ColorPair.hpp"
#include "graphics/Color.hpp"
#include "graphics/ColorConstants.hpp"

std::map<ColorPair::NcPairID, ColorPair::Pair> ColorPair::allocatedColorPairs;
std::map<ColorPair::Pair, ColorPair::NcPairID> ColorPair::pairToPairNumber;
std::set<ColorPair::NcPairID> ColorPair::availableColorPairNumbers;
std::deque<ColorPair::NcPairID> ColorPair::colorPalletes;
bool ColorPair::isInitialized = false;


void ColorPair::Initialize()
{
    allocatedColorPairs.clear();
    pairToPairNumber.clear();
    availableColorPairNumbers.clear();
    colorPalletes.clear();
    // 利用可能な空いている色を初期化登録
    for (NcPairID i = COLOR_PAIR_START_USER; i < COLOR_PAIR_MAX; i++)
    {
        availableColorPairNumbers.insert(i);
    }

    isInitialized = true;
}


bool ColorPair::initNcursesColorPair(ColorPair::NcPairID colorPairNumber, Color::NcColorID fg, Color::NcColorID bg)
{
    int res = init_extended_pair(colorPairNumber, fg, bg);
    return res == OK;
}


ColorPair::NcPairID ColorPair::allocateNcursesColorPair(Color::NcColorID fg, Color::NcColorID bg)
{
    if (!isInitialized) return 0; // error
    Pair newPair = {fg, bg};

    auto p2pnIter = pairToPairNumber.find(newPair);
    if (p2pnIter != pairToPairNumber.end())
    {
        return p2pnIter->second;
    }

    if (availableColorPairNumbers.empty())
    {
        return 0;// error
    }

    // ユーザが使用できてかつまだ使われてない一番小さいペア番号を取得
    NcPairID insertIndex = *availableColorPairNumbers.begin();
    if (!initNcursesColorPair(insertIndex, fg, bg))
    {
        return 0; // error
    }

    // クラス変数に反映

    allocatedColorPairs.insert({insertIndex, newPair});
    pairToPairNumber.insert({newPair, insertIndex});

    // 空きリストからペア番号を削除
    availableColorPairNumbers.erase(insertIndex);

    colorPalletes.push_front(insertIndex);

    return insertIndex;
}

void ColorPair::deleteNcursesColorPair(NcPairID targetIndex)
{
    if (!isInitialized) return;
    if (targetIndex == 0)
    {
        return;
    }

    auto targetIter = availableColorPairNumbers.find(targetIndex);

    // 利用可能な色の中に存在する ＝ まだ取得すらされていないため削除しない
    if (targetIter != availableColorPairNumbers.end())
    {
        return;
    }

    // 黒色に設定して初期化
    init_extended_color(targetIndex, 0, 0, 0);

    // 登録を削除
    // 色番号に対応するRGBがなかったときの念の為の処理
    if (allocatedColorPairs.count(targetIndex) == 0)
    {
        return;
    }

    Pair p = allocatedColorPairs.at(targetIndex);

    pairToPairNumber.erase(p);
    allocatedColorPairs.erase(targetIndex);

    // 空き配列に追加
    availableColorPairNumbers.insert(targetIndex);

    auto palletIter = std::find(colorPalletes.begin(), colorPalletes.end(), targetIndex);
    if (palletIter != colorPalletes.end())
    {
        colorPalletes.erase(palletIter);
    }
    return;

}

const std::deque<ColorPair::NcPairID>& ColorPair::getColorPalletes()
{
    return colorPalletes;
}

Color::NcColorID ColorPair::getPairFromColorPair(NcPairID colorPair)
{
    if (colorPair < COLOR_PAIR_START_USER ||
        COLOR_PAIR_MAX < colorPair)
        return 0;


    auto targetIter = availableColorPairNumbers.find(colorPair);

    // 登録されていないなら取得できないので終了
    if (targetIter != availableColorPairNumbers.end())
    {
        return 0;
    }

    // 色番号に対応するRGBがなかったときの念の為の処理
    if (allocatedColorPairs.count(colorPair) == 0)
    {
        return 0;
    }

    return allocatedColorPairs.at(colorPair).bg;
}

void ColorPair::setBackgroundPairColor()
{
    // プログラム側で予約定義された背景用のペア番号と色番号を取得
    NcPairID bgp = static_cast<NcPairID>(ReservedPairNumber::BACKGROUND);
    Color::NcColorID fg = static_cast<Color::NcColorID>(ReservedColorNumber::BACKGROUND);

    // 背景黒、前景は任意の色で背景模様用の色ペアを作成
    if (!initNcursesColorPair(bgp, fg, 0))
    {
        return; // error
    }
}

void ColorPair::setCursorPairColor(bool isDefault)
{
    NcPairID curp = static_cast<NcPairID>(ReservedPairNumber::CURSOR);
    Color::NcColorID bg = static_cast<Color::NcColorID>(ReservedColorNumber::CURSOR_BG);
    Color::NcColorID fg = static_cast<Color::NcColorID>(ReservedColorNumber::CURSOR_FG);

    if (isDefault)
    {
        int defaultFg, defaultBg;
        if (assume_default_colors(defaultFg, defaultBg) == OK)
        {
            if (!initNcursesColorPair(curp, 1, defaultBg))
            {
                return;
            }
        } else {
            if (!initNcursesColorPair(curp, 0, COLOR_BLACK))
            {
                return;
            }

        }
    }
    if (!initNcursesColorPair(curp, 0, bg))
    {
        return;
    }
}
