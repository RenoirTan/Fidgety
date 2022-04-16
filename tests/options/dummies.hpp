/**
 * @file tests/options/dummies.hpp
 * @author RenoirTan
 * @brief Header file for helper function for the `options` test group.
 * This file contains functions which create dummy Fidgety::Options so that
 * I don't have to constantly write boilerplate code.
 * @version 0.1
 * @date 2022-03-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _DUMMIES_HPP
#   define _DUMMIES_HPP

#   include <utility> // std::pair
#   include <fidgety/options.hpp>
#   include <fidgety/_tests.hpp>
#   include <fidgety/_utils.hpp>
#   include <fmt/core.h>
#   include <spdlog/spdlog.h>

namespace Fidgety {

    Option makeDummyOption(OptionIdentifier &&identifier) {
        spdlog::debug("MAKING DUMMY OPTION");
        std::unique_ptr<Validator> validator(new Validator());
        OptionEditor oet(
            OptionEditorType::Blanked,
            std::map<std::string, std::string>()
        );
        Option option(
            identifier,
            std::move(oet),
            std::move(validator),
            OptionValueType::RAW_VALUE
        );
        spdlog::debug("MADE DUMMY OPTION");
        return option;
    }

    std::pair<OptionsMap, NestedOptionNameList> makeNestedOptionList(size_t number) {
        spdlog::debug("MAKING NESTED OPTION LIST");
        OptionsMap omap;
        NestedOptionNameList nonl;
        for (size_t i = 0; i < number; ++i) {
            spdlog::trace("NOL COUNT: {0}", i);
            Option *option = new Option(
                std::move(makeDummyOption(fmt::format("Option {0}", i)))
            );
            nonl.push_back(option->getIdentifier());
            omap.emplace(option->getIdentifier(), option);
        }
        spdlog::debug("MADE NESTED OPTION LIST");
        return {omap, nonl};
    }

    ValidatorContextInner makeValidatorContextInner(size_t number) {
        spdlog::debug("MAKING VALIDATOR CONTEXT INNER");
        ValidatorContextInner vci;
        for (size_t i = 0; i < number; ++i) {
            spdlog::trace("VCI COUNT: {0}", i);
            OptionIdentifier identifier = fmt::format("Option {0}", i);
            Option *option = new Option(std::move(makeDummyOption(std::move(identifier))));
            vci.emplace(identifier, option);
        }
        spdlog::debug("MADE VALIDATOR CONTEXT INNER");
        return vci;
    }

}

#endif
