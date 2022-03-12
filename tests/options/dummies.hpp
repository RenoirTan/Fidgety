#ifndef _DUMMIES_HPP
#   define _DUMMIES_HPP

#   include <fidgety/options.hpp>
#   include <fmt/core.h>

Fidgety::Option makeDummyOption(std::string &&identifier) {
    Fidgety::Validator validator;
    Fidgety::OptionEditor oet(
        Fidgety::OptionEditorType::Blanked,
        std::map<std::string, std::string>()
    );
    return Fidgety::Option(
        identifier,
        std::move(oet),
        std::move(validator),
        Fidgety::OptionValueType::RAW_VALUE
    );
}

Fidgety::NestedOptionList makeNestedOptionList(size_t number) {
    Fidgety::NestedOptionList nol;
    for (size_t i = 0; i < number; ++i) {
        Fidgety::Option *option = new Fidgety::Option(
            std::move(makeDummyOption(fmt::format("Option {0}", i)))
        );
        nol.emplace_back(option);
    }
    return nol;
}

#endif
