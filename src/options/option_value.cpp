/**
 * @file src/options/option_value.cpp
 * @author RenoirTan
 * @brief Implementation of Fidgety::OptionValue and other related classes.
 * @version 0.1
 * @date 2022-04-16
 * 
 * @copyright Copyright (c) 2022
 */

#include <spdlog/spdlog.h>
#include <fidgety/options.hpp>
#include <fidgety/_utils.hpp>

using namespace Fidgety;

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

OptionValueInner::OptionValueInner(NestedOptionNameList &&nestedList) :
    valueType(OptionValueType::NESTED_LIST),
    value { nestedList: nestedList }
{
    spdlog::debug("created Fidgety::OptionValueInner with a NestedOptionNameList");
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
            new (&this->value.nestedList) NestedOptionNameList(other.value.nestedList);
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
            new (&this->value.nestedList) NestedOptionNameList(std::move(other.value.nestedList));
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
            this->value.nestedList.NestedOptionNameList::~vector();
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

const NestedOptionNameList &OptionValueInner::getNestedList(void) const {
    spdlog::trace("getting nested option list from Fidgety::OptionValueInner");
    spdlog::trace("OptionValueInner::valueType == {0}", this->valueType);
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
    spdlog::trace("OptionValueInner::valueType == {0}", this->valueType);
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
    } else {
        mValue = std::move(value);
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
    } else {
        mDefault = std::move(defaultValue);
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
