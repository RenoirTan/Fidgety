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
#   include <QRect>
#   include <QScreen>
#   include <QSize>
#   include <QString>
#   include <QWidget>
#   include <QWindow>
#   include <fidgety/editor.hpp>

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

    class HomepageWidget : public QWidget {
        public:
            static const QRect DEFAULT_FRAME_GEOMETRY; // QRect(0, 0, 480, 320)
            static const char *WINDOW_TITLE; // "Fidgety"

            HomepageWidget(QWidget *parent, Qt::WindowFlags f=Qt::WindowFlags());
            ~HomepageWidget(void);

            QSize sizeHint(void) const;

            EditorStatus openNewWindow(QApplication *app);
        protected:
    };
}

#endif
