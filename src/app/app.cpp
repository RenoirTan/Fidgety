/**
 * @file src/app/app.cpp
 * @author RenoirTan
 * @brief Source file containing the entry point for Fidgety's g app.
 * @version 0.1
 * @date 2022-04-22
 * 
 * @copyright Copyright (c) 2022
 */

#include <iostream>
#include <cstddef>
#include <cstdint>
#include <fidgety/editor.hpp>
#include <QApplication>
#include <spdlog/spdlog.h>

using namespace Fidgety;

int32_t main(int32_t argc, char **argv, char **env) {
    spdlog::debug("[main] Fidgety is starting up!");
    
    QApplication app(argc, argv);
    Editor editor;
    
    editor.show();

    int32_t status = app.exec();
    if (status == 0) {
        spdlog::debug("[main] Fidgety exited with code 0");
    } else {
        spdlog::error("[main] Fidgety exited with code {}", status);
    }

    return status;
}
