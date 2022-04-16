/**
 * @file include/fidgety/options/_option_editor.hpp
 * @author RenoirTan
 * @brief Fidgety::OptionEditor determines how a setting can be edited and does
 * bound-checking at runtime.
 * @version 0.1
 * @date 2022-04-15
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef _FIDGETY_OPTIONS_OPTION_EDITOR_HPP
#   define _FIDGETY_OPTIONS_OPTION_EDITOR_HPP

#   include "_fwd.hpp"

namespace Fidgety {
    enum class OptionEditorType : int32_t {
        Blanked = 0,
        TextEntry = 1,
        Toggle = 2,
        Slider = 3,
        Dropdown = 4,
        Options = 5,
        Checkboxes = 6
    };

    const char *const OPTION_EDITOR_TYPES_STRING[] = {
        "blanked",
        "textentry",
        "toggle",
        "slider",
        "dropdown",
        "options",
        "checkboxes"
    };

    class OptionEditor {
        public:
            OptionEditor(OptionEditorType oet, std::map<std::string, std::string> &&constraints);

        protected:
            OptionEditorType mEditorType;
                std::map<std::string, std::string> mConstraints;
    };
}

#endif
