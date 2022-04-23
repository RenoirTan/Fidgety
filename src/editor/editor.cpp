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
#include <QPushButton>
#include <QScreen>
#include <fidgety/editor.hpp>

using namespace Fidgety;

Editor::Editor(QWidget *parent) : QWidget(parent) {
    QRect screenDimensions = QGuiApplication::primaryScreen()->geometry();
    setFixedSize(screenDimensions.width() * 0.7, screenDimensions.height() * 0.7);
    setWindowTitle("Fidgety");
    mButton = new QPushButton("Test button", this);
    mButton->setGeometry(10, 10, 150, 50);
}
