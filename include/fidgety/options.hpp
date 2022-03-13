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

#   include <iostream>
#   include <map>
#   include <memory>
#   include <string>
#   include <vector>
#   include <fidgety/exception.hpp>

namespace Fidgety {
    /*
     * Start Forward Declarations
     */

    enum class ValidatorMessageType;
    class ValidatorMessage;
    class ValidatorContext;
    class Validator;
    enum class OptionStatus;
    class OptionException;
    class OptionValue;
    enum class OptionEditorType;
    class OptionEditor;
    class Option;

    /*
     * End Forward Declarations
     */

    typedef std::string OptionIdentifier;
    typedef std::map<OptionIdentifier, std::shared_ptr<Option>> ValidatorContextInner;
    typedef std::vector<std::shared_ptr<Option>> NestedOptionList;

    enum class ValidatorMessageType : int32_t {
        Valid = 0,
        Problematic = 1,
        Invalid = 2,
        Unexpected = 3
    };

    class ValidatorMessage {
        public:
            ValidatorMessage(
                ValidatorMessageType messageType = ValidatorMessageType::Valid,
                std::string &&message = ""
            );
            ~ValidatorMessage(void);

            const std::string &getMessage(void) const noexcept;
            ValidatorMessageType getMessageType(void) const noexcept;
            std::string fullMessage(void) const;

            friend std::ostream &operator<<(std::ostream &stream, const ValidatorMessage &message);
        
        protected:
            std::string mMessage;
            ValidatorMessageType mType;
    };

    class ValidatorContext {
        public:
            ValidatorContext(void);
            ValidatorContext(ValidatorContextInner &&map);

            bool optionExists(const OptionIdentifier &identifier) const noexcept;
            const Option &getOption(const OptionIdentifier &identifier) const;

            const ValidatorContextInner &getInnerMap(void) const noexcept;
        
        protected:
            ValidatorContextInner mMap;
    };

    class Validator {
        public:
            Validator(void);
            
            virtual ValidatorMessage validate(
                const Option &option,
                const ValidatorContext &context
            );

        protected:
    };

    enum class OptionStatus : int32_t {
        Ok = 0,
        InvalidValueType = 1,
        IncompatibleOptionEditor = 2,
        NotFound = 3
    };

    class OptionException : public Exception {
        public:
            using Exception::Exception;
            std::string codeAsErrorType(void) const;

        protected:
            const char *getSimpleWhat(void) const noexcept;
    };

    union _OptionValueInner {
        NestedOptionList nestedList;
        std::string rawValue;

        ~_OptionValueInner(void);
    };

    struct OptionValueInner {
        int32_t valueType;
        _OptionValueInner value;

        OptionValueInner(void);
        OptionValueInner(const char *rawValue);
        OptionValueInner(std::string &&rawValue);
        OptionValueInner(NestedOptionList &&nestedList);
        ~OptionValueInner(void);

        OptionValueInner(const OptionValueInner &other);
        OptionValueInner(OptionValueInner &&other);
        OptionValueInner &operator=(const OptionValueInner &other);
        OptionValueInner &operator=(OptionValueInner &&other);

        void _deleteCurrentValue(void);

        const NestedOptionList &getNestedList(void) const;
        const std::string &getRawValue(void) const;
    };

    namespace OptionValueType {
        const int32_t NESTED_LIST = 1;
        const int32_t RAW_VALUE = 2;

        bool isValid(const int32_t valueType);
    }

    class OptionValue {
        public:
            OptionValue(int32_t acceptedValueType);
            OptionValue(
                OptionValueInner &&defaultValue,
                int32_t acceptedValueTypes
            );

            int32_t getValueType(void) const noexcept;
            const OptionValueInner &getValue(void) const noexcept;
            OptionStatus setValue(OptionValueInner &&value);

            int32_t getDefaultValueType(void) const noexcept;
            const OptionValueInner &getDefaultValue(void) const noexcept;
            OptionStatus setDefaultValue(OptionValueInner &&defaultValue);

            void resetValue(void);
            void setAcceptedValueTypes(int32_t acceptedValueTypes);

        protected:
            int32_t mAcceptedValueTypes;
            OptionValueInner mValue;
            OptionValueInner mDefault;
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
                Validator &&validator,
                int32_t acceptedValueTypes
            ) noexcept;
            Option(
                OptionIdentifier identifier,
                OptionEditor &&optionEditor,
                Validator &&validator,
                OptionValue &&value
            );
            ~Option(void);

            // Option(const Option &option) = delete;
            // Option(Option &&option) = default;

            const OptionIdentifier &getIdentifier(void) const noexcept;
            const OptionValue &getOptionValue(void) const noexcept;
            const OptionValueInner &getValue(void) const noexcept;
            const OptionValueInner &getDefaultValue(void) const noexcept;

            int32_t getValueType(void) const;
            const NestedOptionList &getNestedList(void) const;
            const std::string &getRawValue(void) const;

            int32_t getDefaultValueType(void) const;
            const NestedOptionList &getDefaultNestedList(void) const;
            const std::string &getDefaultRawValue(void) const;

            OptionStatus setValue(std::string &&value);
            OptionStatus setValue(NestedOptionList &&value);

            OptionStatus setDefaultValue(std::string &&defaultValue);
            OptionStatus setDefaultValue(NestedOptionList &&defaultValue);

            OptionStatus resetValue(void);
            OptionStatus setAcceptedValueTypes(int32_t acceptedValueTypes);

            void setValidator(Validator validator) noexcept;
            ValidatorMessage validate(const ValidatorContext &context);
            const ValidatorMessage &getLastValidatorMessage(void) const noexcept;

            const OptionEditor &getOptionEditor(void) const noexcept;
            OptionStatus setOptionEditor(OptionEditor &&optionEditor);

        protected:
            OptionIdentifier mIdentifier;
            OptionValue mValue;
            Validator mValidator;
            ValidatorMessage mLastValidatorMessage;
            OptionEditor mOptionEditor;
    };
}

#endif
