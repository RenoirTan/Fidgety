/**
 * @file include/fidgety/options/_fwd.hpp
 * @author RenoirTan
 * @brief Forward declarations for definitions used in Fidgety::Options
 * @version 0.1
 * @date 2022-04-14
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef _FIDGETY_OPTIONS_FWD_HPP
#   define _FIDGETY_OPTIONS_FWD_HPP

#   include <string>
#   include <map>
#   include <vector>
#   include <memory>

namespace Fidgety {
    enum class ValidatorMessageType;
    class ValidatorMessage;
    class ValidatorContext;
    class Validator;
    class OptionIdentifier;
    enum class OptionStatus;
    class OptionException;
    class OptionValue;
    enum class OptionEditorType;
    class OptionEditor;
    class Option;

    using OptionName = std::string;
    using OptionsMap = std::map<OptionIdentifier, std::shared_ptr<Option>>;
    using ValidatorContextInner = OptionsMap;
    using NestedOptionNameList = std::vector<OptionName>;
    using OptionIdentifierList = std::vector<OptionIdentifier>;
    using InnerOptionsNameList = std::vector<OptionName>;
}

#endif
