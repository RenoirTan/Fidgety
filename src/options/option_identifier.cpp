/**
 * @file src/option_identifier.cpp
 * @author RenoirTan
 * @brief Implementation of Fidgety::OptionIdentifier
 * @version 0.1
 * @date 2022-04-16
 * 
 * @copyright Copyright (c) 2022
 */

#include <boost/algorithm/string.hpp>
#include <spdlog/spdlog.h>
#include <fidgety/_utils.hpp>
#include <fidgety/options.hpp>

using namespace Fidgety;

const size_t OptionIdentifier::npos = std::string::npos;

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

static bool _validateOptionIdentifierName(const std::string &name) {
    return !(name.empty() || (std::isdigit(name[0]) && !isDecimalInteger(name)));
}

bool OptionIdentifier::isValid(void) const noexcept {
    if (mPath.empty()) {
        return false;
    }
    std::vector<OptionName> splat = split();
    for (const auto &name : splat) {
        if (!_validateOptionIdentifierName(name)) {
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

OptionIdentifier &OptionIdentifier::operator+=(const std::string &addon) {
    if (!_validateOptionIdentifierName(addon)) {
        FIDGETY_CRITICAL(
            OptionException,
            OptionStatus::InvalidName,
            "[Fidgety::OptionIdentifier::operator+=] bad addon: '{}'",
            addon
        );
    }
    mPath += '.';
    mPath += addon;
    return *this;
}

OptionIdentifier OptionIdentifier::operator+(const std::string &addon) const {
    OptionIdentifier identifier(*this);
    identifier += addon;
    return identifier;
}

// shitty O(n^2) algo
size_t OptionIdentifier::findSubset(const OptionIdentifier &identifier) const {
    const size_t myDepth = depth();
    const size_t otherDepth = identifier.depth();
    if (myDepth < otherDepth) {
        return npos;
    }
    const auto itOtherBegin = identifier.begin();
    const auto itOtherEnd = identifier.end();
    for (size_t index = 0; index < myDepth; ++index) {
        // no possible matches
        if ((myDepth-index) < otherDepth) {
            return npos;
        }
        auto itMe = at(index);
        bool found = true;
        for (auto itOther = itOtherBegin; itOther != itOtherEnd; ++itOther) {
            if (*itMe != *itOther) {
                found = false;
                break;
            }
            ++itMe;
        }
        if (found) {
            return index;
        }
    }
    return npos;
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
