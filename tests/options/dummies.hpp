#ifndef _DUMMIES_HPP
#   define _DUMMIES_HPP

#   include <fidgety/options.hpp>
#   include <fidgety/_utils.hpp>
#   include <fmt/core.h>

namespace Fidgety {

    Option makeDummyOption(std::string &&identifier) {
        Validator validator;
        OptionEditor oet(
            OptionEditorType::Blanked,
            std::map<std::string, std::string>()
        );
        return Option(
            identifier,
            std::move(oet),
            std::move(validator),
            OptionValueType::RAW_VALUE
        );
    }

    NestedOptionList makeNestedOptionList(size_t number) {
        NestedOptionList nol;
        for (size_t i = 0; i < number; ++i) {
            Option *option = new Option(
                std::move(makeDummyOption(fmt::format("Option {0}", i)))
            );
            nol.emplace_back(option);
        }
        return nol;
    }

    ValidatorContextInner makeValidatorContextInner(size_t number) {
        ValidatorContextInner vci;
        for (size_t i = 0; i < number; ++i) {
            OptionIdentifier identifier = fmt::format("Option {0}", i);
            Option *option = new Option(std::move(makeDummyOption(std::move(identifier))));
            vci.emplace(identifier, option);
        }
        return vci;
    }

}

#endif
