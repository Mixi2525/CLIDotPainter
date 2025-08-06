#pragma once

#include <ncurses.h>
#include <optional>

class KeyConstants
{
public:
    // 同一の操作に割り当て可能なキーの最大数
    static constexpr int MAX_REGISTABLE_KEY = 2;

    // カーソル移動
    static constexpr std::optional<int> MOVE_CURSOR_LEFT_KEY[MAX_REGISTABLE_KEY] = {'h', KEY_LEFT};
    static constexpr std::optional<int> MOVE_CURSOR_DOWN_KEY[MAX_REGISTABLE_KEY] = {'j', KEY_DOWN};
    static constexpr std::optional<int> MOVE_CURSOR_UP_KEY[MAX_REGISTABLE_KEY] = {'k', KEY_UP};
    static constexpr std::optional<int> MOVE_CURSOR_RIGHT_KEY[MAX_REGISTABLE_KEY] = {'l', KEY_RIGHT};

    // 塗り
    static constexpr std::optional<int> DRAW_KEY[MAX_REGISTABLE_KEY] = {32, };

    // アプリ終了
    static constexpr std::optional<int> PROGRAM_QUIT_KEY[MAX_REGISTABLE_KEY] = {'q', };

    // デバッグ実行
    static constexpr std::optional<int> EXECUTE_DEBUG_PROCESS_KEY[MAX_REGISTABLE_KEY] = {'d', };

    // 描画色RGB変更
    static constexpr std::optional<int> INCREASE_CURRENTCOLOR_R_KEY[MAX_REGISTABLE_KEY] = {'a', };
    static constexpr std::optional<int> INCREASE_CURRENTCOLOR_G_KEY[MAX_REGISTABLE_KEY] = {'s', };
    static constexpr std::optional<int> INCREASE_CURRENTCOLOR_B_KEY[MAX_REGISTABLE_KEY] = {'d', };
    
    static constexpr std::optional<int> DECREASE_CURRENTCOLOR_R_KEY[MAX_REGISTABLE_KEY] = {'z', };
    static constexpr std::optional<int> DECREASE_CURRENTCOLOR_G_KEY[MAX_REGISTABLE_KEY] = {'x', };
    static constexpr std::optional<int> DECREASE_CURRENTCOLOR_B_KEY[MAX_REGISTABLE_KEY] = {'c', };
};
