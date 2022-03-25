/**
 * @file src/options/validator.cpp
 * @author RenoirTan
 * @brief Implementation of option validators.
 * @version 0.1
 * @date 2022-03-04
 * 
 * @copyright Copyright (c) 2022
 */

#include <spdlog/spdlog.h>
//#include <fidgety/extensions.hpp>
#include <fidgety/options.hpp>
#include <fidgety/_utils.hpp>

using namespace Fidgety;

ValidatorMessage::ValidatorMessage(ValidatorMessageType messageType, std::string &&message) :
    mMessage(message),
    mType(messageType)
{
    spdlog::trace("Creating Fidgety::ValidatorMessage.");
}

ValidatorMessage::~ValidatorMessage(void) {
    spdlog::trace("Deleting Fidgety::ValidatorMessage.");
}

const std::string &ValidatorMessage::getMessage(void) const noexcept {
    return mMessage;
}

ValidatorMessageType ValidatorMessage::getMessageType(void) const noexcept {
    return mType;
}

static inline const char *_ValidatorMessageTypeToStr(const ValidatorMessageType mt) {
    switch (mt) {
        case ValidatorMessageType::Valid: return "Valid";
        case ValidatorMessageType::Problematic: return "Problematic";
        case ValidatorMessageType::Invalid: return "Invalid";
        case ValidatorMessageType::Unexpected: return "Unexpected";
        default: throw fmt::format("Invalid ValidatorMessageType: {0}", (int32_t) mt);
    }
}

std::string ValidatorMessage::fullMessage(void) const {
    spdlog::trace("Creating full message from ValidatorMessage.");
    return fmt::format(
        "{0}: {1}",
        _ValidatorMessageTypeToStr(getMessageType()),
        getMessage()
    );
}

std::ostream &operator<<(std::ostream &stream, const ValidatorMessage &message) {
    return stream << message.fullMessage();
}

ValidatorContext::ValidatorContext(void) : mMap() {
    spdlog::trace("Creating Fidgety::ValidatorContext using default constructor.");
}

ValidatorContext::ValidatorContext(ValidatorContextInner &&map) : mMap(map) {
    spdlog::trace(
        "Creating Fidgety::ValidatorContext with provided Fidgety::ValidatorContextInner."
    );
}

bool ValidatorContext::optionExists(const OptionIdentifier &identifier) const noexcept {
    spdlog::trace("Checking if option exists in Fidgety::ValidatorContext.");
    return mMap.find(identifier) != mMap.end();
}

const Option &ValidatorContext::getOption(const OptionIdentifier &identifier) const {
    spdlog::trace("Getting option from Fidgety::ValidatorContext.");
    auto iterator = mMap.find(identifier);
    if (iterator == mMap.end()) {
        spdlog::trace("Could not find option in Fidgety::ValidatorContext.");
        FIDGETY_CRITICAL(
            OptionException,
            OptionStatus::NotFound,
            "could not find option with the identifier '{0}' in the current context",
            identifier
        );
    } else {
        spdlog::trace("Option found in Fidgety::ValidatorContext.");
        return *(iterator->second);
    }
}

const ValidatorContextInner &ValidatorContext::getInnerMap(void) const noexcept {
    return mMap;
}

Validator::Validator(void) {
    spdlog::trace("Creating Fidgety::Validator");
}

ValidatorMessage Validator::validate(
    const Option &option,
    const ValidatorContext &context
) {
    spdlog::trace("Validating option {0} from Fidgety::Validator.", option.getIdentifier());
    return ValidatorMessage(ValidatorMessageType::Valid, "Ok");
}

Validator *Validator::clone(void) const {
    return new Validator();
}

/*
#ifdef __cplusplus

extern "C" {
    FIDGETY_ALLOC(FIDGETY_VALIDATOR_ALLOC_PROT(), Validator);
    FIDGETY_DELETE(FIDGETY_VALIDATOR_DELETE_PROT());
}

#endif
*/
