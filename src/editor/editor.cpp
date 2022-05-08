/**
 * @file src/editor/editor.cpp
 * @author RenoirTan
 * @brief Implementation of Fidgety::Editor.
 * @version 0.1
 * @date 2022-04-20
 * 
 * @copyright Copyright (c) 2022
 */

#include <QGuiApplication>
#include <QScreen>
#include <fidgety/_utils_qt.hpp>
#include <fidgety/editor.hpp>

using namespace Fidgety;

Editor::Editor(QWidget *parent) : QWidget(parent) {
    QRect screenDimensions = QGuiApplication::primaryScreen()->geometry();
    setMinimumSize(480, 270); // 16:9
    setBaseSize(screenDimensions.width() * 0.7, screenDimensions.height() * 0.7);
    setWindowTitle("Fidgety");
    mAppList = new QListView(this);
    mAppList->setGeometry(getGeometry(this->geometry(), QSize(150, 100), 10));
}

EditorStatus Fidgety::initFidgety(QCoreApplication &app, bool debugMode) {
    if (debugMode) {
        app.addLibraryPath("./resources/qml/");
    } else {
        app.addLibraryPath("/usr/share/fidgety/resources/qml/");
    }
    return EditorStatus::Ok;
}
