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
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <fidgety/_general.hpp>
#include <fidgety/editor.hpp>
#include <fmt/core.h>
#include <QUrl>
#include <spdlog/spdlog.h>

using namespace Fidgety;
namespace BoostFs = boost::filesystem;

static BoostFs::path exeDir;
static BoostFs::path qmlDir;

static void _logLibraryPaths(const QStringList &paths) {
    spdlog::debug("[_logLibraryPaths] app.libraryPaths():");
    for (const auto &path : paths) {
        spdlog::debug("[_logLibraryPaths]    {0}", path.toStdString());
    }
}

int32_t run(int32_t argc, char **argv, char **env) {
    spdlog::debug("[run] Fidgety is starting up!");

    Editor editor(argc, argv);
    editor.getPathsMut().populateFieldsWithArgv0(argv[0]);
    _logLibraryPaths(editor.libraryPaths());
    editor.registerRcc("qml.rcc");

    spdlog::debug("[run] Fidgety has been initialised");

    spdlog::trace("[run] loading homepage.qml");
    const QUrl homepageQurl("qrc:/homepage.qml");
    editor.load(homepageQurl);

    spdlog::debug("[run] loaded homepage.qml");

    int32_t status = editor.exec();
    if (status == 0) {
        spdlog::debug("[run] Fidgety exited with code 0");
    } else {
        spdlog::error("[run] Fidgety exited with code {}", status);
    }

    return status;
}

int32_t main(int32_t argc, char **argv, char **env) {
    _FIDGETY_INIT_APP();
    spdlog::debug("[main] entry point");
    int32_t status = run(argc, argv, env);
    spdlog::debug("[main] exiting");
    return status;
}
