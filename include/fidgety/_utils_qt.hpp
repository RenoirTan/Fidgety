/**
 * @file include/fidgety/_utils_qt.hpp
 * @author RenoirTan
 * @brief Utility functions to make programming with Qt much better.
 * @version 0.1
 * @date 2022-04-24
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef _FIDGETY_UTILS_QT_HPP
#   define _FIDGETY_UTILS_QT_HPP

#   include <QSize>
#   include <QWidget>

namespace Fidgety {
    int32_t getFreeRealEstate(int32_t available, int32_t minAllowed, int32_t margin);
    QRect getGeometry(QRect available, QSize minAllowed, int32_t xMargin, int32_t yMargin);
    QRect getGeometry(QRect available, QSize minAllowed, int32_t margin=0);
}

#endif
