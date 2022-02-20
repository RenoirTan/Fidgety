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
#include <sstream>
#include "spdlog/spdlog.h"
#include <fidgety/exception.hpp>
#include <fidgety/_utils.hpp>

using namespace Fidgety;

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
    std::ostringstream oss;
    oss << codeAsErrorType();
    return oss.str();
}

std::string Exception::getGenericDescription(void) const {
    std::ostringstream oss;
    oss << codeAsErrorType() << ": " << getInformation();
    return oss.str();
}

std::string Exception::getLongDescription(void) const {
    std::ostringstream oss;
    oss << codeAsErrorType() << ": " << getInformation() << std::endl;

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
        oss << "No extra details provided";
    } else {
        oss << "Details:" << std::endl << supportingInfo;
    }

    return oss.str();
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
