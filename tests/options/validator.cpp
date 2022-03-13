#include <string>
#include <fidgety/options.hpp>
#include <gtest/gtest.h>
#include "spdlog/spdlog.h"
#include "dummies.hpp"

using namespace Fidgety;

TEST(OptionsValidator, Validator) {
    Validator validator;
    OptionEditor editor(OptionEditorType::Blanked, std::map<std::string, std::string>());
    OptionValue value("value", OptionValueType::RAW_VALUE);
    Option option("dummy", std::move(editor), std::move(validator), std::move(value));

    ValidatorContextInner vci = makeValidatorContextInner(10);
    ValidatorContext context(std::move(vci));
    
    ValidatorMessage message = option.validate(context);
    EXPECT_EQ(message.fullMessage(), "Valid: Ok");
}
