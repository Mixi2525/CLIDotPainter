#include "core/ApplicationContext.hpp"
#include "ui/Canvas.hpp"
#include "ui/Cursor.hpp"
#include <memory>

class Application {
    std::unique_ptr<Canvas> canvas;
    std::unique_ptr<Cursor> cursor;
    ApplicationContext context;

    void InitializeNcursesEnvironment();

public:
    Application(int argc, char* argv[]);

    ~Application();

    // プログラムのメインループや主処理を行う
    int run();
};
