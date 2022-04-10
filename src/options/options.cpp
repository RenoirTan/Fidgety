/**
 * @file src/options/options.cpp
 * @author RenoirTan
 * @brief Implementation of options.
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

OptionIdentifier::OptionIdentifier(const std::string &path) : mPath(path) { }

OptionIdentifier::OptionIdentifier(std::string &&path) : mPath(std::move(path)) { }

OptionIdentifier::OptionIdentifier(const char path[]) : mPath(path) { }

OptionIdentifier &OptionIdentifier::operator=(const std::string &path) {
    mPath = path;
    return *this;
}

OptionIdentifier &OptionIdentifier::operator=(std::string &&path) {
    mPath = std::move(path);
    return *this;
}

bool OptionIdentifier::isValid(void) const noexcept {
    if (mPath.empty()) {
        return false;
    }
    std::vector<OptionName> splat = split();
    for (const auto &name : splat) {
        if (name.empty() || (std::isdigit(name[0]) && !isDecimalInteger(name))) {
            return false;
        }
    }
    return true;
}

/*
OptionIdentifier::operator std::string(void) const {
    return mPath;
}
*/

OptionIdentifier::operator const std::string &(void) const {
    return mPath;
}

const std::string &OptionIdentifier::getPath(void) const {
    return mPath;
}

size_t OptionIdentifier::depth(void) const {
    return countSubstr(mPath, OPTION_NAME_DELIMITER) + 1;
}

std::vector<OptionName> OptionIdentifier::split(void) const {
    std::vector<OptionName> splat;
    boost::split(splat, mPath, boost::is_any_of(OPTION_NAME_DELIMITER));
    return splat;
}

OptionIdentifier::Iterator OptionIdentifier::at(size_t index) const {
    size_t start = 0, end = mPath.find(OPTION_NAME_DELIMITER);
    size_t count = 0;
    for (count = 0; count <= index; ++count) {
        end = mPath.find(OPTION_NAME_DELIMITER, start);
        if (end == std::string::npos) {
            break;
        } else if (count < index) {
            start = end+1;
        }
    }
    end = (end == std::string::npos) ? mPath.size() : end;
    // if out of bounds
    if (count < index) {
        return OptionIdentifier::Iterator {
            .identifier = this,
            .index = index,
            .name = "",
            .state = OptionIdentifier::Iterator::OUT_OF_BOUNDS
        };
    } else {
        return OptionIdentifier::Iterator {
            .identifier = this,
            .index = index,
            .name = mPath.substr(start, end-start),
            .state = OptionIdentifier::Iterator::VALID
        };
    }
}

OptionIdentifier::Iterator OptionIdentifier::begin(void) const { return at(0); }

OptionIdentifier::Iterator OptionIdentifier::end(void) const {
    return OptionIdentifier::Iterator {
        .identifier = this,
        .index = depth(),
        .name = "",
        .state = OptionIdentifier::Iterator::OUT_OF_BOUNDS
    };
}

OptionIdentifier::Iterator::reference OptionIdentifier::Iterator::operator*(void) {
    return this->name;
}

OptionIdentifier::Iterator::pointer OptionIdentifier::Iterator::operator->(void) {
    return &(this->name);
}

OptionIdentifier::Iterator::reference OptionIdentifier::Iterator::operator[](
    difference_type offset
) {
    Iterator somewhere = (*this) + offset;
    return *somewhere;
}

OptionIdentifier::Iterator &OptionIdentifier::Iterator::operator+=(difference_type offset) {
    *this = this->identifier->at(this->index + offset);
    return *this;
}

OptionIdentifier::Iterator &OptionIdentifier::Iterator::operator-=(difference_type offset) {
    if (this->index < offset) {
        *this = this->identifier->end();
    } else {
        *this = this->identifier->at(this->index - offset);
    }
    return *this;
}

OptionIdentifier::Iterator OptionIdentifier::Iterator::operator+(
    difference_type offset
) const {
    OptionIdentifier::Iterator copy = *this;
    copy += offset;
    return copy;
}

OptionIdentifier::Iterator OptionIdentifier::Iterator::operator-(
    difference_type offset
) const {
    OptionIdentifier::Iterator copy = *this;
    copy -= offset;
    return copy;
}

OptionIdentifier::Iterator &OptionIdentifier::Iterator::operator++(void) {
    return *this += 1;
}

OptionIdentifier::Iterator OptionIdentifier::Iterator::operator++(int) {
    OptionIdentifier::Iterator copy = *this;
    *this += 1;
    return copy;
}

OptionIdentifier::Iterator &OptionIdentifier::Iterator::operator--(void) {
    return *this -= 1;
}

OptionIdentifier::Iterator OptionIdentifier::Iterator::operator--(int) {
    OptionIdentifier::Iterator copy = *this;
    *this -= 1;
    return copy;
}

/*

static inline bool _oiitComparable(
    const OptionIdentifier::Iterator &a,
    const OptionIdentifier::Iterator &b
) {
    return (a.identifier == b.identifier && a.state == b.state);
}

#define _OIIT_CMPEX(cmpOp) \
    ( \
        a.identifier == b.identifier && \
        ( \
            (a.state == b.state) && \
            ( \
                (a.state == OptionIdentifier::Iterator::VALID && a.index cmpOp b.index) || \
                a.state == OptionIdentifier::Iterator::OUT_OF_BOUNDS \
            ) \
        ) \
    ) \

bool Fidgety::operator==(
    const OptionIdentifier::Iterator &a,
    const OptionIdentifier::Iterator &b
) {
    spdlog::trace("[Fidgety::operator==] this is being used");
    return _OIIT_CMPEX(==);
}

bool Fidgety::operator!=(
    const OptionIdentifier::Iterator &a,
    const OptionIdentifier::Iterator &b
) {
    return !(a == b);
}

bool Fidgety::operator<(
    const OptionIdentifier::Iterator &a,
    const OptionIdentifier::Iterator &b
) {
    return _OIIT_CMPEX(<);
}

bool Fidgety::operator>(
    const OptionIdentifier::Iterator &a,
    const OptionIdentifier::Iterator &b
) {
    return _OIIT_CMPEX(>);
}

bool Fidgety::operator<=(
    const OptionIdentifier::Iterator &a,
    const OptionIdentifier::Iterator &b
) {
    return _OIIT_CMPEX(<=);
}

bool Fidgety::operator>=(
    const OptionIdentifier::Iterator &a,
    const OptionIdentifier::Iterator &b)
{
    return _OIIT_CMPEX(>=);
}

#undef _OIIT_CMP

*/

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
