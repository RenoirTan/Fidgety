/**
 * @file tests/selector/selector_validator.cpp
 * @author RenoirTan
 * @brief Custom validator for selector test.
 * @version 0.1
 * @date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 */

#include <string>
#include <fidgety/_tests.hpp>
#include <fidgety/_utils.hpp>
#include <fidgety/extensions.hpp>
#include <fidgety/options.hpp>

using namespace Fidgety;

static inline bool _isPow2(int64_t n) {
    // https://stackoverflow.com/a/108360
    return (n>0 && (n&(n-1)) == 0);
}

static inline int64_t _log2(int64_t n) {
    // https://stackoverflow.com/a/24748637
    #define S(k) if (n >= (UINT64_C(1) << k)) { i += k; n >>= k; }

    int i = -(n <= 0); S(32); S(16); S(8); S(4); S(2); S(1); return i;

    #undef S
}

class Exp2Validator : public Validator {
    ValidatorMessage validate(const Option &option, const ValidatorContext &context) {
        if (option.getValueType() != OptionValueType::RAW_VALUE) {
            return ValidatorMessage(ValidatorMessageType::Unexpected, "expected a value");
        }
        const std::string &identifier = option.getIdentifier();
        int64_t index = std::stol(identifier);

        std::string svalue = option.getRawValue();
        int64_t ivalue;
        try {
            ivalue = std::stol(svalue);
        } catch (const std::invalid_argument &iae) {
            return ValidatorMessage(ValidatorMessageType::Invalid, "value must be an integer");
        }

        if (!_isPow2(ivalue)) {
            return ValidatorMessage(ValidatorMessageType::Invalid, "value must be a power of 2");
        }

        if (context.optionExists("0")) {
            // Simple way to check if numbers are consecutive powers of 2
            // They don't necessarily have to start with 1 (2^0)

            const Option &zeroeth_option = context.getOption("0");
            if (zeroeth_option.getValueType() != OptionValueType::RAW_VALUE) {
                return ValidatorMessage(
                    ValidatorMessageType::Unexpected,
                    "zeroeth value must be a raw value"
                );
            }
            std::string zsvalue = zeroeth_option.getRawValue();
            int64_t zivalue;
            try {
                zivalue = std::stol(zsvalue);
            } catch (const std::invalid_argument &iae) {
                return ValidatorMessage(
                    ValidatorMessageType::Unexpected,
                    "zeroeth value must be an integer"
                );
            }

            int64_t zlog2res = _log2(zivalue);
            if (zlog2res < 0) {
                return ValidatorMessage(
                    ValidatorMessageType::Unexpected,
                    "zeroeth value must be a power of 2"
                );
            }
            if (index != (_log2(ivalue)-zlog2res)) {
                return ValidatorMessage(ValidatorMessageType::Invalid, "value not in order");
            }
        }

        return ValidatorMessage(ValidatorMessageType::Valid, "all ok");
    }

    Exp2Validator *clone(void) const override {
        return new Exp2Validator();
    }
};

#ifdef __cplusplus

extern "C" {
    FIDGETY_ALLOC(FIDGETY_VALIDATOR_ALLOC_PROT(), Exp2Validator);
    FIDGETY_DELETE(FIDGETY_VALIDATOR_DELETE_PROT());
}

#endif
