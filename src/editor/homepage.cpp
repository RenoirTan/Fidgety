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
#include <QHeaderView>
#include <QObject>
#include <QScreen>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
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

const QStringList HomepageFilelistWidget::COLUMN_NAMES = {"App", "File"};

HomepageFilelistWidget::~HomepageFilelistWidget(void) {
    spdlog::trace(
        "[Fidgety::HomepageFilelistWidget::~HomepageFilelistWidget] deleting"
    );
    cleanWidget();
}

const char *HomepageFilelistWidget::widgetClassName(void) const noexcept {
    return "Fidgety::HomepageFilelistWidget";
}

EditorStatus HomepageFilelistWidget::initializeWidget(QApplication *app) {
    spdlog::trace("[Fidgety::HomepageFilelistWidget::initializeWidget] initialising");
    setColumnCount(COLUMN_NAMES.length());
    setHorizontalHeaderLabels(COLUMN_NAMES);
    QHeaderView *header = horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    spdlog::trace("[Fidgety::HomepageFilelistWidget::initializeWidget] columns setup");
    return EditorStatus::Ok;
}

EditorStatus HomepageFilelistWidget::cleanWidget(void) {
    spdlog::trace("[Fidgety::HomepageFilelistWidget::initializeWidget] cleaning");
    return EditorStatus::Ok;
}

EditorStatus HomepageFilelistWidget::addAppdata(const Appdata &appdata) {
    spdlog::trace("[Fidgety::HomepageFilelistWidget::addAppdata] creating one row");
    insertRow(rowCount());
    QTableWidgetItem *app = new QTableWidgetItem((QString) appdata.appName.c_str());
    QTableWidgetItem *file = new QTableWidgetItem((QString) appdata.configFilePath.c_str());
    int lastRowIndex = rowCount()-1;
    setItem(lastRowIndex, 0, app);
    setItem(lastRowIndex, 1, file);
    spdlog::trace("[Fidgety::HomepageFilelistWidget::addAppdata] row set");
    return EditorStatus::Ok;
}

EditorStatus HomepageFilelistWidget::addAppdata(const std::vector<Appdata> &appdata) {
    spdlog::trace("[Fidgety::HomepageFilelistWidget::addAppdata] adding a bunch of data");
    for (const auto &data : appdata) {
        spdlog::trace("[Fidgety::HomepageFilelistWidget::addAppdata] creating one row");
        insertRow(rowCount());
        QTableWidgetItem *app = new QTableWidgetItem((QString) data.appName.c_str());
        QTableWidgetItem *file = new QTableWidgetItem((QString) data.configFilePath.c_str());
        int lastRowIndex = rowCount() - 1;
        setItem(lastRowIndex, 0, app);
        setItem(lastRowIndex, 1, file);
        spdlog::trace("[Fidgety::HomepageFilelistWidget::addAppdata] row set");
    }
    spdlog::trace("[Fidgety::HomepageFilelistWidget::addAppdata] done");
    return EditorStatus::Ok;
}

const QSize HomepageWidget::DEFAULT_SIZE = QSize(480, 320);
const QSize HomepageWidget::MINIMUM_SIZE = QSize(720, 480);
const char *HomepageWidget::WINDOW_TITLE = "Fidgety";

HomepageWidget::HomepageWidget(QWidget *parent, Qt::WindowFlags f) :
    WindowWidget(parent, f)
{
    spdlog::debug("[Fidgety::HomepageWidget::HomepageWidget] creating");
    mMainGrid = nullptr;
    mFilelist = nullptr;
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
    if (mFilelist != nullptr) {
        spdlog::trace("[Fidgety::HomepageWidget::deleteWindowElements] deleting mFilelist");
        delete mFilelist;
        mFilelist = nullptr;
        spdlog::trace("[Fidgety::HomepageWidget::deleteWindowElements] deleted mFilelist");
    }
    return EditorStatus::Ok;
}

HomepageFilelistWidget *HomepageWidget::getFilelistWidget(void) const noexcept {
    return mFilelist;
}

EditorStatus HomepageWidget::initializeWindowElements(QApplication *app) {
    EditorStatus status = EditorStatus::Ok;
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

    mFilelist = new HomepageFilelistWidget;
    if (mFilelist == nullptr) {
        FIDGETY_ERROR(
            EditorException,
            EditorStatus::QtError,
            "[Fidgety::HomepageWidget::initializeWindowElements] could not create mFilelist"
        );
    }
    status = mFilelist->initializeWidget(app);
    if (status != EditorStatus::Ok) return status;
    
    mMainGrid->addWidget(mFilelist);

    spdlog::debug("[Fidgety::HomepageWidget::initializeWindowElements] new window opened");
    return EditorStatus::Ok;
}
