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
#include <QString>
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