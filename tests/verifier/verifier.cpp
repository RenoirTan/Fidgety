/**
 * @file tests/verifier/verifier.cpp
 * @author RenoirTan
 * @brief Test for the verifier in Fidgety. This test suite simulates an app
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
#include <fidgety/_tests.hpp>
#include <fmt/core.h>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

using namespace Fidgety;

#define CONS std::map<std::string, std::string>
#define ASSIGN_OPT(vmol, id, val)                                \
    vmol[id] = std::make_shared<Option>(                         \
        id,                                                      \
        OptionEditor(OptionEditorType::TextEntry, CONS()),       \
        std::unique_ptr<SimpleValidator>(new SimpleValidator()), \
        OptionValue(val, OptionValueType::RAW_VALUE)             \
    )

#define ASSIGN_OPT_NESTED(vmol, id, ...) {                         \
    NestedOptionNameList noml({__VA_ARGS__});                      \
    vmol[id] = std::make_shared<Option>(                           \
        id,                                                        \
        OptionEditor(OptionEditorType::TextEntry, CONS()),         \
        std::unique_ptr<SimpleValidator>(new SimpleValidator()),   \
        OptionValue(std::move(noml), OptionValueType::NESTED_LIST) \
    );                                                             \
}                                                                  \

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
            for (const auto &option : verifier) {
                if (option.first == identifier) {
                    continue;
                } else {
                    // other->first: Identifier of option in context
                    // other->second: Reference to the option in context
                    vci[option.first] = option.second;
                }
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
            spdlog::trace("Validating '{0}' in SimpleValidator.", optionIdentifier);
            const int64_t optionValue = std::atol(option.getRawValue().c_str());
            std::map<OptionIdentifier, int64_t> values;
            values[optionIdentifier] = optionValue;
            spdlog::trace("Size of context: {0}", context.getInnerMap().size());
            const Fidgety::ValidatorContextInner &innerMap = context.getInnerMap();
            for (const auto &other : innerMap) {
                // other->first: Identifier of option in context
                // other->second: Reference to the option in context
                values[other.first] = std::atol(other.second->getRawValue().c_str());
            }
            int64_t left = values["A"] + values["B"];
            int64_t right = values["C"] + values["D"];
            if (left == right) {
                return ValidatorMessage(ValidatorMessageType::Valid, "Values match!");
            } else {
                return ValidatorMessage(
                    ValidatorMessageType::Invalid,
                    fmt::format("A+B does not match C+D! A+B = {0} but C+D = {1}", left, right)
                );
            }
        }

        SimpleValidator *clone(void) const override {
            return new SimpleValidator();
        }
};

VerifierManagedOptionList createOptions(void) {
    VerifierManagedOptionList vmol;
    ASSIGN_OPT(vmol, "A", "1");
    ASSIGN_OPT(vmol, "B", "4");
    ASSIGN_OPT(vmol, "C", "2");
    ASSIGN_OPT(vmol, "D", "3");
    return vmol;
}

TEST(VerifierVerifier, CreateVerifier) {
    _FIDGETY_INIT_TEST();
    std::unique_ptr<ValidatorContextCreator> vcc(new SimpleValidatorContextCreator());
    Verifier verifier(createOptions(), std::move(vcc));
}

TEST(VerifierVerifier, ValidateOriginal) {
    _FIDGETY_INIT_TEST();
    std::unique_ptr<ValidatorContextCreator> vcc(new SimpleValidatorContextCreator());
    Verifier verifier(createOptions(), std::move(vcc));
    VerifierOptionLock lock = verifier.getLock("A");
    EXPECT_TRUE(verifier.isOptionLocked("A"));
    ValidatorMessage message = lock.release();
    EXPECT_EQ(message.fullMessage(), "Valid: Values match!");
    ASSERT_TRUE(lock.isReleased());
    EXPECT_FALSE(verifier.isOptionLocked("A"));
}

TEST(VerifierVerifier, ValidateChanged) {
    _FIDGETY_INIT_TEST();
    std::unique_ptr<ValidatorContextCreator> vcc(new SimpleValidatorContextCreator());
    Verifier verifier(createOptions(), std::move(vcc));
    VerifierOptionLock lock = verifier.getLock("A");
    EXPECT_TRUE(verifier.isOptionLocked("A"));
    lock.getMutOption().setValue("25");
    ValidatorMessage message = lock.release();
    EXPECT_EQ(message.fullMessage(), "Invalid: A+B does not match C+D! A+B = 29 but C+D = 5");
    ASSERT_TRUE(lock.isReleased());
    EXPECT_FALSE(verifier.isOptionLocked("A"));
}

TEST(VerifierVerifier, OverwriteOptions) {
    _FIDGETY_INIT_TEST();
    std::unique_ptr<ValidatorContextCreator> vcc(new SimpleValidatorContextCreator());
    Verifier verifier(createOptions(), std::move(vcc));
    EXPECT_TRUE(verifier.optionExists("A"));
    EXPECT_TRUE(verifier.optionExists("B"));
    EXPECT_TRUE(verifier.optionExists("C"));
    EXPECT_TRUE(verifier.optionExists("D"));

    VerifierManagedOptionList freshVmol;
    ASSIGN_OPT(freshVmol, "E", "10");
    ASSIGN_OPT(freshVmol, "F", "40");
    ASSIGN_OPT(freshVmol, "G", "20");
    ASSIGN_OPT(freshVmol, "H", "30");
    ASSERT_TRUE(verifier.canBeOverwritten());
    ASSERT_EQ(verifier.overwriteOptions(std::move(freshVmol)), VerifierStatus::Ok);
}

TEST(VerifierVerifier, PurgeOrphans) {
    _FIDGETY_INIT_TEST();
    std::unique_ptr<ValidatorContextCreator> vcc(new SimpleValidatorContextCreator());
    VerifierManagedOptionList vmol;
    ASSIGN_OPT_NESTED(vmol, "A", "B");
    ASSIGN_OPT_NESTED(vmol, "A.B", "F", "G"); // g has no value
    ASSIGN_OPT(vmol, "A.B.F", "a.b.f:raw");
    ASSIGN_OPT(vmol, "A.B.G", "a.b.g:raw");
    ASSIGN_OPT_NESTED(vmol, "A.C", "D", "E");
    ASSIGN_OPT(vmol, "A.C.D", "a.c.d:raw");
    ASSIGN_OPT(vmol, "A.C.E", "a.c.e:raw");
    Verifier verifier(std::move(vmol), std::move(vcc));
    EXPECT_TRUE(verifier.optionExists("A"));
    EXPECT_TRUE(verifier.optionExists("A.B"));
    EXPECT_TRUE(verifier.optionExists("A.B.F"));
    EXPECT_TRUE(verifier.optionExists("A.B.G"));
    EXPECT_TRUE(verifier.optionExists("A.C"));
    EXPECT_TRUE(verifier.optionExists("A.C.D"));
    EXPECT_TRUE(verifier.optionExists("A.C.E"));

    ASSERT_EQ(verifier.purgeOrphanedOptions(), VerifierStatus::Ok);
    ASSERT_EQ(verifier.numberOfLocks(), 0);
    EXPECT_TRUE(verifier.optionExists("A"));
    EXPECT_TRUE(verifier.optionExists("A.B"));
    EXPECT_TRUE(verifier.optionExists("A.B.F"));
    EXPECT_TRUE(verifier.optionExists("A.B.G"));
    EXPECT_FALSE(verifier.optionExists("A.C"));
    EXPECT_FALSE(verifier.optionExists("A.C.D"));
    EXPECT_FALSE(verifier.optionExists("A.C.E"));
}
