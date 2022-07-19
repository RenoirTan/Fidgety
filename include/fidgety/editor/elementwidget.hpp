/**
 * @file include/fidgety/editor/elementwidget.hpp
 * @author RenoirTan
 * @brief Declaration of ElementWidget class which should be one of the parents
 * of all inner widgets in a Fidgety GUI window.
 * @version 0.1
 * @date 2022-07-19
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef FIDGETY_EDITOR_ELEMENTWIDGET_HPP
#   define FIDGETY_EDITOR_ELEMENTWIDGET_HPP

#   include <QApplication>
#   include <fidgety/editor.hpp>

namespace Fidgety {
    class ElementWidget {
        public:
            virtual const char *widgetClassName(void) const noexcept;
            virtual EditorStatus initializeWidget(QApplication *app);
            virtual EditorStatus cleanWidget(void);
            EditorStatus reloadWidget(QApplication *app);
    };
}

#endif
