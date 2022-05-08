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
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QResource>
#include <QUrl>
#include <spdlog/spdlog.h>

using namespace Fidgety;
namespace BoostFs = boost::filesystem;

static BoostFs::path exeDir;
static BoostFs::path qmlDir;

static void _logLibraryPaths(const QStringList &paths) {
    spdlog::debug("[main] QGuiApplication app.libraryPaths():");
    for (const auto &path : paths) {
        spdlog::debug("[main]    {0}", path.toStdString());
    }
}

static void _init(const char *argv0) {
    BoostFs::path path = BoostFs::system_complete(BoostFs::path(argv0));
    exeDir = path.parent_path();
    qmlDir = exeDir.parent_path() / "share/fidgety/qml";
}

static std::string _getRccPath(const std::string &rccRelPath) {
    return fmt::format("{0}/{1}", qmlDir.string(), rccRelPath);
}

int32_t main(int32_t argc, char **argv, char **env) {
    _FIDGETY_INIT_APP();

    spdlog::debug("[main] Fidgety is starting up!");

    assert(argc >= 1); // if not true then linus torvalds is going to have a fun time
    _init(argv[0]);

    Editor editor(argc, argv);
    editor.getPathsMut().populateFieldsWithArgv0(argv[0]);

    // initFidgety(app, true);
    _logLibraryPaths(editor.getApp()->libraryPaths());
    editor.registerRcc("qml.rcc");

    spdlog::debug("[main] Fidgety has been initialised");

    spdlog::trace("[main] loading homepage.qml");
    const QUrl homepageQurl("qrc:/homepage.qml");
    editor.load(homepageQurl);

    int32_t status = editor.getApp()->exec();
    if (status == 0) {
        spdlog::debug("[main] Fidgety exited with code 0");
    } else {
        spdlog::error("[main] Fidgety exited with code {}", status);
    }

    return status;
}
