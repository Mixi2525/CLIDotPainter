#include <iostream>

class Application {
public:
    Application(int argc, char* argv[]);

    ~Application();

    // プログラムのメインループや主処理を行う
    int run();
};
