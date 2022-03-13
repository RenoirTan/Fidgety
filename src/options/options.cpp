/**
 * @file src/options/options.cpp
 * @author RenoirTan
 * @brief Implementation of options.
 * @version 0.1
 * @date 2022-03-04
 * 
 * @copyright Copyright (c) 2022
 */

#include "spdlog/spdlog.h"
#include <fidgety/options.hpp>
#include <fidgety/_utils.hpp>

using namespace Fidgety;

std::string OptionException::codeAsErrorType(void) const {
    switch (mCode) {
        case 0: return "Ok";
        case 1: return "InvalidValueType";
        case 2: return "IncompatibleOptionEditor";
        case 3: return "NotFound";
        default: return "Other";
    }
}

const char *OptionException::getSimpleWhat(void) const noexcept {
    return "A Fidgety::OptionException occurred.";
}

_OptionValueInner::~_OptionValueInner(void) {
    spdlog::debug("deleted Fidgety::_OptionValueInner ((PRIVATE))");
}

OptionValueInner::OptionValueInner(void) :
    valueType(OptionValueType::RAW_VALUE),
    value { rawValue: std::string() }
{
    spdlog::debug("created Fidgety::OptionValueInner using defaults");
}

OptionValueInner::OptionValueInner(const char *rawValue) :
    valueType(OptionValueType::RAW_VALUE),
    value { rawValue: std::string(rawValue) }
{
    spdlog::debug("created Fidgety::OptionValueInner with a char array");
}

OptionValueInner::OptionValueInner(std::string &&rawValue) :
    valueType(OptionValueType::RAW_VALUE),
    value { rawValue: std::string(rawValue) }
{
    spdlog::debug("created Fidgety::OptionValueInner with std::string");
}

OptionValueInner::OptionValueInner(NestedOptionList &&nestedList) :
    valueType(OptionValueType::NESTED_LIST),
    value { nestedList: nestedList }
{
    spdlog::debug("created Fidgety::OptionValueInner with a NestedOptionList");
}

OptionValueInner::~OptionValueInner(void) {
    spdlog::trace("deleting Fidgety::OptionValueInner");
    this->_deleteCurrentValue();
    spdlog::debug("deleted Fidgety::OptionValueInner");
}

OptionValueInner::OptionValueInner(const OptionValueInner &other) :
    valueType(OptionValueType::RAW_VALUE),
    value { rawValue: std::string() }
{
    spdlog::trace("creating Fidgety::OptionValueInner using copy constructor");
    *this = other;
    spdlog::debug("created Fidgety::OptionValueInner using copy constructor");
}

OptionValueInner::OptionValueInner(OptionValueInner &&other) :
    valueType(OptionValueType::RAW_VALUE),
    value { rawValue: std::string() }
{
    spdlog::trace("creating Fidgety::OptionValueInner using move constructor");
    *this = std::move(other);
    spdlog::debug("created Fidgety::OptionValueInner using move constructor");
}

OptionValueInner &OptionValueInner::operator=(const OptionValueInner &other) {
    spdlog::trace("assigning Fidgety::OptionValueInner by copying");
    if (OptionValueType::isValid(other.valueType)) {
        this->_deleteCurrentValue();
    } else {
        FIDGETY_CRITICAL(
            OptionException,
            OptionStatus::InvalidValueType,
            "invalid value type code ({0}) detected when trying to copy "
            "Fidgety::OptionValueInner when using copy assignment",
            other.valueType
        );
    }
    switch (other.valueType) {
        case OptionValueType::RAW_VALUE: {
            spdlog::trace(
                "detected raw value when assigning Fidgety::OptionValueInner by copying"
            );
            new (&this->value.rawValue) std::string(other.value.rawValue);
            this->valueType = other.valueType;
            break;
        }
        case OptionValueType::NESTED_LIST: {
            spdlog::trace(
                "detected nested option list when assigning Fidgety::OptionValueInner by copying"
            );
            new (&this->value.nestedList) NestedOptionList(other.value.nestedList);
            this->valueType = other.valueType;
            break;
        }
    }
    spdlog::debug("assigned Fidgety::OptionValueInner by copying");
    return *this;
}

