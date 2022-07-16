/**
 * @file include/fidgety/editor/homepage.hpp
 * @author RenoirTan
 * @brief Backend of the home page
 * @version 0.1
 * @date 2022-06-01
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef FIDGETY_EDITOR_HOMEPAGE_HPP
#   define FIDGETY_EDITOR_HOMEPAGE_HPP

#   include <QApplication>
#   include <QObject>
#   include <QScreen>
#   include <QSize>
#   include <QString>
#   include <QWidget>
#   include <QWindow>
#   include <fidgety/editor.hpp>
#   include <fidgety/editor/windowwidget.hpp>

namespace Fidgety {
    class HomepageBackend : public QObject {
        // Q_OBJECT
        public:
            HomepageBackend(QObject *parent = nullptr);

            Q_INVOKABLE void printSelectedConfigFile(
                const QString &name,
                const QString &path
            );
    };

    class HomepageWidget : public WindowWidget {

        public:
            static const QSize DEFAULT_SIZE; // QSize(720, 480)
            static const QSize MINIMUM_SIZE; // QSize(480, 320)
            static const char *WINDOW_TITLE; // "Fidgety"

            HomepageWidget(QWidget *parent, Qt::WindowFlags f=Qt::WindowFlags());
            ~HomepageWidget(void);

            QSize sizeHint(void) const;
            QSize minimumSizeHint(void) const;

            EditorStatus initializeWindow(QApplication *app);
        protected:
    };
}

#endif
