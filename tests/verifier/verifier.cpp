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

class SimpleValidatorContextCreator : public virtual ValidatorContextCreator {
    public:
        SimpleValidatorContextCreator(void) {
            spdlog::trace("Creating a SimpleValidatorContextCreator.");
        }

        ValidatorContext createContext(
            const VerifierManagedOptionList &verifier,
            const OptionIdentifier &identifier
        ) {
            spdlog::trace(
                "Creating Fidgety::ValidatorContext from SimpleValidatorContextCreator for '{0}'",
                identifier
            );
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

class SimpleValidator : public virtual Validator {
    public:
        SimpleValidator(void) {
            spdlog::trace("Creating a SimpleValidator.");
        }

        ValidatorMessage validate(const Option &option, const ValidatorContext &context) {
            const OptionIdentifier &optionIdentifier = option.getIdentifier();
            spdlog::trace("Validating '{0}' in SimpleValidator.");
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
                return ValidatorMessage(ValidatorMessageType::Valid, "Values match!");
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
#define ASSIGN_OPT(vmol, id, val) \
    vmol[id] = std::make_shared<Option>( \
        id, \
        OptionEditor(OptionEditorType::TextEntry, CONS()), \
        SimpleValidator(), \
        OptionValue(val, OptionValueType::RAW_VALUE) \
    ) \

    VerifierManagedOptionList vmol;
    ASSIGN_OPT(vmol, "A", "1");
    ASSIGN_OPT(vmol, "B", "4");
    ASSIGN_OPT(vmol, "C", "2");
    ASSIGN_OPT(vmol, "D", "3");
#undef ASSIGN_OPT
#undef CONS
    return vmol;
}

TEST(VerifierVerifier, CreateVerifier) {
    spdlog::set_level(spdlog::level::trace);
    Verifier verifier(createOptions(), SimpleValidatorContextCreator());
}

TEST(VerifierVerifier, ValidateOriginal) {
    spdlog::set_level(spdlog::level::trace);
    Verifier verifier(createOptions(), SimpleValidatorContextCreator());
    VerifierOptionLock lock = verifier.getLock("A");
    ValidatorMessage message = lock.release();
    EXPECT_EQ(message.fullMessage(), "Valid: Values Match!");
    ASSERT_TRUE(lock.isReleased());
}

TEST(VerifierVerifier, ValidateChanged) {
    spdlog::set_level(spdlog::level::trace);
    Verifier verifier(createOptions(), SimpleValidatorContextCreator());
    VerifierOptionLock lock = verifier.getLock("A");
    lock.getMutOption().setValue("25");
    ValidatorMessage message = lock.release();
    EXPECT_EQ(message.fullMessage(), "Invalid: A+B does not match C+D! A+B = 29 but C+D = 5");
    ASSERT_TRUE(lock.isReleased());
}
