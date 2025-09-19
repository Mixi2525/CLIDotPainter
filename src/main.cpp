
#include <algorithm>
#include <clocale>
#include <cstdint>
#include <cctype>
#include <functional>
#include <ncurses.h>
#include <optional>
#include <random>
#include <string>
#include <tuple>
#include <unordered_map>

#include "core/KeyConstants.hpp"
#include "core/ApplicationSettings.hpp"
#include "graphics/Color.hpp"
#include "graphics/ColorPair.hpp"
#include "core/Vector2.hpp"
#include "ui/FooterData.hpp"
#include "ui/SidePanel.hpp"
#include "ui/Canvas.hpp"
#include "ui/Cursor.hpp"
#include "ui/Footer.hpp"
#include "ui/FooterData.hpp"

void waitEnter()
{
    while (getch() != 10)
        ;
}

void debugAction(Canvas& canvas, Cursor& cursor)
{
    ColorPair::NcPairID cursorColorPair = canvas.getTopVisibleColorPair(cursor.getCursorPos());
    mvprintw(0, 0, "%d", cursorColorPair);
    refresh();
    waitEnter();
    if (cursorColorPair != 0)
    {

        Color::NcColorID cursorColor = ColorPair::getPairFromColorPair(cursorColorPair);
        Color::RGB cursorRgb = Color::getRGBFromColor(cursorColor);

        
        int res = Color::setCursorColor(200, 200, 200, cursorRgb.r, cursorRgb.g, cursorRgb.b);

        mvprintw(0, 0, "cursorColor[%d]: %d, \nrgb: %d %d %d", res, cursorColor, cursorRgb.r, cursorRgb.g, cursorRgb.b);
        refresh();
        waitEnter();
        ColorPair::setCursorPairColor();
    }
    else
    {
        Color::setCursorColor(200, 200, 200, 100, 100, 100);
        ColorPair::setCursorPairColor();
    }
    cursor.print();
    refresh();


    canvas.addLayer("test1");
    canvas.addLayer("test2");
    canvas.moveLayer(1, 0);
    canvas.addLayer("test3", 2);

    canvas.printLayersName();
    refresh();
    waitEnter();

}

struct AppContext
{
    bool isClose = false;
    Color::RGB currentRgb;
    std::optional<std::reference_wrapper<Canvas>> canvas;
    std::optional<std::reference_wrapper<Cursor>> cursor;
    std::mt19937 mt{std::random_device{}()};
    std::uniform_int_distribution<int> dist{0, 255};
};

void changeColorValue(int& col, int step)
{
    col = std::clamp(col + step, 0, 255);
}

void bindKeyAction(
        std::unordered_map<int, std::function<void()>>& keyActionMap, 
        const std::optional<int> keys[2], 
        std::function<void()> action)
{
    for (int idx = 0; idx < KeyConstants::MAX_REGISTABLE_KEY; idx++)
    {
        if (keys[idx].has_value())
        { // キーが設定されている
            keyActionMap[keys[idx].value()] = action;
        }
    }
}

auto createChangeColorValueAction(AppContext& context,
        int (Color::RGB::* changeValuePtr), 
        int step)
{
    return [=, &context]() {
        changeColorValue(context.currentRgb.*changeValuePtr, step);
    };
}

