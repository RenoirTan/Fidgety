/**
 * @file src/editor/elementwidget.cpp
 * @author RenoirTan
 * @brief Implementation of ElementWidget class which should be one of the
 * parents of all inner widgets in a Fidgety GUI window.
 * @version 0.1
 * @date 2022-07-19
 *
 * @copyright Copyright (c) 2022
 */

#include <spdlog/spdlog.h>
#include <fidgety/editor/elementwidget.hpp>

using namespace Fidgety;

const char *ElementWidget::widgetClassName(void) const noexcept {
    return "Fidgety::ElementWidget";
}

EditorStatus ElementWidget::initializeWidget(QApplication *app) {
    spdlog::trace(
        "[Fidgety::ElementWidget::initializeWidget] initialising widget: {0}",
        widgetClassName()
    );
    return EditorStatus::Ok;
}

EditorStatus ElementWidget::cleanWidget(void) {
    spdlog::trace(
        "[Fidgety::ElementWidget::initializeWidget] cleaning widget: {0}",
        widgetClassName()
    );
    return EditorStatus::Ok;
}

EditorStatus ElementWidget::reloadWidget(QApplication *app) {
    spdlog::debug(
        "[Fidgety::ElementWidget::reloadWidget] reloading widget: {0}",
        widgetClassName()
    );
    EditorStatus status = cleanWidget();
    if (status != EditorStatus::Ok) return status;
    status = initializeWidget(app);
    return status;
}
