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

#   include <QWidget>

class QPushButton;

namespace Fidgety {
    class Editor : public QWidget {
        // DO NOT FUCKING ADD Q_OBJECT

        public:
            explicit Editor(QWidget *parent = 0);

        signals:
        public slots:

        protected:
            QPushButton *mButton;
    };
}

#endif
