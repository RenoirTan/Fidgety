/**
 * @file include/fidgety/options/_option.hpp
 * @author RenoirTan
 * @brief Fidgety::Option is the class that manages the value of a setting and
 * is the interface between the editor and the verifier.
 * @version 0.1
 * @date 2022-04-15
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef _FIDGETY_OPTIONS_OPTION_HPP
#   define _FIDGETY_OPTIONS_OPTION_HPP

#   include "_fwd.hpp"
#   include "_option_editor.hpp"
#   include "_option_identifier.hpp"
#   include "_option_value.hpp"
#   include "_validator_message.hpp"
#   include <fmt/format.h>

namespace Fidgety {
    class Option {
        public:
            Option(
                OptionIdentifier identifier,
                OptionEditor &&optionEditor,
                std::unique_ptr<Validator> &&validator,
                int32_t acceptedValueTypes
            ) noexcept;
            Option(
                OptionIdentifier identifier,
                OptionEditor &&optionEditor,
                std::unique_ptr<Validator> &&validator,
                OptionValue &&value
            );
            ~Option(void);

            // Option(const Option &option) = delete;
            Option(Option &&option);
            Option &operator=(Option &&option);

            const OptionIdentifier &getIdentifier(void) const noexcept;
            const OptionValue &getOptionValue(void) const noexcept;
            const OptionValueInner &getValue(void) const noexcept;
            const OptionValueInner &getDefaultValue(void) const noexcept;

            int32_t getValueType(void) const;
            const NestedOptionNameList &getNestedList(void) const;
            const std::string &getRawValue(void) const;

            int32_t getDefaultValueType(void) const;
            const NestedOptionNameList &getDefaultNestedList(void) const;
            const std::string &getDefaultRawValue(void) const;

            OptionStatus setValue(std::string &&value);
            OptionStatus setValue(NestedOptionNameList &&value);

            OptionStatus setDefaultValue(std::string &&defaultValue);
            OptionStatus setDefaultValue(NestedOptionNameList &&defaultValue);

            OptionStatus resetValue(void);
            OptionStatus setAcceptedValueTypes(int32_t acceptedValueTypes);

            void setValidator(std::unique_ptr<Validator> &&validator) noexcept;
            ValidatorMessage validate(const ValidatorContext &context);
            const ValidatorMessage &getLastValidatorMessage(void) const noexcept;

            const OptionEditor &getOptionEditor(void) const noexcept;
            OptionStatus setOptionEditor(OptionEditor &&optionEditor);

        protected:
            OptionIdentifier mIdentifier;
            OptionValue mValue;
            std::unique_ptr<Validator> mValidator;
            ValidatorMessage mLastValidatorMessage;
            OptionEditor mOptionEditor;
    };
}

// https://fmt.dev/latest/api.html#format-api
template <> struct fmt::formatter<Fidgety::OptionIdentifier> : fmt::formatter<std::string> {
    /*
    auto parse(fmt::format_parse_context &prsCtx) -> decltype(prsCtx.begin()) {
        // only "{}" allowed as the formatter
        auto it = prsCtx.begin(), end = prsCtx.end();
        if (it != end && *it != '}') {
            throw format_error("invalid format (fmt::formatter<Fidgety::OptionIdentifier>");
        }
        return it;
    }
    */

    auto format(
        const Fidgety::OptionIdentifier &oi,
        fmt::format_context &fmtCtx
    ) -> decltype(fmtCtx.out()) {
        // literally just copy paste the string
        return fmt::formatter<std::string>::format(oi.getPath(), fmtCtx);
    }
};

#endif
