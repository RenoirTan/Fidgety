/**
 * @file src/editor/editor.cpp
 * @author RenoirTan
 * @brief Implementation of Fidgety::Editor.
 * @version 0.1
 * @date 2022-04-20
 * 
 * @copyright Copyright (c) 2022
 */

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QResource>
#include <QScreen>
#include <spdlog/spdlog.h>
#include <fidgety/_utils.hpp>
#include <fidgety/_utils_qt.hpp>
#include <fidgety/editor.hpp>

using namespace Fidgety;
namespace BoostFs = boost::filesystem;

std::string EditorException::codeAsErrorType(void) const {
    switch (mCode) {
        case 0: return "Ok";
        case 1: return "FileNotFound";
        case 2: return "CannotReadFile";
        case 3: return "CannotWriteFile";
        case 4: return "CannotCloseFile";
        case 5: return "ResourceBusy";
        case 6: return "CannotOpenMultipleFiles";
        case 7: return "FilesNotOpen";
        case 8: return "SyntaxError";
        case 9: return "TraversalError";
        case 10: return "QtError";
        case 11: return "UninitializedError";
        default: return "Other";
    }
}

const char *EditorException::getSimpleWhat(void) const noexcept {
    return "A Fidgety::EditorException occurred.";
}

EditorStatus EditorAppPaths::populateFieldsWithArgv0(const char *exePath) {
    return this->populateFieldsWithArgv0(BoostFs::path(exePath));
}

EditorStatus EditorAppPaths::populateFieldsWithArgv0(const boost::filesystem::path &exePath) {
    this->exePath = exePath;
    if (!exePath.has_parent_path() || !exePath.parent_path().has_parent_path()) {
        FIDGETY_ERROR(
            EditorException,
            EditorStatus::TraversalError,
            "[Fidgety::EditorAppPaths::populateFieldsWithArgv0] exePath ('{0}') has no parent path",
            exePath.string()
        );
    }
    this->prefixDir = exePath.parent_path().parent_path();
    this->resourceDir = this->prefixDir / "share/fidgety";
    if (!BoostFs::exists(this->resourceDir)) {
        FIDGETY_ERROR(
            EditorException,
            EditorStatus::TraversalError,
            "[Fidgety::EditorAppPaths::populateFieldsWithArgv0] "
            "this->resourceDir ('{0}') does not exist",
            this->resourceDir.string()
        );
    }
    this->qmlDir = this->resourceDir / "qml";
    if (!BoostFs::exists(this->qmlDir)) {
        FIDGETY_ERROR(
            EditorException,
            EditorStatus::TraversalError,
            "[Fidgety::EditorAppPaths::populateFieldsWithArgv0] "
            "this->qmlDir ('{0}') does not exist",
            this->qmlDir.string()
        );
    }
    return EditorStatus::Ok;
}

Editor::Editor(int32_t argc, char **argv) : QApplication(argc, argv) {
    mEngine = new QQmlApplicationEngine(this);
    // spdlog::trace("[Fidgety::Editor::Editor] mEngine: {0}", (void *) mEngine);
    if (!mEngine) {
        FIDGETY_CRITICAL(
            EditorException,
            EditorStatus::QtError,
            "[Fidgety::Editor::Editor] could not create mApp"
        );
    }
}

Editor::~Editor(void) {
    if (mEngine) delete mEngine;
    QGuiApplication::~QGuiApplication();
}

const EditorAppPaths &Editor::getPaths(void) const noexcept {
    return mPaths;
}

EditorAppPaths &Editor::getPathsMut(void) noexcept {
    return mPaths;
}

EditorStatus Editor::setPaths(EditorAppPaths &&paths) {
    mPaths = std::move(paths);
    return EditorStatus::Ok;
}

QQmlApplicationEngine *Editor::getEngine(void) noexcept {
    return mEngine;
}

EditorStatus Editor::setEngine(QQmlApplicationEngine *engine) {
    if (mEngine) delete mEngine;
    mEngine = engine;
    return EditorStatus::Ok;
}

BoostFs::path Editor::getRccPath(const std::string &relative) const {
    return mPaths.qmlDir / relative;
}

EditorStatus Editor::registerRcc(const std::string &relative) const {
    spdlog::debug("[Fidgety::Editor::registerRcc] getRccPath for '{0}'", relative);
    std::string rccPath = getRccPath(relative).string();
    spdlog::debug("[Fidgety::Editor::registerRcc] registering '{0}'", rccPath);
    if (!BoostFs::exists(rccPath)) {
        FIDGETY_ERROR(
            EditorException,
            EditorStatus::FileNotFound,
            "[Fidgety::Editor::registerRcc] file does not exist: {0}",
            rccPath
        );
    }
    bool hmm = QResource::registerResource(QString(rccPath.c_str()));
    if (hmm) {
        return EditorStatus::Ok;
    } else {
        FIDGETY_ERROR(
            EditorException,
            EditorStatus::QtError,
            "[Fidgety::Editor::registerRcc] could not register '{0}'",
            rccPath
        );
    }
}

EditorStatus Editor::load(const QUrl &qurl) {
    if (!mEngine) {
        FIDGETY_ERROR(
            EditorException,
            EditorStatus::UninitializedError,
            "[Fidgety::Editor::load] mEngine is null"
        );
    }
    spdlog::trace("[Fidgety::Editor::load] loading '{}'", qurl.toString().toStdString());
    mEngine->load(qurl);
    spdlog::trace("[Fidgety::Editor::load] loaded");
    return EditorStatus::Ok;
}