OptionValueInner &OptionValueInner::operator=(OptionValueInner &&other) {
    spdlog::trace("assigning Fidgety::OptionValueInner by moving");
    if (OptionValueType::isValid(other.valueType)) {
        this->_deleteCurrentValue();
    } else {
        FIDGETY_CRITICAL(
            OptionException,
            OptionStatus::InvalidValueType,
            "invalid value type code ({0}) detected when trying to move "
            "Fidgety::OptionValueInner when using move assignment",
            other.valueType
        );
    }
    switch (other.valueType) {
        case OptionValueType::RAW_VALUE: {
            spdlog::trace(
                "detected raw value when assigning Fidgety::OptionValueInner by moving"
            );
            new (&this->value.rawValue) std::string(std::move(other.value.rawValue));
            this->valueType = other.valueType;
            break;
        }
        case OptionValueType::NESTED_LIST: {
            spdlog::trace(
                "detected nested option list when assigning Fidgety::OptionValueInner by moving"
            );
            new (&this->value.nestedList) NestedOptionList(std::move(other.value.nestedList));
            this->valueType = other.valueType;
            break;
        }
    }
    spdlog::debug("assigned Fidgety::OptionValueInner by moving");
    return *this;
}

void OptionValueInner::_deleteCurrentValue(void) {
    spdlog::trace("deleting current value in Fidgety::OptionValueInner");
    switch (this->valueType) {
        case OptionValueType::RAW_VALUE: {
            spdlog::trace("RAW_VALUE detected when deleting value in Fidgety::OptionValueInner");
            this->value.rawValue.std::string::~string();
            break;
        }
        case OptionValueType::NESTED_LIST: {
            spdlog::trace("NESTED_LIST detected when deleting value in Fidgety::OptionValueInner");
            this->value.nestedList.NestedOptionList::~vector();
            break;
        }
        default: {
            FIDGETY_CRITICAL(
                OptionException,
                OptionStatus::InvalidValueType,
                "invalid value type code ({0}) detected when trying to delete "
                "the internal value of Fidgety::OptionValueInner in _deleteCurrentValue",
                this->valueType
            );
        }
    }
}

const NestedOptionList &OptionValueInner::getNestedList(void) const {
    spdlog::trace("getting nested option list from Fidgety::OptionValueInner");
    if (this->valueType == OptionValueType::NESTED_LIST) {
        spdlog::trace("returning nested option list");
        return this->value.nestedList;
    } else {
        FIDGETY_CRITICAL(
            OptionException,
            OptionStatus::InvalidValueType,
            "requested a nestedList from OptionValueInner::getNestedList but failed"
        );
    }
}

const std::string &OptionValueInner::getRawValue(void) const {
    spdlog::trace("getting raw value from Fidgety::OptionValueInner");
    if (this->valueType == OptionValueType::RAW_VALUE) {
        spdlog::trace("returning raw value");
        return this->value.rawValue;
    } else {
        FIDGETY_CRITICAL(
            OptionException,
            OptionStatus::InvalidValueType,
            "requested a rawValue from OptionValueInner::getRawValue but failed"
        );
    }
}

bool OptionValueType::isValid(const int32_t valueType) {
    switch (valueType) {
        case OptionValueType::RAW_VALUE:
        case OptionValueType::NESTED_LIST:
            return true;
        default:
            return false;
    }
}

OptionValue::OptionValue(int32_t acceptedValueTypes) :
    mAcceptedValueTypes(acceptedValueTypes)
{
    spdlog::trace("creating Fidgety::OptionValue using only acceptedValueTypes");
    if (!(mAcceptedValueTypes & mDefault.valueType)) {
        FIDGETY_CRITICAL(
            OptionException,
            OptionStatus::InvalidValueType,
            "OptionValue::mDefault.valueType ({0}) "
            "not allowed by OptionValue::mAcceptedValueTypes ({1})",
            mDefault.valueType,
            mAcceptedValueTypes
        );
    }
    mValue = mDefault;
    spdlog::debug("created Fidgety::OptionValue using only acceptedValueTypes");
}

OptionValue::OptionValue(
    OptionValueInner &&defaultValue,
    int32_t acceptedValueTypes
) :
    mAcceptedValueTypes(acceptedValueTypes),
    mDefault(defaultValue)
{
    spdlog::trace("creating Fidgety::OptionValue with defaultValue and acceptedValueTypes");
    if (!(mAcceptedValueTypes & mDefault.valueType)) {
        FIDGETY_CRITICAL(
            OptionException,
            OptionStatus::InvalidValueType,
            "OptionValue::mDefault.valueType ({0}) "
            "not allowed by OptionValue::mAcceptedValueTypes ({1})",
            mDefault.valueType,
            mAcceptedValueTypes
        );
    }
    mValue = mDefault;
    spdlog::trace("created Fidgety::OptionValue with defaultValue and acceptedValueTypes");
}

int32_t OptionValue::getValueType(void) const noexcept {
    return mValue.valueType;
}

const OptionValueInner &OptionValue::getValue(void) const noexcept {
    return mValue;
}

