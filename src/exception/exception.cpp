/**
 * @file src/exception/exception.cpp
 * @author RenoirTan
 * @brief Source file for the base Fidgety Exception class.
 * @version 0.1
 * @date 2022-02-20
 * 
 * @copyright Copyright (c) 2022
 */

#include <cstdlib>
#include <iostream>
#include "spdlog/spdlog.h"
#include <fidgety/exception.hpp>
#include <fidgety/_utils.hpp>

using namespace Fidgety;

Exception::Exception(void) noexcept : mCode(1) { }

Exception::Exception(const std::exception &origin) noexcept :
    mCode(1),
    mInformation(origin.what())
{ }

Exception::Exception(const Exception &origin) noexcept :
    mCode(origin.mCode),
    mInformation(origin.mInformation),
    mSupportingInformation(origin.mSupportingInformation)
{ }

Exception &Exception::operator=(const std::exception &origin) noexcept {
    mCode = 1;
    mInformation = origin.what();
    return *this;
}

Exception &Exception::operator=(const Exception &origin) noexcept {
    mCode = origin.mCode;
    mInformation = origin.mInformation;
    mSupportingInformation = origin.mSupportingInformation;
    return *this;
}

Exception::~Exception(void) { }

const char *Exception::what(void) const noexcept {
#ifdef FIDGETY_EXCEPTION_WHAT_USE_DESCRIPTION
    std::string description = getLongDescription();
    char *error_what = new char[description.length()+1];
    std::strcpy(error_what, description.c_str());
    return error_what;
#else
    return getSimpleWhat();
#endif
}

Exception::Exception(
    const int32_t code,
    const std::string &info,
    const std::string &supportingInfo
) :
    mCode(code),
    mInformation(info),
    mSupportingInformation(supportingInfo)
{ }

Exception::Exception(const int32_t code, const std::string &info) :
    mCode(code),
    mInformation(info)
{ }

Exception::Exception(const int32_t code, std::string &&info, std::string &&supportingInfo) :
    mCode(code),
    mInformation(std::move(info)),
    mSupportingInformation(std::move(supportingInfo))
{ }

Exception::Exception(const int32_t code, std::string &&info) :
    mCode(code),
    mInformation(std::move(info))
{ }

Exception::Exception(const int32_t code) :
    mCode(code)
{ }

int32_t Exception::getCode(void) const {
    return mCode;
}

std::string Exception::codeAsErrorType(void) const {
    return std::string();
}

const std::string &Exception::getInformation(void) const {
    return mInformation;
}

const std::string &Exception::getSupportingInformation(void) const {
    return mSupportingInformation;
}

std::string Exception::getShortDescription(void) const {
    return codeAsErrorType();
}

std::string Exception::getGenericDescription(void) const {
    return fmt::format("{0}: {1}", codeAsErrorType(), getInformation());
}

std::string Exception::getLongDescription(void) const {
    /*
    const std::string &supportingInfo = getSupportingInformation();
    if (!supportingInfo.empty()) {
        oss << "Details:";
        std::istringstream iss(supportingInfo);
        for (std::string line; std::getline(iss, line); ) {
            oss << std::endl << "\t" << line;
        }
    }
    */
    std::string supportingInfo = tabIndentSed(getSupportingInformation());
    if (supportingInfo.empty()) {
        return fmt::format("{0}\nNo extra details provided", getGenericDescription());
    } else {
        return fmt::format("{0}\nDetails:\n{1}", getGenericDescription(), supportingInfo);
    }
}

void Exception::exit(bool printToStdErr, bool outputToLog) {
    if (printToStdErr) {
        std::cerr << getLongDescription();
    }
    if (outputToLog) {
        spdlog::critical(getGenericDescription());
    }
    std::exit(getCode());
}

const char *Exception::getSimpleWhat(void) const noexcept {
    return "A Fidgety::Exception occurred.";
}
