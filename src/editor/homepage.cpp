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
#include <QTableWidget>
#include <spdlog/spdlog.h>
#include <fidgety/_utils.hpp>
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
    spdlog::debug("[Fidgety::HomepageWidget::HomepageWidget] creating");
    mMainGrid = nullptr;
}

HomepageWidget::~HomepageWidget(void) {
    spdlog::debug("[Fidgety::HomepageWidget::~HomepageWidget] deleting");
    deleteWindowElements();
}

QSize HomepageWidget::sizeHint(void) const {
    return DEFAULT_SIZE;
}

QSize HomepageWidget::minimumSizeHint(void) const {
    return MINIMUM_SIZE;
}

const char *HomepageWidget::windowClassName(void) const noexcept {
    return "Fidgety::HomepageWidget";
}

EditorStatus HomepageWidget::deleteWindowElements(void) {
    spdlog::debug("[Fidgety::HomepageWidget::deleteWindowElements] deleting inner elements");
    if (mMainGrid != nullptr) {
        spdlog::trace("[Fidgety::HomepageWidget::deleteWindowElements] deleting mMainGrid");
        delete mMainGrid;
        mMainGrid = nullptr;
        spdlog::trace("[Fidgety::HomepageWidget::deleteWindowElements] deleted mMainGrid");
    }
    return EditorStatus::Ok;
}

EditorStatus HomepageWidget::initializeWindowElements(QApplication *app) {
    spdlog::debug("[Fidgety::HomepageWidget::initializeWindowElements] initialising window");
    setMinimumSize(MINIMUM_SIZE);
    setWindowTitle(app->translate("toplevel", HomepageWidget::WINDOW_TITLE));
    resize(DEFAULT_SIZE);

    mMainGrid = new QGridLayout(this);
    if (mMainGrid == nullptr) {
        FIDGETY_ERROR(
            EditorException,
            EditorStatus::QtError,
            "[Fidgety::HomepageWidget::initializeWindowElements] could not create mMainGrid"
        );
    }
    mMainGrid->setSpacing(5);

    {
        QTableWidget *fileList = new QTableWidget;
        if (fileList == nullptr) {
            FIDGETY_ERROR(
                EditorException,
                EditorStatus::QtError,
                "[Fidgety::HomepageWidget::initializeWindowElements] could not create fileList"
            );
        }
        mMainGrid->addWidget(fileList);
    }

    spdlog::debug("[Fidgety::HomepageWidget::initializeWindowElements] new window opened");
    return EditorStatus::Ok;
}
