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
#include <QSizePolicy>
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

const QRect HomepageWidget::DEFAULT_FRAME_GEOMETRY = QRect(0, 0, 480, 320);
const char *HomepageWidget::WINDOW_TITLE = "Fidgety";

HomepageWidget::HomepageWidget(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f)
{
    spdlog::trace("[Fidgety::HomepageWidget::HomepageWidget] initialising");
    QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
    setSizePolicy(sizePolicy);
}

HomepageWidget::~HomepageWidget(void) {
    spdlog::trace("[Fidgety::HomepageWidget::HomepageWidget] destroying");
    QWidget::~QWidget();
}

QSize HomepageWidget::sizeHint(void) const {
    return QSize(DEFAULT_FRAME_GEOMETRY.width(), DEFAULT_FRAME_GEOMETRY.height());
}

EditorStatus HomepageWidget::openNewWindow(QApplication *app) {
    spdlog::debug("[Fidgety::HomepageWidget::openNewWindow] opening new window");
    show();
    setWindowTitle(app->translate("toplevel", HomepageWidget::WINDOW_TITLE));
    spdlog::debug("[Fidgety::HomepageWidget::openNewWindow] new window opened");
    return EditorStatus::Ok;
}
