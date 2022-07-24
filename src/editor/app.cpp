/**
 * @file src/editor/app.cpp
 * @author RenoirTan
 * @brief Editor App
 * @version 0.1
 * @date 2022-07-16
 * 
 * @copyright Copyright (c) 2022
 */

#include <spdlog/spdlog.h>
#include <fidgety/editor.hpp>
#include <fidgety/editor/app.hpp>
#include <fidgety/_utils.hpp>

using namespace Fidgety;

Editor::Editor(int32_t &argc, char **argv) : QApplication(argc, argv) {
    spdlog::trace("[Fidgety::Editor::Editor] initialising");
    mHomepageWidget = nullptr;
}

Editor::~Editor(void) {
    spdlog::trace("[Fidgety::Editor::~Editor] deleting");
    closeAllWindows();
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

EditorStatus Editor::launchHomepage(void) {
    EditorStatus status = EditorStatus::Ok;
    spdlog::debug("[Fidgety::Editor::launchHomepage] launching homepage...");
    if (mHomepageWidget != nullptr) {
        FIDGETY_ERROR(
            EditorException,
            EditorStatus::NotFreeError,
            "[Fidgety::Editor::launchHomepage] homepage already launched"
        );
    }
    mHomepageWidget = new HomepageWidget(nullptr);
    if (mHomepageWidget == nullptr) {
        FIDGETY_ERROR(
            EditorException,
            EditorStatus::QtError,
            "[Fidgety::Editor::launchHomepage] could not launch homepage"
        );
    }
    spdlog::trace("[Fidgety::Editor::launchHomepage] got homepage handle");
    status = mHomepageWidget->openNewWindow(this);
    return status;
}

HomepageWidget *Editor::getHomepage(void) const noexcept {
    return mHomepageWidget;
}

EditorStatus Editor::closeHomepage(void) {
    spdlog::debug("[Fidgety::Editor::closeHomepage] closing homepage");
    if (mHomepageWidget != nullptr) {
        delete mHomepageWidget;
    }
    mHomepageWidget = nullptr;
    return EditorStatus::Ok;
}

void Editor::closeAllWindows(void) {
    closeHomepage();
}