void setupKeyBindings(std::unordered_map<int, std::function<void()>>& keyActionMap, AppContext& context)
{
    /* キーと関連つける処理定義 */
    // プログラム終了フラグ
    auto appQuit = [&]()
    {
        context.isClose = true;
    };

    auto draw = [&]()
    {
        if (!context.canvas || !context.cursor)
        { // 少なくともどっちかが参照を持っていない
            return;
        }

        Canvas& canvas = context.canvas->get();
        Cursor& cursor = context.cursor->get();

        Vector2 cpos = cursor.getCursorPos(); // キャンバスの正方形ピクセル座標
        Vector2 canvasSize = canvas.getCanvasSize();

        if (0 <= cpos.y && cpos.y < canvasSize.y &&
            0 <= cpos.x && cpos.x < canvasSize.x)
        {
            Color::NcColorID colnum = Color::allocateNcursesColor(context.currentRgb);
            ColorPair::NcPairID pair = ColorPair::allocateNcursesColorPair(0, colnum);
            canvas.getCurrentLayerIter()->setpx(cpos.y, cpos.x, pair);
        }
    };

    auto moveLeft = [&]() 
    {
        if (context.cursor)
            context.cursor->get().move(Vector2::LEFT);
    };
    auto moveDown = [&]()
    {
        if (context.cursor)
            context.cursor->get().move(Vector2::DOWN);
    };
    auto moveUp = [&]() 
    {
        if (context.cursor)
            context.cursor->get().move(Vector2::UP);
    };
    auto moveRight = [&]() 
    {
        if (context.cursor)
            context.cursor->get().move(Vector2::RIGHT); 
    };

    auto debug = [&]() 
    {
        if (!context.canvas || !context.cursor)
        { // 少なくともどっちかが参照を持っていない
            return;
        }

        debugAction(context.canvas->get(), context.cursor->get());
    };

    /* キーと処理を関連つける */
    bindKeyAction(keyActionMap, KeyConstants::PROGRAM_QUIT_KEY, appQuit);
    bindKeyAction(keyActionMap, KeyConstants::DRAW_KEY, draw);
    bindKeyAction(keyActionMap, KeyConstants::MOVE_CURSOR_LEFT_KEY, moveLeft);
    bindKeyAction(keyActionMap, KeyConstants::MOVE_CURSOR_DOWN_KEY, moveDown);
    bindKeyAction(keyActionMap, KeyConstants::MOVE_CURSOR_UP_KEY, moveUp);
    bindKeyAction(keyActionMap, KeyConstants::MOVE_CURSOR_RIGHT_KEY, moveRight);
    bindKeyAction(keyActionMap, KeyConstants::EXECUTE_DEBUG_PROCESS_KEY, debug);

    // 小文字と大文字のRGB色操作のキーバインド
    // 処理用変数定義
    const std::array<const std::optional<int>*, 6> 
    changeColorKeys = {
        KeyConstants::INCREASE_CURRENTCOLOR_R_KEY,
        KeyConstants::INCREASE_CURRENTCOLOR_G_KEY,
        KeyConstants::INCREASE_CURRENTCOLOR_B_KEY,
        KeyConstants::DECREASE_CURRENTCOLOR_R_KEY,
        KeyConstants::DECREASE_CURRENTCOLOR_G_KEY,
        KeyConstants::DECREASE_CURRENTCOLOR_B_KEY
    };

    const std::array<int Color::RGB::*, 3> 
    colorRgbPtrs = {
        &Color::RGB::r,
        &Color::RGB::g,
        &Color::RGB::b
    };

    const std::array<int, 2>
    steps = {
        ApplicationSettings::BaseColorChangeStepValue,
        ApplicationSettings::ShiftColorChangeStepValue
    };
    
    // 実際の登録処理
    for (int idx = 0; idx < changeColorKeys.size(); idx++)
    {
        // 3以降は値減少キーだからマイナスにする必要がある
        int8_t fact = ((idx >= 3) ? -1 : 1);

        bindKeyAction(keyActionMap, 
            changeColorKeys[idx], 
            createChangeColorValueAction(
                context, 
                colorRgbPtrs[idx % 3],
                steps[0] * fact));

        std::optional<int> shiftedKeys[KeyConstants::MAX_REGISTABLE_KEY];
        for (int keyIdx = 0; keyIdx < KeyConstants::MAX_REGISTABLE_KEY; keyIdx++)
        {
            // キーが設定されているかどうか
            if (changeColorKeys[idx][keyIdx])
                shiftedKeys[keyIdx] = std::toupper(changeColorKeys[idx][keyIdx].value());
            else
                shiftedKeys[keyIdx] = std::nullopt;
        }

        bindKeyAction(keyActionMap, 
            shiftedKeys, 
            createChangeColorValueAction(
                context, 
                colorRgbPtrs[idx % 3],
                steps[1] * fact));
    }
}

