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

#   include <QObject>
#   include <QString>

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
}

#endif
