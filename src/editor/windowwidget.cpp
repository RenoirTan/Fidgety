/**
 * @file src/windowwidget.cpp
 * @author RenoirTan
 * @brief Base class for widgets (that act like windows) in Fidgety
 * @version 0.1
 * @date 2022-07-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <QSize>
#include <spdlog/spdlog.h>
#include <fidgety/editor/windowwidget.hpp>

using namespace Fidgety;

WindowWidget::WindowWidget(QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f) {
    spdlog::debug(
        "[Fidgety::WindowWidget::WindowWidget] creating window object: {0}",
        windowClassName()
    );
}

WindowWidget::~WindowWidget(void) {
    spdlog::trace(
        "[Fidgety::WindowWidget::~WindowWidget] deleting window object: {0}",
        windowClassName()
    );
}

const char *WindowWidget::windowClassName(void) const noexcept {
    return "Fidgety::WindowWidget";
}

EditorStatus WindowWidget::initializeWindow(QApplication *app) {
    spdlog::trace(
        "[Fidgety::WindowWidget::initializeWindow] initialising window: {0}",
        windowClassName()
    );
    return EditorStatus::Ok;
}

EditorStatus WindowWidget::openNewWindow(QApplication *app) {
    const char *wcname = windowClassName();
    spdlog::debug("[Fidgety::WindowWidget::openNewWindow] initialising window: {0}", wcname);
    EditorStatus status = initializeWindow(app);
    spdlog::debug("[Fidgety::WindowWidget::openNewWindow] opening new window: {0}", wcname);
    show();
    return status;
}