int main()
{
    Cursor cursor;
    Vector2 windowSize;
    Vector2 correctWindowSize;
    Vector2 moveLimitMin, moveLimitMax;
    Vector2 printAreaLimitMin, printAreaLimitMax;
    std::unordered_map<int, std::function<void()>> keyActionMap;
    AppContext context;

    //setting
    setlocale(LC_ALL, "");
    
    // initializing
    initscr(); // ncurse用にwindowを初期化
    cbreak(); // 改行が入力されるのを待たずにバッファから入力を受け取る
    noecho(); // 画面に入力した文字列を表示しない
    keypad(stdscr, true); // 特殊キーも入力できるようにする
    curs_set(0); // 端末のカーソルを非表示にする
    start_color(); // 色の描画を開始
    nodelay(stdscr, true); // キー入力を待たないようにする
    use_default_colors();

    /* 色管理初期化 */
    Color::Initialize();
    ColorPair::Initialize();
    
    /* 座標と描画範囲の初期化 */
    getmaxyx(stdscr, windowSize.y, windowSize.x);

    // このソフトの座標は半角2文字で1マスだから、window.xは偶数に直してから1マス分引く
    correctWindowSize.x = windowSize.x - (windowSize.x % 2) - 2;
    correctWindowSize.y = windowSize.y - 1;

    // 移動範囲決定
    moveLimitMin = Vector2(0, 0);
    moveLimitMax = Vector2(correctWindowSize.x,
                           correctWindowSize.y);

    /* キャンバス初期化 */
    // 色管理を初期化してから行う。内部で色の登録を行っている。
    Canvas canvas(10, 10);

    SidePanel sidePanel(correctWindowSize, 
            printAreaLimitMin, printAreaLimitMax);

    sidePanel.update(correctWindowSize, 
            printAreaLimitMin,
            printAreaLimitMax,
            canvas.layers, 
            canvas.getCurrentLayerIter());

    Footer footer(
            FooterData(cursor.getCursorPos(), 
                             canvas.getPos(),
                             context.currentRgb), 
            correctWindowSize.y
    );

    canvas.setMoveLimit(moveLimitMin, moveLimitMax);
    canvas.setPrintAreaLimit(moveLimitMin, moveLimitMax);
    cursor.setMoveLimit(moveLimitMin, moveLimitMax);

    canvas.addLayer("layer");

    ColorPair::NcPairID cursorColorPair = canvas.getTopVisibleColorPair(cursor.getCursorPos());
    if (cursorColorPair != 0)
    {

        Color::NcColorID cursorColor = ColorPair::getPairFromColorPair(cursorColorPair);
        Color::RGB cursorRgb = Color::getRGBFromColor(cursorColor);

        Color::setCursorColor(200, 200, 200, cursorRgb.r, cursorRgb.g, cursorRgb.b);
        ColorPair::setCursorPairColor();
    }
    else
    {
        Color::setCursorColor(200, 200, 200, 100, 100, 100);
        ColorPair::setCursorPairColor(true);
    }
    cursor.print();
    refresh();

    context.canvas = canvas;
    context.cursor = cursor;
    setupKeyBindings(keyActionMap, context);

    int ch;
    context.isClose = false;

    /* mainroop */
    while (!context.isClose)
    {
        // 画面のリサイズに合わせて移動制限範囲を変更
        getmaxyx(stdscr, windowSize.y, windowSize.x);
        correctWindowSize.x = windowSize.x - (windowSize.x % 2) - 2;
        correctWindowSize.y = windowSize.y - 1;


        moveLimitMin = Vector2(0, 0);
        moveLimitMax = Vector2(correctWindowSize.x,
                           correctWindowSize.y);
    
        canvas.setMoveLimit(moveLimitMin, 
                            moveLimitMax);
    
        canvas.setPrintAreaLimit(moveLimitMin, 
                                 moveLimitMax);

        cursor.setMoveLimit(moveLimitMin, 
                            moveLimitMax);

        // 入力文字を取得
        ch = getch();

        if (keyActionMap.contains(ch))
        {
            keyActionMap[ch]();
        }
        else
        {
            mvprintw(correctWindowSize.y, 0, "Invalid Key");
        }

        // 描画処理
        erase();

        // キャンバスの背景とレイヤーの描画
        canvas.print();

        // カーソルの背景色をキャンバスの色に合わせる処理　要リファクタリング
        ColorPair::NcPairID cursorColorPair = canvas.getTopVisibleColorPair(cursor.getCursorPos());
        if (cursorColorPair != 0)
        {
            Color::NcColorID cursorColor = ColorPair::getPairFromColorPair(cursorColorPair);
            Color::RGB cursorRgb = Color::getRGBFromColor(cursorColor);

            Color::setCursorColor(0, 0, 0, cursorRgb.r, cursorRgb.g, cursorRgb.b);

            ColorPair::setCursorPairColor();
        }
        else
        {
            Color::setCursorColor(200, 200, 200, 100, 100, 100);
            ColorPair::setCursorPairColor();
        }
        cursor.print();

        // 現在のカーソル位置を描画
        // attr_set(A_NORMAL, 0, NULL);
        // Vector2 cpos = cursor.getCursorPos();
        // mvprintw(windowSize.y - 2, 0, "(%d, %d)", cpos.x, cpos.y);

        // print可能な範囲
        std::tie(printAreaLimitMin, printAreaLimitMax)
            = canvas.getPrintAreaLimitMinMax();
        
        // SidePanelの情報をアップデート
        sidePanel.update(correctWindowSize, 
            printAreaLimitMin,
            printAreaLimitMax,
            canvas.layers, 
            canvas.getCurrentLayerIter());

        sidePanel.print();

        footer.update(
            FooterData(cursor.getCursorPos(), 
                       canvas.getPos(),
                       context.currentRgb), 
            correctWindowSize.y
        );

        footer.print();

        refresh();
    }

    endwin();
    return 0;
}
