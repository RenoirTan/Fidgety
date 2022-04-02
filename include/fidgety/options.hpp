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
#   include <iterator>
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
    class NewOptionIdentifier;
    enum class OptionStatus;
    class OptionException;
    class OptionValue;
    enum class OptionEditorType;
    class OptionEditor;
    class Option;

    /*
     * End Forward Declarations
     */

    using OptionIdentifier = std::string;
    using OptionName = std::string;
    using ValidatorContextInner = std::map<OptionIdentifier, std::shared_ptr<Option>>;
    using NestedOptionList = std::vector<std::shared_ptr<Option>>;
    using OptionIdentifierList = std::vector<OptionIdentifier>;
    using InnerOptionsNameList = std::vector<OptionName>;

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

            virtual Validator *clone(void) const;

        protected:
    };

    // Always use char array just in case
    constexpr char OPTION_NAME_DELIMITER[] = ".";

    class NewOptionIdentifier {
        public:

            struct Iterator {
                using iterator_category = std::random_access_iterator_tag;
                using difference_type = std::ptrdiff_t;
                using value_type = const OptionName;
                using pointer = value_type*;
                using reference = value_type&;

                static const int32_t VALID = 0;
                static const int32_t OUT_OF_BOUNDS = 1;

                // dereference

                reference operator*(void);
                pointer operator->(void);
                reference operator[](difference_type steps);

                // traversal

                Iterator &operator+=(difference_type steps);
                Iterator &operator-=(difference_type steps);

                Iterator operator+(difference_type steps) const;
                Iterator operator-(difference_type steps) const;

                Iterator &operator++(void); // ++it
                Iterator operator++(int); // it++
                Iterator &operator--(void); // --it
                Iterator operator--(int); // it--

                // comparison

#define _FIDGETY_OIIT_CMPEX(cmpOp) \
    ( \
        a.identifier == b.identifier && \
        ( \
            (a.state == b.state) && \
            ( \
                (a.state == NewOptionIdentifier::Iterator::VALID && a.index cmpOp b.index) || \
                a.state == NewOptionIdentifier::Iterator::OUT_OF_BOUNDS \
            ) \
        ) \
    ) \

                friend bool operator==(const Iterator &a, const Iterator &b) {
                    return _FIDGETY_OIIT_CMPEX(==);
                }

                friend bool operator!=(const Iterator &a, const Iterator &b) {
                    return !(a == b);
                }

                friend bool operator<(const Iterator &a, const Iterator &b) {
                    return _FIDGETY_OIIT_CMPEX(<);
                }

                friend bool operator>(const Iterator &a, const Iterator &b) {
                    return _FIDGETY_OIIT_CMPEX(>);
                }

                friend bool operator<=(const Iterator &a, const Iterator &b) {
                    return _FIDGETY_OIIT_CMPEX(<=);
                }

                friend bool operator>=(const Iterator &a, const Iterator &b) {
                    return _FIDGETY_OIIT_CMPEX(>=);
                }

#undef _FIDGETY_OIIT_CMPEX

                const NewOptionIdentifier *identifier;
                size_t index;
                OptionName name;
                int32_t state;
            };

            NewOptionIdentifier(const std::string &path);
            NewOptionIdentifier(std::string &&path);

            NewOptionIdentifier &operator=(const std::string &path);
            NewOptionIdentifier &operator=(std::string &&path);

            bool isValid(void) const noexcept;

            operator std::string(void) const;
            const std::string &getPath(void) const;

            size_t depth(void) const;
            std::vector<OptionName> split(void) const;
            Iterator at(size_t index) const;
            Iterator begin(void) const;
            Iterator end(void) const;

        protected:
            std::string mPath;
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

#endif
