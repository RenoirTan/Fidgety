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

#   include <QCoreApplication>
#   include <QPushButton>
#   include <QListView>
#   include <QWidget>
#   include <fidgety/exception.hpp>

namespace Fidgety {
    enum class EditorStatus : int32_t {
        Ok = 0,
        FileNotFound = 1,
        CannotReadFile = 2,
        CannotWriteFile = 3,
        CannotCloseFile = 4,
        ResourceBusy = 5,
        CannotOpenMultipleFiles = 6,
        FilesNotOpen = 7,
        SyntaxError = 8
    };

    class EditorException : public Exception {
        public:
            using Exception::Exception;
            std::string codeAsErrorType(void) const;
        
        protected:
            const char *getSimpleWhat(void) const noexcept;
    };

    class Editor : public QWidget {
        // DO NOT FUCKING ADD Q_OBJECT

        public:
            explicit Editor(QWidget *parent = 0);

        signals:
        public slots:

        protected:
            QListView *mAppList;
    };

    EditorStatus initFidgety(QCoreApplication &app, bool debugMode=false);
}

#endif
