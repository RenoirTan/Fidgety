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

#   include <boost/filesystem/path.hpp>
#   include <QGuiApplication>
#   include <QQmlApplicationEngine>
#   include <QUrl>
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
        SyntaxError = 8,
        TraversalError = 9,
        QtError = 10,
        UninitializedError = 11
    };

    class EditorException : public Exception {
        public:
            using Exception::Exception;
            std::string codeAsErrorType(void) const;
        
        protected:
            const char *getSimpleWhat(void) const noexcept;
    };

    struct EditorAppPaths {
        boost::filesystem::path prefixDir;
        boost::filesystem::path exePath;
        boost::filesystem::path resourceDir;
        boost::filesystem::path qmlDir;

        EditorStatus populateFieldsWithArgv0(const char *exePath);
        EditorStatus populateFieldsWithArgv0(const boost::filesystem::path &exePath);
    };

    class Editor {
        public:
            Editor(void);
            Editor(int32_t argc, char **argv);
            Editor(EditorAppPaths &&paths, QGuiApplication *app, QQmlApplicationEngine *engine);
            ~Editor(void);

            const EditorAppPaths &getPaths(void) const noexcept;
            EditorAppPaths &getPathsMut(void) noexcept;
            EditorStatus setPaths(EditorAppPaths &&paths);

            QGuiApplication *getApp(void) noexcept;
            EditorStatus setApp(QGuiApplication *app);
            QQmlApplicationEngine *getEngine(void) noexcept;
            EditorStatus setEngine(QQmlApplicationEngine *engine);

            boost::filesystem::path getRccPath(const std::string &relative) const;
            EditorStatus registerRcc(const std::string &relative) const;
            EditorStatus load(const QUrl &qurl);

        protected:
            EditorAppPaths mPaths;
            QGuiApplication *mApp;
            QQmlApplicationEngine *mEngine;
    };

    EditorStatus initFidgety(QCoreApplication &app, bool debugMode=false);
}

#endif