OptionStatus OptionValue::setValue(OptionValueInner &&value) {
    spdlog::trace("setting mValue of Fidgety::OptionValue");
    if (!(mAcceptedValueTypes & value.valueType)) {
        FIDGETY_CRITICAL(
            OptionException,
            OptionStatus::InvalidValueType,
            "value.valueType ({0}) "
            "not allowed by OptionValue::mAcceptedValueTypes ({1})",
            value.valueType,
            mAcceptedValueTypes
        );
    }
    return OptionStatus::Ok;
}

int32_t OptionValue::getDefaultValueType(void) const noexcept {
    return mDefault.valueType;
}

const OptionValueInner &OptionValue::getDefaultValue(void) const noexcept {
    return mDefault;
}

OptionStatus OptionValue::setDefaultValue(OptionValueInner &&defaultValue) {
    spdlog::trace("setting mDefault of Fidgety::OptionValue");
    if (!(mAcceptedValueTypes & defaultValue.valueType)) {
        FIDGETY_CRITICAL(
            OptionException,
            OptionStatus::InvalidValueType,
            "defaultValue.valueType ({0}) "
            "not allowed by OptionValue::mAcceptedValueTypes ({1})",
            defaultValue.valueType,
            mAcceptedValueTypes
        );
    }
    return OptionStatus::Ok;
}

void OptionValue::resetValue(void) {
    spdlog::trace("resetting mValue of Fidgety::OptionValue using mDefault");
    mValue = mDefault;
    spdlog::debug("reset mValue of Fidgety::OptionValue using mDefault");
}

void OptionValue::setAcceptedValueTypes(int32_t acceptedValueTypes) {
    spdlog::trace("setting mAcceptedValueTypes of Fidgety::OptionValue");
    if (!(acceptedValueTypes & mDefault.valueType)) {
        FIDGETY_CRITICAL(
            OptionException,
            OptionStatus::InvalidValueType,
            "OptionValie::mDefault.valueType ({0}) "
            "not allowed by acceptedValueTypes ({1})",
            mDefault.valueType,
            acceptedValueTypes
        );
    } else if (!(acceptedValueTypes & mValue.valueType)) {
        FIDGETY_CRITICAL(
            OptionException,
            OptionStatus::InvalidValueType,
            "OptionValie::mValue.valueType ({0}) "
            "not allowed by acceptedValueTypes ({1})",
            mValue.valueType,
            acceptedValueTypes
        );
    }
    mAcceptedValueTypes = acceptedValueTypes;
    spdlog::debug("set mAcceptedValueTypes of Fidgety::OptionValue");
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
    Validator &&validator,
    int32_t acceptedValueTypes
) noexcept :
    mIdentifier(identifier),
    mValue(acceptedValueTypes),
    mValidator(validator),
    mOptionEditor(optionEditor)
{
    spdlog::debug("created Fidgety::Option ({0}) using acceptedValueTypes", mIdentifier);
}

Option::Option(
    OptionIdentifier identifier,
    OptionEditor &&optionEditor,
    Validator &&validator,
    OptionValue &&value
) :
    mIdentifier(identifier),
    mValue(value),
    mValidator(validator),
    mOptionEditor(optionEditor)
{
    spdlog::debug("created Fidgety::Option ({0}) using Fidgety::OptionValue", mIdentifier);
}

Option::~Option(void) {
    spdlog::debug("deleted Fidgety::Option");
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

const NestedOptionList &Option::getNestedList(void) const {
    return getValue().getNestedList();
}

const std::string &Option::getRawValue(void) const {
    return getValue().getRawValue();
}

int32_t Option::getDefaultValueType(void) const {
    return getDefaultValue().valueType;
}

const NestedOptionList &Option::getDefaultNestedList(void) const {
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

OptionStatus Option::setValue(NestedOptionList &&value) {
    spdlog::trace("setting value of Fidgety::Option ({0}) using Fidgety::NestedOptionList", mIdentifier);
    OptionValueInner v(std::move(value));
    return mValue.setValue(std::move(v));
}

OptionStatus Option::setDefaultValue(std::string &&defaultValue) {
    spdlog::trace("setting default value of Fidgety::Option ({0}) using std::string", mIdentifier);
    OptionValueInner dv(std::move(defaultValue));
    return mValue.setDefaultValue(std::move(dv));
}

OptionStatus Option::setDefaultValue(NestedOptionList &&defaultValue) {
    spdlog::trace(
        "setting default value of Fidgety::Option ({0}) using Fidgety::NestedOptionList",
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

void Option::setValidator(Validator validator) noexcept {
    spdlog::trace("setting validator in Fidgety::Option ({0})", mIdentifier);
    mValidator = validator;
}

ValidatorMessage Option::validate(const ValidatorContext &context) {
    spdlog::trace("validating value in Fidgety::Option ({0})", mIdentifier);
    ValidatorMessage message = mValidator.validate(*this, context);
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
