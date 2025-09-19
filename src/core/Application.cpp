#include <iostream>

#include "core/Application.hpp"

Application::Application(int argc, char* argv[]) {
    std::cout << "Application is starting..." << std::endl;
    // ここでコマンドライン引数の解析とかもできるよ
}

Application::~Application() {
    std::cout << "Application is shutting down..." << std::endl;
}

int Application::run() {
    std::cout << "Program running..." << std::endl;

    // ... ここにメインの処理をたくさん書いていく ...

    // 正常終了
    return 0; 
}
