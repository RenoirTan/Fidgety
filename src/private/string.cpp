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
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void Fidgety::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void Fidgety::trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// empty or all spaces
bool Fidgety::isEffectivelyEmpty(const std::string &s) {
    return (
        s.empty() ||
        std::all_of(s.begin(), s.end(), [](char c) { return std::isspace(c); })
    );
}

std::string Fidgety::sed(const std::string &s, StringEditor &m) {
    if (s.empty()) {
        return "";
    }
    std::istringstream iss(s);
    std::ostringstream oss;
    std::string line;
    if (std::getline(iss, line)) {
        m.mutateString(line);
        oss << line;
    } else {
        return "";
    }
    while (std::getline(iss, line)) {
        m.mutateString(line);
        oss << std::endl << line;
    }
    return oss.str();
}

std::string Fidgety::tabIndentSed(const std::string &s, uint32_t tabs) {
    StringIndenter indenter = StringIndenter(true, tabs);
    return sed(s, indenter);
}

std::string Fidgety::spaceIndentSed(const std::string &s, uint32_t spaces) {
    StringIndenter indenter = StringIndenter(false, spaces);
    return sed(s, indenter);
}
