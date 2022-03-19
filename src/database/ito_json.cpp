/**
 * @file src/database/ito_json.cpp
 * @author RenoirTan
 * @brief Source file for the Fidgety::ItoJson class.
 * @version 0.1
 * @date 2022-03-19
 * 
 * @copyright Copyright (c) 2022
 */

#include <fidgety/config.h>

#include <algorithm>
#include <map>
#include <boost/algorithm/string.hpp>
#include <spdlog/spdlog.h>
#include <fidgety/_utils.hpp>
#include <fidgety/_utils_json.hpp>
#include <fidgety/database/ito_database.hpp>
#include <fidgety/options.hpp>

using namespace Fidgety;
namespace BoostAl = boost::algorithm;

ItoJson::ItoJson(const nlohmann::json &desc) : mDesc(desc) { }

ItoJson::ItoJson(nlohmann::json &&desc) : mDesc(std::move(desc)) { }

VerifierManagedOptionList ItoJson::toVmol(
    const nlohmann::json &intermediate,
    const Validator &validator
) {
    VerifierManagedOptionList vmol;
    for (const auto &option : intermediate.items()) {
        const OptionIdentifier &identifier = option.key();
        const nlohmann::json &value = option.value();
        std::string svalue;
        if (jsonScalarToString(value, svalue)) {
            FIDGETY_CRITICAL(
                DatabaseException,
                DatabaseStatus::InvalidData,
                "[Fidgety::ItoJson::toVmol] value of '{0}' must be a scalar (for now)",
                identifier
            );
        }

        spdlog::trace("[Fidgety::ItoJson::toVmol] setting up option '{0}'", identifier);

        const auto &descItem = mDesc.find(identifier);
        if (descItem == mDesc.end()) {
            FIDGETY_CRITICAL(
                DatabaseException,
                DatabaseStatus::InvalidData,
                "could not find option '{0}' in Fidgety::ItoJson::mDesc",
                identifier
            );
        }

        // DEFAULT VALUE
        const auto &defaultValueJson = descItem->find("default");
        if (defaultValueJson == descItem->end()) {
            FIDGETY_CRITICAL(
                DatabaseException,
                DatabaseStatus::InvalidData,
                "[Fidgety::ItoJson::toVmol] could not find key: '{0}.default'",
                identifier
            );
        }
        std::string defaultValue;
        if (jsonScalarToString(*defaultValueJson, defaultValue)) {
            FIDGETY_CRITICAL(
                DatabaseException,
                DatabaseStatus::InvalidData,
                "[Fidgety::ItoJson::toVmol] value of '{0}.default' must be a scalar",
                identifier
            );
        }

        // ACCEPTED VALUE TYPES
        const auto &acceptedValueTypesJson = descItem->find("acceptedValueTypes");
        if (acceptedValueTypesJson == descItem->end()) {
            FIDGETY_CRITICAL(
                DatabaseException,
                DatabaseStatus::InvalidData,
                "[Fidgety::ItoJson::toVmol] could not find key: '{0}.acceptedValueTypes'",
                identifier
            );
        }
        int32_t acceptedValueTypes;
        auto avtJst = acceptedValueTypesJson->type();
        if (avtJst == nlohmann::json::value_t::number_integer) {
            acceptedValueTypes = (int64_t) *acceptedValueTypesJson;
        } else if (avtJst == nlohmann::json::value_t::number_unsigned) {
            acceptedValueTypes = (uint64_t) *acceptedValueTypesJson;
        } else {
            FIDGETY_CRITICAL(
                DatabaseException,
                DatabaseStatus::InvalidData,
                "[Fidgety::ItoJson::toVmol] value of '{0}.acceptedValueTypes' must be an integer",
                identifier
            );
        }

        // OPTION EDITOR
        std::string editorType;
        std::map<std::string, std::string> editorConstraints;
        const auto &editorJson = descItem->find("editor");
        if (editorJson == descItem->end()) {
            FIDGETY_CRITICAL(
                DatabaseException,
                DatabaseStatus::InvalidData,
                "[Fidgety::ItoJson::toVmol] could not find key: '{0}.editor'",
                identifier
            );
        }
        auto editorJst = editorJson->type();
        if (editorJst == nlohmann::json::value_t::string) {
            editorType = (std::string) *editorJson;
        } else if (editorJst == nlohmann::json::value_t::object) {
            const auto &editorTypeJson = editorJson->find("type");
            if (editorTypeJson == editorJson->end()) {
                FIDGETY_CRITICAL(
                    DatabaseException,
                    DatabaseStatus::InvalidData,
                    "[Fidgety::ItoJson::toVmol] could not find key: '{0}.editor.type'",
                    identifier
                );
            }
            if (editorTypeJson->type() != nlohmann::json::value_t::string) {
                FIDGETY_CRITICAL(
                    DatabaseException,
                    DatabaseStatus::InvalidData,
                    "[Fidgety::ItoJson::toVmol] '{0}.editor.type' must be a string",
                    identifier
                );
            }
            editorType = (std::string) *editorTypeJson;

            const auto &editorConstraintsJson = editorJson->find("constraints");
            if (editorConstraintsJson != editorJson->end()) {
                auto editorConstraintsJst = editorConstraintsJson->type();
                if (editorConstraintsJst == nlohmann::json::value_t::array) {
                    size_t index = 0;
                    for (const auto &constraint : *editorConstraintsJson) {
                        std::string scalar;
                        if (jsonScalarToString(constraint, scalar)) {
                            FIDGETY_CRITICAL(
                                DatabaseException,
                                DatabaseStatus::InvalidData,
                                "[Fidgety::ItoJson::toVmol] "
                                "'{0}.editor.constraints.{1} is not scalar",
                                identifier,
                                index
                            );
                        }
                        editorConstraints[std::to_string(index)] = scalar;
                        ++index;
                    }
                } else if (editorConstraintsJst == nlohmann::json::value_t::object) {
                    for (const auto &constraint : editorConstraintsJson->items()) {
                        const std::string &key = constraint.key();
                        const auto &value = constraint.value();
                        std::string valueScalar;
                        if (jsonScalarToString(value, valueScalar)) {
                            FIDGETY_CRITICAL(
                                DatabaseException,
                                DatabaseStatus::InvalidData,
                                "[Fidgety::ItoJson::toVmol] "
                                "'{0}.editor.constraints.{1} is not scalar",
                                identifier,
                                key
                            );
                        }
                        editorConstraints[key] = valueScalar;
                    }
                } else {
                    FIDGETY_CRITICAL(
                        DatabaseException,
                        DatabaseStatus::InvalidData,
                        "[Fidgety::ItoJson::toVmol] "
                        "'{0}.editor.constraints' is not an array or object",
                        identifier
                    );
                }
            }
        }
        std::string oets = BoostAl::to_lower_copy(editorType);
        trim(oets);
        OptionEditorType oet;
#define OETS_2_OET(as_string, as_enum) \
    if (oets == as_string) \
        oet = as_enum; \
    else \

        OETS_2_OET("blanked", OptionEditorType::Blanked)
        OETS_2_OET("textentry", OptionEditorType::TextEntry)
        OETS_2_OET("toggle", OptionEditorType::Toggle)
        OETS_2_OET("slider", OptionEditorType::Slider)
        OETS_2_OET("dropdown", OptionEditorType::Dropdown)
        OETS_2_OET("options", OptionEditorType::Options)
        OETS_2_OET("checkboxes", OptionEditorType::Checkboxes)
        {
            FIDGETY_CRITICAL(
                DatabaseException,
                DatabaseStatus::InvalidData,
                "[Fidgety::ItoJson::toVmol] invalid '{0}.editor.constraints.type': {1}",
                identifier,
                editorType
            );
        }

#undef OETS_2_OET

        OptionEditor oe(oet, std::move(editorConstraints));
        OptionIdentifier oi = identifier;
        std::unique_ptr<Validator> ov(validator.clone());
        OptionValue ovalue(std::move(defaultValue), acceptedValueTypes);
        ovalue.setValue(std::move(svalue));
        Option *done = new Option(std::move(oi), std::move(oe), std::move(ov), std::move(ovalue));
        std::shared_ptr<Option> spDone(done);

        spdlog::debug("[Fidgety::ItoJson::toVmol] adding option: '{0}'", identifier);

        vmol[identifier] = std::move(spDone);
    }
    return vmol;
}
