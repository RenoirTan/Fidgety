/**
 * @file src/editor/editor.cpp
 * @author RenoirTan
 * @brief Implementation of Fidgety::Editor.
 * @version 0.1
 * @date 2022-04-20
 * 
 * @copyright Copyright (c) 2022
 */

#include <fidgety/editor.hpp>

using namespace Fidgety;

Editor::Editor(QWidget *parent) : QWidget(parent) {
    setFixedSize(100, 50);
    mButton = new QPushButton("Test button", this);
    mButton->setGeometry(10, 10, 80, 30);
}
