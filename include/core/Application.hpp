#include "core/ApplicationContext.hpp"
#include "ui/Canvas.hpp"
#include "ui/Cursor.hpp"
#include "ui/SidePanel.hpp"
#include "ui/Footer.hpp"
#include <memory>

class Application {
    std::unique_ptr<Canvas> canvas;
    std::unique_ptr<Cursor> cursor;
    std::unique_ptr<SidePanel> sidePanel;
    std::unique_ptr<Footer> footer;
    ApplicationContext context;

    void InitializeNcursesEnvironment();

public:
    Application(int argc, char* argv[]);

    ~Application();

    // プログラムのメインループや主処理を行う
    int run();
};
