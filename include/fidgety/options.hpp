/**
 * @file include/fidgety/options.hpp
 * @author RenoirTan
 * @brief Header file for the options that appear when Fidgety is editing a
 * config file.
 * @version 0.1
 * @date 2022-03-02
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef FIDGETY_OPTIONS_HPP
#   define FIDGETY_OPTIONS_HPP

#   include <map>
#   include <memory>
#   include <string>
#   include <vector>
#   include <fidgety/exception.hpp>

namespace Fidgety {
    enum class ValidatorMessageType : int32_t {
        Valid = 0,
        Problematic = 1,
        Invalid = 2,
        Unexpected = 3
    };

    class ValidatorMessage {
        public:
            ValidatorMessage(
                std::string &&message = "",
                ValidatorMessageType messageType = ValidatorMessageType::Valid
            );
            ~ValidatorMessage(void);

            const std::string &getMessage(void) const noexcept;
            ValidatorMessageType getMessageType(void) const noexcept;
        
        protected:
            std::string mMessage;
            ValidatorMessageType mType;
    };

    typedef std::map<OptionIdentifier, std::unique_ptr<Option>> ValidatorContextInner;

    class ValidatorContext {
        public:
            ValidatorContext(void);
            ValidatorContext(ValidatorContextInner &&map);

            bool optionExists(const OptionIdentifier &identifier) const noexcept;
            const Option &getOption(const OptionIdentifier &identifier) const;

            const ValidatorContextInner &getInnerMap(void) const;
        
        protected:
            ValidatorContextInner mMap;
    };

    typedef ValidatorMessage (*Validator)(const Option &, const ValidatorContext &context);
    typedef std::string OptionIdentifier;

    enum class OptionStatus : int32_t {
        Ok = 0,
        InvalidValueType = 1,
        IncompatibleOptionEditor = 2
    };

    class OptionException : public Exception {
        public:
            using Exception::Exception;
            std::string codeAsErrorType(void) const;

        protected:
            const char *getSimpleWhat(void) const noexcept;
    };

    class OptionValue {
        public:
            OptionValue(int32_t acceptedValueType = OptionValueType::RAW_VALUE);
            OptionValue(
                OptionValueInner &&defaultValue,
                int32_t acceptedValueTypes = OptionValueType::RAW_VALUE
            );

            int32_t getValueType(void) const noexcept;
            const OptionValueInner &getValue(void) const noexcept;
            OptionException setValue(const OptionValueInner &value);

            int32_t getDefaultValueType(void) const;
            const OptionValueInner &getValueDefault(void) const noexcept;
            OptionException setDefaultValue(const OptionValueInner &defaultValue);

            void resetValue(void);
            void setAcceptedValueType(int32_t acceptedValueType);

        protected:
            int32_t mAcceptedValueTypes;
            OptionValueInner mValue;
            OptionValueInner mDefault;
    };

    struct OptionValueInner {
        int32_t valueType;
        _OptionValueInner value;

        OptionValueInner(void);
        OptionValueInner(const char *rawValue);
        OptionValueInner(std::string &&rawValue);
        OptionValueInner(std::vector<Option> &&nestedList);
    };

    namespace OptionValueType {
        const int32_t NESTED_LIST = 1;
        const int32_t RAW_VALUE = 2;
    }

    union _OptionValueInner {
        std::vector<Option> nestedList;
        std::string rawValue;
    };

    enum class OptionEditorType : int32_t {
        Blanked = 0,
        TextEntry = 1,
        Toggle = 2,
        Slider = 3,
        Dropdown = 4,
        Options = 5,
        Checkboxes = 6
    };

    class OptionEditor {
        public:
            OptionEditor(OptionEditorType oet, std::map<std::string, std::string> &&constraints);

        protected:
            OptionEditorType mEditorType;
            std::map<std::string, std::string> mConstraints;
    };

    class Option {
        public:
            Option(
                OptionIdentifier identifier,
                OptionEditor &&optionEditor,
                Validator validator = nullptr
            ) noexcept;
            Option(
                OptionIdentifier identifier,
                OptionEditor &&optionEditor,
                OptionValue &&value,
                Validator validator = nullptr
            );
            ~Option(void);

            const OptionIdentifier &getIdentifier(void) const noexcept;
            const OptionValue &getOptionValue(void) const noexcept;
            const OptionValueInner &getValue(void) const noexcept;
            const OptionValueInner &getDefaultValue(void) const noexcept;

            int32_t getValueType(void) const;
            const std::vector<Option> &getNestedList(void) const;
            const std::string &getRawValue(void) const;

            int32_t getDefaultValueType(void) const;
            const std::vector<Option> &getDefaultNestedList(void) const;
            const std::string &getDefaultRawValue(void) const;

            OptionException setValue(std::string &&value);
            OptionException setValue(std::vector<Option> &&value);

            OptionException setDefaultValue(std::string &&defaultValue);
            OptionException setDefaultValue(std::vector<Option> &&defaultValue);

            OptionException resetValue(void);
            OptionException setAcceptedValueType(int32_t acceptedValueType);

            void setValidator(Validator validator = nullptr) noexcept;
            ValidatorMessage validate(const ValidatorContext &context);
            const ValidatorMessage &getLastValidatorMessage(void) const noexcept;

            const OptionEditor &getOptionEditor(void) const noexcept;
            OptionException setOptionEditor(OptionEditor &&optionEditor);

        protected:
            OptionIdentifier mIdentifier;
            OptionValue mValue;
            Validator mValidator;
            ValidatorMessage mLastValidatorMessage;
            OptionEditor mOptionEditor;
    };
}

#endif
