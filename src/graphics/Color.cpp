#include <ncurses.h>
#include <cstdint>

#include "graphics/Color.hpp"
#include "graphics/ColorConstants.hpp"

std::map<Color::NcColorID, Color::RGB> Color::allocatedColors;
std::map<Color::RGB, Color::NcColorID> Color::rgbToColorNumber;
std::set<Color::NcColorID> Color::availableColorNumbers;
bool Color::isInitialized = false;



bool Color::initNcursesColor(Color::NcColorID colorNumber, uint8_t r, uint8_t g, uint8_t b)
{
    short nc_r = r * 1000 / 255;
    short nc_g = g * 1000 / 255;
    short nc_b = b * 1000 / 255;
    auto res = init_extended_color(colorNumber, nc_r, nc_g, nc_b); // Ncursesで色登録
    return res == OK;
}

void Color::Initialize()
{
    allocatedColors.clear();
    rgbToColorNumber.clear();
    availableColorNumbers.clear();
    // 利用可能な空いている色を初期化登録
    for (Color::NcColorID i = COLOR_START_USER; i < COLOR_MAX; i++)
    {
        availableColorNumbers.insert(i);
    }
    isInitialized = true;
}

/* allocating processes */
Color::NcColorID Color::allocateNcursesColor(uint8_t r, uint8_t g, uint8_t b)
{
    if (!isInitialized) return 0; // error
    RGB newRGB = {r, g, b};

    auto rgb2cnIter = rgbToColorNumber.find(newRGB);
    if (rgb2cnIter != rgbToColorNumber.end())
    {
        return rgb2cnIter->second;
    }

    if (availableColorNumbers.empty())
    {
        return 0;// error
    }

    // ユーザが使用できてかつまだ使われてない一番小さい色番号を取得
    Color::NcColorID insertIndex = *availableColorNumbers.begin();
    if (!initNcursesColor(insertIndex, r, g, b))
    {
        return 0; // error
    }

    // クラス変数に反映
    allocatedColors.insert({insertIndex, newRGB});
    rgbToColorNumber.insert({newRGB, insertIndex});

    // 空きリストから色番号を削除
    availableColorNumbers.erase(insertIndex);

    return insertIndex;
}

/* deleting process */
void Color::deleteNcursesColor(Color::NcColorID targetIndex)
{
    if (!isInitialized) return;
    if (targetIndex == 0)
    {
        return;
    }

    auto targetIter = availableColorNumbers.find(targetIndex);

    // 利用可能な色の中に存在する ＝ まだ取得すらされていないため削除しない
    if (targetIter != availableColorNumbers.end())
    {
        return;
    }

    // 黒色に設定して初期化
    init_extended_color(targetIndex, 0, 0, 0);

    // 登録を削除
    // 色番号に対応するRGBがなかったときの念の為の処理
    if (allocatedColors.count(targetIndex) == 0)
    {
        return;
    }

    RGB rgb = allocatedColors.at(targetIndex);

    rgbToColorNumber.erase(rgb);
    allocatedColors.erase(targetIndex);

    // 空き配列に追加
    availableColorNumbers.insert(targetIndex);
}

Color::RGB Color::getRGBFromColor(Color::NcColorID color)
{
    auto targetIter = availableColorNumbers.find(color);

    // endまでいってない ＝ 利用可能な色のなかにある ＝ まだ使われてない色番号
    if (targetIter != availableColorNumbers.end())
    {
        return RGB();
    }

    // 色番号に対応するRGBがなかったときの念の為の処理
    if (allocatedColors.count(color) == 0)
    {
        return RGB();
    }

    return allocatedColors.at(color);
}

void Color::setBackgroundColor(uint8_t r, uint8_t g, uint8_t b)
{
    NcColorID bg = static_cast<NcColorID>(ReservedColorNumber::BACKGROUND);
    if (!initNcursesColor(bg, r, g, b))
    {
        return; // error
    }
}

bool Color::setCursorColor(uint8_t fgr, uint8_t fgg, uint8_t fgb, uint8_t bgr, uint8_t bgg, uint8_t bgb)
{
    NcColorID curfg = static_cast<NcColorID>(ReservedColorNumber::CURSOR_FG);
    NcColorID curbg = static_cast<NcColorID>(ReservedColorNumber::CURSOR_BG);
    if(!initNcursesColor(curfg, fgr, fgg, fgb))
    {
        return false;
    }
    if (!initNcursesColor(curbg, bgr, bgg, bgb))
    {
        return false;
    }
    return true;
}
