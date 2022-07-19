/**
 * @file include/fidgety/editor/app.hpp
 * @author RenoirTan
 * @brief Editor App
 * @version 0.1
 * @date 2022-07-16
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef FIDGETY_EDITOR_APP_HPP
#   define FIDGETY_EDITOR_APP_HPP

#   include <QApplication>
#   include <fidgety/editor.hpp>
#   include <fidgety/editor/homepage.hpp>


namespace Fidgety {
    class Q_WIDGETS_EXPORT Editor : public QApplication {
        // Q_OBJECT

        public:
            Editor(int32_t &argc, char **argv);
            ~Editor(void);

            const EditorAppPaths &getPaths(void) const noexcept;
            EditorAppPaths &getPathsMut(void) noexcept;
            EditorStatus setPaths(EditorAppPaths &&paths);

            EditorStatus launchHomepage(void);
            HomepageWidget *getHomepage(void) const noexcept;
            EditorStatus closeHomepage(void);

        protected:
            EditorAppPaths mPaths;
            HomepageWidget *mHomepageWidget;

            void closeAllWindows(void);
    };
}

#endif
