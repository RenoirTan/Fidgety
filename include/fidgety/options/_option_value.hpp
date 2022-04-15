/**
 * @file include/fidgety/options/_option_value.hpp
 * @author RenoirTan
 * @brief Fidgety::OptionValue stores the value of the current setting and its
 * default value.
 * @version 0.1
 * @date 2022-04-15
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef _FIDGETY_OPTIONS_OPTION_VALUE_HPP
#   define _FIDGETY_OPTIONS_OPTION_VALUE_HPP

#   include "_fwd.hpp"

namespace Fidgety {
    union _OptionValueInner {
        NestedOptionNameList nestedList;
        std::string rawValue;

        ~_OptionValueInner(void);
    };

    struct OptionValueInner {
        int32_t valueType;
        _OptionValueInner value;

        OptionValueInner(void);
        OptionValueInner(const char *rawValue);
        OptionValueInner(std::string &&rawValue);
        OptionValueInner(NestedOptionNameList &&nestedList);
        ~OptionValueInner(void);

        OptionValueInner(const OptionValueInner &other);
        OptionValueInner(OptionValueInner &&other);
        OptionValueInner &operator=(const OptionValueInner &other);
        OptionValueInner &operator=(OptionValueInner &&other);

        void _deleteCurrentValue(void);

        const NestedOptionNameList &getNestedList(void) const;
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
}


#endif
