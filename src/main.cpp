#include "core/Application.hpp"

int main(int argc, char* argv[])
{
    // アプリケーションのメインエントリポイント
    Application app = Application(argc, argv);

    return app.run();
}
