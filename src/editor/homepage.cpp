/**
 * @file src/editor/rootwindow.cpp
 * @author RenoirTan
 * @brief Backend of the root ApplicationWindow
 * @version 0.1
 * @date 2022-06-01
 * 
 * @copyright Copyright (c) 2022
 */

#include <iostream>
#include <fmt/core.h>
#include <QObject>
#include <QScreen>
#include <QString>
#include <spdlog/spdlog.h>
#include <fidgety/editor/homepage.hpp>

using namespace Fidgety;

HomepageBackend::HomepageBackend(QObject *parent) {
    Q_UNUSED(parent);
}

void HomepageBackend::printSelectedConfigFile(
    const QString &name,
    const QString &path
) {
    std::cout
        << fmt::format("name: {0}, path: {1}", name.toStdString(), path.toStdString())
        << std::endl;
}

const QSize HomepageWidget::DEFAULT_SIZE = QSize(480, 320);
const QSize HomepageWidget::MINIMUM_SIZE = QSize(720, 480);
const char *HomepageWidget::WINDOW_TITLE = "Fidgety";

HomepageWidget::HomepageWidget(QWidget *parent, Qt::WindowFlags f) :
    WindowWidget(parent, f)
{
    spdlog::trace("[Fidgety::HomepageWidget::HomepageWidget] initialising");
}

HomepageWidget::~HomepageWidget(void) {
    spdlog::trace("[Fidgety::HomepageWidget::HomepageWidget] destroying");
    QWidget::~QWidget();
}

QSize HomepageWidget::sizeHint(void) const {
    return DEFAULT_SIZE;
}

QSize HomepageWidget::minimumSizeHint(void) const {
    return MINIMUM_SIZE;
}

EditorStatus HomepageWidget::initializeWindow(QApplication *app) {
    spdlog::debug("[Fidgety::HomepageWidget::openNewWindow] initialising window");
    setMinimumSize(MINIMUM_SIZE);
    setWindowTitle(app->translate("toplevel", HomepageWidget::WINDOW_TITLE));
    resize(DEFAULT_SIZE);
    spdlog::debug("[Fidgety::HomepageWidget::openNewWindow] new window opened");
    return EditorStatus::Ok;
}
