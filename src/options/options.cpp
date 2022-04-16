/**
 * @file src/options/options.cpp
 * @author RenoirTan
 * @brief Implementation of Fidgety::Option and any other things that don't
 * make sense to be spun off into another .cpp file.
 * @version 0.1
 * @date 2022-03-04
 * 
 * @copyright Copyright (c) 2022
 */

#include <algorithm>
#include <cctype>
#include <boost/algorithm/string.hpp>
#include <spdlog/spdlog.h>
#include <fidgety/options.hpp>
#include <fidgety/_utils.hpp>

using namespace Fidgety;

std::string OptionException::codeAsErrorType(void) const {
    switch (mCode) {
        case 0: return "Ok";
        case 1: return "InvalidValueType";
        case 2: return "IncompatibleOptionEditor";
        case 3: return "NotFound";
        case 4: return "InvalidIdentifier";
        case 5: return "InvalidName";
        default: return "Other";
    }
}

const char *OptionException::getSimpleWhat(void) const noexcept {
    return "A Fidgety::OptionException occurred.";
}

OptionEditor::OptionEditor(OptionEditorType oet, std::map<std::string, std::string> &&constraints) :
    mEditorType(oet),
    mConstraints(constraints)
{
    spdlog::debug("created Fidgety::OptionEditor");
}

Option::Option(
    OptionIdentifier identifier,
    OptionEditor &&optionEditor,
    std::unique_ptr<Validator> &&validator,
    int32_t acceptedValueTypes
) noexcept :
    mIdentifier(identifier),
    mValue(acceptedValueTypes),
    mValidator(std::move(validator)),
    mOptionEditor(optionEditor)
{
    spdlog::debug("created Fidgety::Option ({0}) using acceptedValueTypes", mIdentifier);
}

Option::Option(
    OptionIdentifier identifier,
    OptionEditor &&optionEditor,
    std::unique_ptr<Validator> &&validator,
    OptionValue &&value
) :
    mIdentifier(identifier),
    mValue(value),
    mValidator(std::move(validator)),
    mOptionEditor(optionEditor)
{
    spdlog::debug("created Fidgety::Option ({0}) using Fidgety::OptionValue", mIdentifier);
}

Option::~Option(void) {
    spdlog::debug("deleted Fidgety::Option");
}

Option::Option(Option &&option) :
    mIdentifier(std::move(option.mIdentifier)),
    mValue(std::move(option.mValue)),
    mValidator(std::move(option.mValidator)),
    mOptionEditor(std::move(option.mOptionEditor))
{
    spdlog::trace("creating Fidgety::Option using move constructor");
}

Option &Option::operator=(Option &&option) {
    spdlog::trace("reassigning Fidgety::Option using move assignment");
    mIdentifier = std::move(option.mIdentifier);
    mValue = std::move(option.mValue);
    mValidator = std::move(option.mValidator);
    mOptionEditor = std::move(option.mOptionEditor);
    return *this;
}

const OptionIdentifier &Option::getIdentifier(void) const noexcept {
    return mIdentifier;
}

const OptionValue &Option::getOptionValue(void) const noexcept {
    return mValue;
}

const OptionValueInner &Option::getValue(void) const noexcept {
    return mValue.getValue();
}

const OptionValueInner &Option::getDefaultValue(void) const noexcept {
    return mValue.getDefaultValue();
}

int32_t Option::getValueType(void) const {
    return getValue().valueType;
}

const NestedOptionNameList &Option::getNestedList(void) const {
    return getValue().getNestedList();
}

const std::string &Option::getRawValue(void) const {
    return getValue().getRawValue();
}

int32_t Option::getDefaultValueType(void) const {
    return getDefaultValue().valueType;
}

const NestedOptionNameList &Option::getDefaultNestedList(void) const {
    return getDefaultValue().getNestedList();
}

const std::string &Option::getDefaultRawValue(void) const {
    return getDefaultValue().getRawValue();
}

OptionStatus Option::setValue(std::string &&value) {
    spdlog::trace("setting value of Fidgety::Option ({0}) using std::string", mIdentifier);
    OptionValueInner v(std::move(value));
    return mValue.setValue(std::move(v));
}

OptionStatus Option::setValue(NestedOptionNameList &&value) {
    spdlog::trace("setting value of Fidgety::Option ({0}) using Fidgety::NestedOptionNameList", mIdentifier);
    OptionValueInner v(std::move(value));
    return mValue.setValue(std::move(v));
}

OptionStatus Option::setDefaultValue(std::string &&defaultValue) {
    spdlog::trace("setting default value of Fidgety::Option ({0}) using std::string", mIdentifier);
    OptionValueInner dv(std::move(defaultValue));
    return mValue.setDefaultValue(std::move(dv));
}

OptionStatus Option::setDefaultValue(NestedOptionNameList &&defaultValue) {
    spdlog::trace(
        "setting default value of Fidgety::Option ({0}) using Fidgety::NestedOptionNameList",
        mIdentifier
    );
    OptionValueInner dv(std::move(defaultValue));
    return mValue.setDefaultValue(std::move(dv));
}

OptionStatus Option::resetValue(void) {
    spdlog::trace("resetting value using default value in Fidgety::Option ({0})", mIdentifier);
    mValue.resetValue();
    return OptionStatus::Ok;
}

OptionStatus Option::setAcceptedValueTypes(int32_t acceptedValueTypes) {
    spdlog::trace("setting accepted value types in Fidgety::Option ({0})", mIdentifier);
    mValue.setAcceptedValueTypes(acceptedValueTypes);
    return OptionStatus::Ok;
}

void Option::setValidator(std::unique_ptr<Validator> &&validator) noexcept {
    spdlog::trace("setting validator in Fidgety::Option ({0})", mIdentifier);
    mValidator = std::move(validator);
}

ValidatorMessage Option::validate(const ValidatorContext &context) {
    spdlog::trace("validating value in Fidgety::Option ({0})", mIdentifier);
    ValidatorMessage message = mValidator->validate(*this, context);
    spdlog::trace("validation complete, saving message in Fidgety::mLastValidatorMessage");
    mLastValidatorMessage = message;
    return message;
}

const ValidatorMessage &Option::getLastValidatorMessage(void) const noexcept {
    return mLastValidatorMessage;
}

const OptionEditor &Option::getOptionEditor(void) const noexcept {
    return mOptionEditor;
}

OptionStatus Option::setOptionEditor(OptionEditor &&optionEditor) {
    spdlog::trace("setting option editor of Fidgety::Option ({0})", mIdentifier);
    mOptionEditor = optionEditor;
    return OptionStatus::Ok;
}
