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
            Q_DISABLE_COPY(WindowWidget);
            WindowWidget(QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags());
            ~WindowWidget(void);

            virtual const char *windowClassName(void) const noexcept;
            virtual EditorStatus deleteWindowElements(void);
            EditorStatus initializeWindow(QApplication *app);
            EditorStatus openNewWindow(QApplication *app);
        
        protected:
            virtual EditorStatus initializeWindowElements(QApplication *app);
    };
}

#endif