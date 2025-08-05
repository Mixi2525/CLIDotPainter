#pragma once

#include <ncurses.h>

class KeyConstants
{
    // 同一の操作に割り当て可能なキーの最大数
    static constexpr int MAX_REGISTABLE_KEY = 2;

public:
    static bool isKeyInput(int key, const int keyBind[2])
    {
        return keyBind[0] == key || keyBind[1] == key;
    }

    // カーソル移動
    static constexpr int MOVE_CURSOR_LEFT_KEY[MAX_REGISTABLE_KEY] = {'h', KEY_LEFT};
    static constexpr int MOVE_CURSOR_DOWN_KEY[MAX_REGISTABLE_KEY] = {'j', KEY_DOWN};
    static constexpr int MOVE_CURSOR_UP_KEY[MAX_REGISTABLE_KEY] = {'k', KEY_UP};
    static constexpr int MOVE_CURSOR_RIGHT[MAX_REGISTABLE_KEY] = {'l', KEY_RIGHT};

    // 塗り
    static constexpr int DRAW_KEY[MAX_REGISTABLE_KEY] = {32, };

    // アプリ終了
    static constexpr int PROGRAM_QUIT_KEY[MAX_REGISTABLE_KEY] = {'q', };

    // 描画色RGB変更
    static constexpr int INCREASE_CURRENTCOLOR_R_KEY[MAX_REGISTABLE_KEY] = {'a'};
    static constexpr int INCREASE_CURRENTCOLOR_G_KEY[MAX_REGISTABLE_KEY] = {'s'};
    static constexpr int INCREASE_CURRENTCOLOR_B_KEY[MAX_REGISTABLE_KEY] = {'d'};
    
    static constexpr int DECREASE_CURRENTCOLOR_R_KEY[MAX_REGISTABLE_KEY] = {'z'};
    static constexpr int DECREASE_CURRENTCOLOR_G_KEY[MAX_REGISTABLE_KEY] = {'x'};
    static constexpr int DECREASE_CURRENTCOLOR_B_KEY[MAX_REGISTABLE_KEY] = {'c'};
};
