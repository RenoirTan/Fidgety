/**
 * @file src/private/string.cpp
 * @author your name (you@domain.com)
 * @brief Private string utilities.
 * @version 0.1
 * @date 2022-02-17
 * 
 * @copyright Copyright (c) 2022
 */

#include <algorithm>
#include <sstream>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <fidgety/_utils.hpp>

using namespace Fidgety;

void StringEditor::mutateString(std::string &s) { }
std::string StringEditor::convertString(const std::string &s) { return s; }

// This is my bias showing
StringIndenter::StringIndenter(void) :
    mHeader(4, ' ')
{ }

StringIndenter::StringIndenter(bool useTabs) {
    if (useTabs) {
        mHeader = std::string(1, '\t');
    } else {
        mHeader = std::string(4, ' ');
    }
}

StringIndenter::StringIndenter(size_t size) :
    mHeader(size, ' ')
{ }

StringIndenter::StringIndenter(bool useTabs, size_t size) :
    mHeader(size, useTabs ? '\t' : ' ')
{ }

void StringIndenter::mutateString(std::string &s) {
    s.insert(0, mHeader);
}

std::string StringIndenter::convertString(const std::string &s) {
    std::string result = s;
    mutateString(result);
    return result;
}

StringIndenter StringIndenter::tabs(void) {
    return StringIndenter(true);
}

StringIndenter StringIndenter::spaces(void) {
    return StringIndenter(false);
}

// https://stackoverflow.com/a/217605
// trim from start (in place)
void Fidgety::ltrim(std::string &s) {
    boost::trim_left(s);
}

// trim from end (in place)
void Fidgety::rtrim(std::string &s) {
    boost::trim_right(s);
}

// trim from both ends (in place)
void Fidgety::trim(std::string &s) {
    boost::trim(s);
}

void Fidgety::truncateAfter(std::string &s, const std::string &b, bool caseInsensitive) {
    auto sloc = caseInsensitive ? boost::ifind_first(s, b) : boost::find_first(s, b);
    s.erase(sloc.begin(), s.end());
}

std::string Fidgety::truncateAfterCopy(
    const std::string &s,
    const std::string &b,
    bool caseInsensitive
) {
    std::string c = s;
    truncateAfter(c, b, caseInsensitive);
    return c;
}

// empty or all spaces
bool Fidgety::isEffectivelyEmpty(const std::string &s) {
    return boost::all(s, boost::is_space());
}

bool Fidgety::isDecimalInteger(const std::string &s) {
    return boost::all(s, boost::is_digit());
}

size_t Fidgety::countSubstr(const std::string &s, const std::string &substr) {
    if (substr.empty()) {
        return 0;
    }
    size_t count = 0;
    for (
        size_t lastFound = s.find(substr);
        lastFound != std::string::npos;
        lastFound = s.find(substr, lastFound + substr.size())
    ) {
        ++count;
    }
    return count;
}

std::string Fidgety::sed(const std::string &s, StringEditor *m) {
    if (s.empty()) {
        return "";
    }
    std::istringstream iss(s);
    std::ostringstream oss;
    std::string line;
    if (std::getline(iss, line)) {
        m->mutateString(line);
        oss << line;
    } else {
        return "";
    }
    while (std::getline(iss, line)) {
        m->mutateString(line);
        oss << '\n' << line;
    }
    return oss.str();
}

std::string Fidgety::tabIndentSed(const std::string &s, uint32_t tabs) {
    StringIndenter indenter = StringIndenter(true, tabs);
    return sed(s, &indenter);
}

std::string Fidgety::spaceIndentSed(const std::string &s, uint32_t spaces) {
    StringIndenter indenter = StringIndenter(false, spaces);
    return sed(s, &indenter);
}
