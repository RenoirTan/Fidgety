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

EditorStatus WindowWidget::initializeWindow(QApplication *app) {
    spdlog::trace("[Fidgety::WindowWidget::initializeWindow] initialising Window");
    return EditorStatus::Ok;
}

EditorStatus WindowWidget::openNewWindow(QApplication *app) {
    spdlog::debug("[Fidgety::WindowWidget::openNewWindow] initialising window");
    EditorStatus status = initializeWindow(app);
    spdlog::debug("[Fidgety::WindowWidget::openNewWindow] opening new window");
    show();
    return status;
}
