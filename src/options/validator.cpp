/**
 * @file src/options/validator.cpp
 * @author RenoirTan
 * @brief Implementation of option validators.
 * @version 0.1
 * @date 2022-03-04
 * 
 * @copyright Copyright (c) 2022
 */

#include "spdlog/spdlog.h"
#include <fidgety/options.hpp>
#include <fidgety/_utils.hpp>

using namespace Fidgety;

ValidatorMessage::ValidatorMessage(std::string &&message, ValidatorMessageType messageType) :
    mMessage(message),
    mType(messageType)
{ }

ValidatorMessage::~ValidatorMessage(void) { }

const std::string &ValidatorMessage::getMessage(void) const noexcept {
    return mMessage;
}

ValidatorMessageType ValidatorMessage::getMessageType(void) const noexcept {
    return mType;
}

namespace Fidgety {
    inline const char *_validator_message_type_to_str(const ValidatorMessageType mt) {
        switch (mt) {
            case ValidatorMessageType::Valid: return "Valid";
            case ValidatorMessageType::Problematic: return "Problematic";
            case ValidatorMessageType::Invalid: return "Invalid";
            case ValidatorMessageType::Unexpected: return "Unexpected";
            default: throw fmt::format("Invalid ValidatorMessageType: {0}", mt);
        }
    }
};

std::string ValidatorMessage::fullMessage(void) const {
    return fmt::format(
        "{0}: {1}",
        _validator_message_type_to_str(getMessageType()),
        getMessage()
    );
}

std::ostream &operator<<(std::ostream &stream, const ValidatorMessage &message) {
    return stream << message.fullMessage();
}

ValidatorContext::ValidatorContext(void) : mMap() { }

ValidatorContext::ValidatorContext(ValidatorContextInner &&map) : mMap(map) {}

bool ValidatorContext::optionExists(const OptionIdentifier &identifier) const noexcept {
    return mMap.find(identifier) != mMap.end();
}

const Option &ValidatorContext::getOption(const OptionIdentifier &identifier) const {
    auto iterator = mMap.find(identifier);
    if (iterator == mMap.end()) {
        FIDGETY_CRITICAL(
            OptionException,
            OptionStatus::NotFound,
            "could not find option with the identifier '{0}' in the current context",
            identifier
        );
    } else {
        return *(iterator->second);
    }
}

const ValidatorContextInner &ValidatorContext::getInnerMap(void) const noexcept {
    return mMap;
}

Validator::Validator(void) { }

ValidatorMessage validate(
    const Option &option,
    const ValidatorContext &context
) {
    return ValidatorMessage("Ok", ValidatorMessageType::Valid);
}
