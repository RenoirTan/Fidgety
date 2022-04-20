/**
 * @file include/fidgety/editor.hpp
 * @author RenoirTan
 * @brief Header file for Fidgety's graphical user interface.
 * @version 0.1
 * @date 2022-04-20
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef FIDGETY_EDITOR_HPP
#   define FIDGETY_EDITOR_HPP

#   include <QPushButton>
#   include <QWidget>

namespace Fidgety {
    class Editor : public QWidget {
        Q_OBJECT
        public:
            Editor(QWidget *parent = nullptr);

        signals:
        public slots:

        protected:
            QPushButton *mButton;
    };
}

#endif
