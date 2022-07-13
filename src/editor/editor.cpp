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

EditorStatus EditorAppPaths::populateFieldsWithArgv0(const BoostFs::path &exePath) {
    this->exePath = BoostFs::canonical(exePath);
    if (!this->exePath.has_parent_path() || !this->exePath.parent_path().has_parent_path()) {
        FIDGETY_ERROR(
            EditorException,
            EditorStatus::TraversalError,
            "[Fidgety::EditorAppPaths::populateFieldsWithArgv0] "
            "exePath ('{0}', '{1}') has no parent path",
            exePath.string(),
            this->exePath.string()
        );
    }
    this->prefixDir = this->exePath.parent_path().parent_path();
    this->resourceDir = this->prefixDir / "share/fidgety";
    this->qmlDir = this->resourceDir / "qml";
    return EditorStatus::Ok;
}

Editor::Editor(int32_t &argc, char **argv) : QApplication(argc, argv) {
    spdlog::trace("[Fidgety::Editor::Editor] initialising");
}

Editor::~Editor(void) {
    spdlog::trace("[Fidgety::Editor::~Editor] deleting");
}

const EditorAppPaths &Editor::getPaths(void) const noexcept {
    return mPaths;
}

EditorAppPaths &Editor::getPathsMut(void) noexcept {
    return mPaths;
}

EditorStatus Editor::setPaths(EditorAppPaths &&paths) {
    spdlog::trace("[Fidgety::Editor::setPaths] setting new mPaths");
    mPaths = std::move(paths);
    return EditorStatus::Ok;
}