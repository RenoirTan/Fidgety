/**
 * @file include/fidgety/editor/windowwidget.hpp
 * @author RenoirTan
 * @brief Base class for widgets (that act like windows) in Fidgety
 * @version 0.1
 * @date 2022-07-16
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef FIDGETY_EDITOR_WINDOWWIDGET_HPP
#   define FIDGETY_EDITOR_WINDOWWIDGET_HPP

#   include <QApplication>
#   include <QSize>
#   include <QWidget>
#   include <fidgety/editor.hpp>

namespace Fidgety {
    class WindowWidget : public QWidget {
        public:
            using QWidget::QWidget;

            virtual EditorStatus initializeWindow(QApplication *app);
            virtual EditorStatus openNewWindow(QApplication *app);
    };
}

#endif