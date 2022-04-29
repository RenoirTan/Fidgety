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
#include <QQmlApplicationEngine>
#include <QUrl>
#include <spdlog/spdlog.h>

using namespace Fidgety;

int32_t main(int32_t argc, char **argv, char **env) {
    spdlog::debug("[main] Fidgety is starting up!");
    
    QApplication app(argc, argv);
    initFidgety(true);

    spdlog::debug("[main] Fidgety has been initialised");
    spdlog::debug("[main] setting up QQmlApplicationEngine");

    QQmlApplicationEngine engine;
    spdlog::trace("[main] loading homepage.qml");
    engine.load(QUrl("qrc:/homepage.qml"));

    int32_t status = app.exec();
    if (status == 0) {
        spdlog::debug("[main] Fidgety exited with code 0");
    } else {
        spdlog::error("[main] Fidgety exited with code {}", status);
    }

    return status;
}
