/**
 * @file tests/verifier/verifier.cpp
 * @author RenoirTan
 * @brief Test for the verifier in Fidgety. This test suites simulates an app
 * with 4 inputs A, B, C, D which are related by this equation:
 * A+B=C+D
 * The verifier classes in this suite have to ensure that this equation is
 * upheld at all times.
 * @version 0.1
 * @date 2022-03-14
 * 
 * @copyright Copyright (c) 2022
 */

#include <cstdlib>
#include <fidgety/verifier.hpp>
#include <fmt/core.h>
#include <gtest/gtest.h>
#include "spdlog/spdlog.h"

using namespace Fidgety;

class SimpleValidatorContextCreator : public ValidatorContextCreator {
    public:
        ValidatorContext createContext(
            const VerifierManagedOptionList &verifier,
            const OptionIdentifier &identifier
        ) {
            ValidatorContextInner vci;
            for (auto option = verifier.cbegin(); option != verifier.cend(); ++option) {
                if (option->first == identifier)
                    continue;
                else
                    vci[identifier] = option->second;
            }
            return ValidatorContext(std::move(vci));
        }
};

class SimpleValidator : public Validator {
    public:
        ValidatorMessage validate(const Option &option, const ValidatorContext &context) {
            const OptionIdentifier &optionIdentifier = option.getIdentifier();
            const int64_t optionValue = std::atol(option.getRawValue().c_str());
            std::map<OptionIdentifier, int64_t> values;
            values[optionIdentifier] = optionValue;
            for (
                auto other = context.getInnerMap().cbegin();
                other != context.getInnerMap().cend();
                ++other
            ) {
                values[other->first] = std::atol(other->second->getRawValue().c_str());
            }
            int64_t left = values["A"] + values["B"], right = values["C"] + values["D"];
            if (left == right) {
                return ValidatorMessage(ValidatorMessageType::Valid, "Ok");
            } else {
                return ValidatorMessage(
                    ValidatorMessageType::Invalid,
                    fmt::format("A+B does not match C+D! A+B = {0} but C+D = {1}", left, right)
                );
            }
        }
};

VerifierManagedOptionList createOptions(void) {
#define CONS std::map<std::string, std::string>
    VerifierManagedOptionList vmol;
    vmol["A"] = std::make_shared<Option>(
        "A",
        OptionEditor(OptionEditorType::TextEntry, CONS()),
        Validator(),
        OptionValue("1", OptionValueType::RAW_VALUE)
    );
    vmol["B"] = std::make_shared<Option>(
        "B",
        OptionEditor(OptionEditorType::TextEntry, CONS()),
        Validator(),
        OptionValue("4", OptionValueType::RAW_VALUE)
    );
    vmol["C"] = std::make_shared<Option>(
        "C",
        OptionEditor(OptionEditorType::TextEntry, CONS()),
        Validator(),
        OptionValue("2", OptionValueType::RAW_VALUE)
    );
    vmol["D"] = std::make_shared<Option>(
        "D",
        OptionEditor(OptionEditorType::TextEntry, CONS()),
        Validator(),
        OptionValue("3", OptionValueType::RAW_VALUE)
    );
#undef CONS
    return vmol;
}

TEST(VerifierVerifier, CreateVerifier) {
    Verifier verifier(createOptions(), SimpleValidatorContextCreator());
}
