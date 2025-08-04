
#include <clocale>
#include <cstdint>
#include <ncurses.h>
#include <random>
#include <string>
#include <tuple>

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

int main()
{
    Cursor cursor;
    Vector2 windowSize;
    Vector2 correctWindowSize;
    Vector2 moveLimitMin, moveLimitMax;
    Vector2 printAreaLimitMin, printAreaLimitMax;
    Color::RGB currentRgb(0, 0, 0);

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
                                        currentRgb), 
            windowSize.y
    );

    canvas.setMoveLimit(moveLimitMin, moveLimitMax);
    canvas.setPrintAreaLimit(moveLimitMin, moveLimitMax);
    cursor.setMoveLimit(moveLimitMin, moveLimitMax);

    canvas.addLayer("layer");

    // デバッグ用のランダム値生成装置作成
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> dist(0, 255);


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

    int ch;
    bool isClose = false;

    /* mainroop */
    while (!isClose)
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

        switch (ch)
        {
            case 'h':
            case KEY_LEFT:
                {
                    cursor.move(Vector2::LEFT);
                }
                break;

            case 'j':
            case KEY_DOWN:
                {
                    cursor.move(Vector2::DOWN);
                }
                break;

            case 'k':
            case KEY_UP:
                {
                    cursor.move(Vector2::UP);
                }
                break;

            case 'l':
            case KEY_RIGHT:
                {
                    cursor.move(Vector2::RIGHT);
                }
                break;

            case 'q':
                {
                    isClose = true;
                }
                break;

            case 'd':
                {
                    debugAction(canvas, cursor);
                }
                break;

            // Space key
            case 32:
                {
                    Vector2 cpos = cursor.getCursorPos(); // キャンバスの正方形ピクセル座標
                    Vector2 canvasSize = canvas.getCanvasSize();

                    if (0 <= cpos.y && cpos.y < canvasSize.y &&
                        0 <= cpos.x && cpos.x < canvasSize.x)
                    {
                        currentRgb.r = dist(mt);
                        currentRgb.g = dist(mt);
                        currentRgb.b = dist(mt);

                        Color::NcColorID colnum = Color::allocateNcursesColor(currentRgb);
                        ColorPair::NcPairID pair = ColorPair::allocateNcursesColorPair(0, colnum);
                        canvas.getCurrentLayerIter()->setpx(cpos.y, cpos.x, pair);
                    }
                }
                break;


            case ERR:
                {

                }
                break;

            default:
                {

                }
                break;
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

            int res = Color::setCursorColor(0, 0, 0, cursorRgb.r, cursorRgb.g, cursorRgb.b);

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
                                        currentRgb), 
            windowSize.y
        );

        footer.print();

        refresh();
    }

    endwin();
    return 0;
}
