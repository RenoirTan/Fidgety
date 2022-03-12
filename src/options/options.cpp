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

_OptionValueInner::~_OptionValueInner(void) { }

OptionValueInner::OptionValueInner(void) :
    valueType(OptionValueType::RAW_VALUE),
    value { rawValue: std::string() }
{ }

OptionValueInner::OptionValueInner(const char *rawValue) :
    valueType(OptionValueType::RAW_VALUE),
    value { rawValue: std::string(rawValue) }
{ }

OptionValueInner::OptionValueInner(std::string &&rawValue) :
    valueType(OptionValueType::RAW_VALUE),
    value { rawValue: std::string(rawValue) }
{ }

OptionValueInner::OptionValueInner(NestedOptionList &&nestedList) :
    valueType(OptionValueType::NESTED_LIST),
    value { nestedList: nestedList }
{ }

OptionValueInner::~OptionValueInner(void) {
    switch (this->valueType) {
        case OptionValueType::RAW_VALUE: {
            this->value.rawValue.std::string::~string();
        }
        case OptionValueType::NESTED_LIST: {
            this->value.nestedList.NestedOptionList::~vector();
        }
        default: {
            // Destructors must be noexcept >:(
            /*
            FIDGETY_CRITICAL(
                OptionException,
                OptionStatus::InvalidValueType,
                "invalid value type code ({0}) detected when trying to delete "
                "Fidgety::OptionValueInner",
                this->valueType
            );
            */
        }
    }
}

OptionValueInner::OptionValueInner(const OptionValueInner &other) :
    valueType(OptionValueType::RAW_VALUE),
    value { rawValue: std::string() }
{
    switch (other.valueType) {
        case OptionValueType::RAW_VALUE: {
            this->value.rawValue = other.value.rawValue;
            this->valueType = other.valueType;
        }
        case OptionValueType::NESTED_LIST: {
            this->value.nestedList = other.value.nestedList;
            this->valueType = other.valueType;
        }
        default: {
            FIDGETY_CRITICAL(
                OptionException,
                OptionStatus::InvalidValueType,
                "invalid value type code ({0}) detected when trying to copy "
                "Fidgety::OptionValueInner",
                other.valueType
            );
        }
    }
}

OptionValueInner::OptionValueInner(OptionValueInner &&other) :
    valueType(OptionValueType::RAW_VALUE),
    value { rawValue: std::string() }
{
    switch (other.valueType) {
        case OptionValueType::RAW_VALUE: {
            this->value.rawValue = std::move(other.value.rawValue);
            this->valueType = other.valueType;
        }
        case OptionValueType::NESTED_LIST: {
            this->value.nestedList = std::move(other.value.nestedList);
            this->valueType = other.valueType;
        }
        default: {
            FIDGETY_CRITICAL(
                OptionException,
                OptionStatus::InvalidValueType,
                "invalid value type code ({0}) detected when trying to move "
                "Fidgety::OptionValueInner",
                other.valueType
            );
        }
    }
}

OptionValueInner &OptionValueInner::operator=(const OptionValueInner &other) {
    switch (other.valueType) {
        case OptionValueType::RAW_VALUE: {
            this->value.rawValue = other.value.rawValue;
            this->valueType = other.valueType;
        }
        case OptionValueType::NESTED_LIST: {
            this->value.nestedList = other.value.nestedList;
            this->valueType = other.valueType;
        }
        default: {
            FIDGETY_CRITICAL(
                OptionException,
                OptionStatus::InvalidValueType,
                "invalid value type code ({0}) detected when trying to copy "
                "Fidgety::OptionValueInner",
                other.valueType
            );
        }
    }
    return *this;
}

OptionValueInner &OptionValueInner::operator=(OptionValueInner &&other) {
    switch (other.valueType) {
        case OptionValueType::RAW_VALUE: {
            this->value.rawValue = std::move(other.value.rawValue);
            this->valueType = other.valueType;
        }
        case OptionValueType::NESTED_LIST: {
            this->value.nestedList = std::move(other.value.nestedList);
            this->valueType = other.valueType;
        }
        default: {
            FIDGETY_CRITICAL(
                OptionException,
                OptionStatus::InvalidValueType,
                "invalid value type code ({0}) detected when trying to move "
                "Fidgety::OptionValueInner",
                other.valueType
            );
        }
    }
    return *this;
}

const NestedOptionList &OptionValueInner::getNestedList(void) const {
    if (this->valueType == OptionValueType::NESTED_LIST) {
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
    if (this->valueType == OptionValueType::RAW_VALUE) {
        return this->value.rawValue;
    } else {
        FIDGETY_CRITICAL(
            OptionException,
            OptionStatus::InvalidValueType,
            "requested a rawValue from OptionValueInner::getRawValue but failed"
        );
    }
}

OptionValue::OptionValue(int32_t acceptedValueType) :
    mAcceptedValueTypes(acceptedValueType)
{
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
}

OptionValue::OptionValue(
    OptionValueInner &&defaultValue,
    int32_t acceptedValueTypes
) :
    mAcceptedValueTypes(acceptedValueTypes),
    mDefault(defaultValue)
{
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
}

int32_t OptionValue::getValueType(void) const noexcept {
    return mValue.valueType;
}

const OptionValueInner &OptionValue::getValue(void) const noexcept {
    return mValue;
}

OptionStatus OptionValue::setValue(OptionValueInner &&value) {
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
    mValue = mDefault;
}

void OptionValue::setAcceptedValueTypes(int32_t acceptedValueTypes) {
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
}

OptionEditor::OptionEditor(OptionEditorType oet, std::map<std::string, std::string> &&constraints) :
    mEditorType(oet),
    mConstraints(constraints)
{ }

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
{ }

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
{ }

Option::~Option(void) { }

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
    OptionValueInner v(std::move(value));
    return mValue.setValue(std::move(v));
}
OptionStatus Option::setValue(NestedOptionList &&value) {
    OptionValueInner v(std::move(value));
    return mValue.setValue(std::move(v));
}

OptionStatus Option::setDefaultValue(std::string &&defaultValue) {
    OptionValueInner dv(std::move(defaultValue));
    return mValue.setDefaultValue(std::move(dv));
}
OptionStatus Option::setDefaultValue(NestedOptionList &&defaultValue) {
    OptionValueInner dv(std::move(defaultValue));
    return mValue.setDefaultValue(std::move(dv));
}

OptionStatus Option::resetValue(void) {
    mValue.resetValue();
    return OptionStatus::Ok;
}
OptionStatus Option::setAcceptedValueTypes(int32_t acceptedValueTypes) {
    mValue.setAcceptedValueTypes(acceptedValueTypes);
    return OptionStatus::Ok;
}

void Option::setValidator(Validator validator) noexcept {
    mValidator = validator;
}

ValidatorMessage Option::validate(const ValidatorContext &context) {
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
    mOptionEditor = optionEditor;
    return OptionStatus::Ok;
}
