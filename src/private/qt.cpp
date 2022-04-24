/**
 * @file src/private/qt.cpp
 * @author RenoirTan
 * @brief Definitions of declaration in `include/fidgety/_utils_qt.hpp`
 * @version 0.1
 * @date 2022-04-24
 * 
 * @copyright Copyright (c) 2022
 */

#include <fidgety/_utils_qt.hpp>

using namespace Fidgety;

int32_t Fidgety::getFreeRealEstate(int32_t available, int32_t minAllowed, int32_t margin) {
    if (available < minAllowed) {
        return minAllowed;
    } else {
        return (available-margin);
    }
}

QRect Fidgety::getGeometry(QRect available, QSize minAllowed, int32_t xMargin, int32_t yMargin) {
    int32_t trulyAvailableX = available.width() - xMargin;
    int32_t trulyAvailableY = available.height() - yMargin;
    int32_t topLeftX = available.x() + xMargin;
    int32_t topLeftY = available.y() + yMargin;
    QRect geometry(
        topLeftX,
        topLeftY,
        getFreeRealEstate(trulyAvailableX, minAllowed.width(), xMargin),
        getFreeRealEstate(trulyAvailableY, minAllowed.height(), yMargin)
    );
    return geometry;
}

QRect Fidgety::getGeometry(QRect available, QSize minAllowed, int32_t margin) {
    return getGeometry(available, minAllowed, margin, margin);
}
