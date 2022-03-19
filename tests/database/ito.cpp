/**
 * @file tests/database/ito.cpp
 * @author RenoirTan
 * @brief Tests for Ito.
 * @version 0.1
 * @date 2022-03-19
 * 
 * @copyright Copyright (c) 2022
 */

#include <fidgety/config.h>

#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <fidgety/_tests.hpp>
#include <fidgety/database/ito_database.hpp>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

using namespace Fidgety;

nlohmann::json generateJsonIto0(void) noexcept {
    nlohmann::json data;

    data["size"] = "large";
    data["color"] = "red";
    data["quality"] = 80;

    return data;
}

class Ito0Validator : public Validator {
    public:
        ValidatorMessage validate(
            const Option &option,
            const ValidatorContext &context
        ) {
            const OptionIdentifier &identifier = option.getIdentifier();
            const std::string &value = option.getRawValue();
            if (identifier == "size") {
                if (
                    value == "extra small" ||
                    value == "small" ||
                    value == "medium" ||
                    value == "large" ||
                    value == "extra large"
                ) {
                    return ValidatorMessage(ValidatorMessageType::Valid, "valid size");
                } else {
                    return ValidatorMessage(ValidatorMessageType::Invalid, "invalid size");
                }
            } else if (identifier == "color") {
                if (value.size() > 20) {
                    return ValidatorMessage(ValidatorMessageType::Valid, "valid color, i think");
                } else {
                    return ValidatorMessage(ValidatorMessageType::Invalid, "too long");
                }
            } else if (identifier == "quality") {
                int32_t quality = std::atoi(value.c_str());
                if (0 <= quality && quality <= 100) {
                    return ValidatorMessage(ValidatorMessageType::Valid, "valid quality");
                } else {
                    return ValidatorMessage(
                        ValidatorMessageType::Unexpected,
                        "quality outside range"
                    );
                }
            } else {
                return ValidatorMessage(ValidatorMessageType::Unexpected, "what");
            }
        }

        Ito0Validator *clone(void) const override {
            return new Ito0Validator();
        }

    protected:
};

TEST(DatabaseIto, GetCandidateIto) {
    _FIDGETY_INIT_TEST();

    std::vector<std::string> appNames = {"ito_0"};
    std::vector<std::string> allowedFileExts = {".json"};
    std::vector<std::string> searchPaths = {"../../../resources/tests/database"};
    std::vector<std::string> itoCandidates = getCandidateIto(
        appNames,
        allowedFileExts,
        searchPaths,
        true
    );

    ASSERT_FALSE(itoCandidates.empty());
    const std::string &candidate = itoCandidates[0];
    std::ifstream itoFile;
    itoFile.open(candidate);
    ASSERT_TRUE(itoFile.good());
    nlohmann::json ito;
    itoFile >> ito;
    nlohmann::json intermediate = generateJsonIto0();

    ItoJson itoJson(ito);
    Ito0Validator validator;

    VerifierManagedOptionList vmol = itoJson.toVmol(intermediate, validator);

#define CHECK_OPTS(key, value, defaultValue) {             \
    const auto &option = vmol[key];                        \
    EXPECT_EQ(option->getRawValue(), value);               \
    EXPECT_EQ(option->getDefaultRawValue(), defaultValue); \
}                                                          \

    CHECK_OPTS("size", "large", "medium");
    CHECK_OPTS("color", "red", "blue");
    CHECK_OPTS("quality", "80", "50");

#undef CHECK_OPTS
}
