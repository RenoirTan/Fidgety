/**
 * @file src/selector/selector.cpp
 * @author RenoirTan
 * @brief Source file for Fidgety's "selectors"
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 */

#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/regex.hpp>
#include <spdlog/spdlog.h>
#include <fidgety/appdata.hpp>
#include <fidgety/selector.hpp>

using namespace Fidgety;
namespace BoostFs = boost::filesystem;

std::string SelectorException::codeAsErrorType(void) const {
    switch (mCode) {
        case 0: return "Ok";
        case 1: return "InvalidInfo";
        case 2: return "MissingInfo";
        case 3: return "PartNotFound";
        default: return "Other";
    }
}

const char *SelectorException::getSimpleWhat(void) const noexcept {
    return "A Fidgety::SelectorException occurred.";
}

void ProcessedPartLocations::clear(void) {
    this->decoder.clear();
    this->encoder.clear();
    this->validator.clear();
    this->validatorContextCreator.clear();
}

Selector::Selector(const Appdata &appdata) : mAppdata(appdata) {
    spdlog::trace("[Fidgety::Selector::Selector] creating selector");
}

Selector::Selector(Appdata &&appdata) : mAppdata(std::move(appdata)) {
    spdlog::trace("[Fidgety::Selector::Selector] creating selector");
}

bool Selector::isValid(void) const {
    return checkValidity() == SelectorStatus::Ok;
}

SelectorStatus Selector::checkValidity(void) const {
    spdlog::debug("[Fidgety::Selector::checkValidity] checking if mAppdata is valid");
// Check if LoadablePartsFileNames is non-zero (not empty)
#define CHK_LPFN_NZ(part)                                   \
    if (mAppdata.loadablePartsFileNames.part.size() == 0) { \
        return SelectorStatus::MissingInfo;                 \
    }                                                       \

    CHK_LPFN_NZ(decoder);
    // The cases below are now handled by CHK_LPFN_NC
    // because if #second is empty, matchIt == #second.end()
    // CHK_LPFN_NZ(encoder);
    // CHK_LPFN_NZ(validator);
    // CHK_LPFN_NZ(validatorContextCreator);
#undef CHK_LPFN_NZ

// Check for no clashes
#define CHK_LPFN_NC(first, second)                                            \
    for (const auto &elemFromFirst : mAppdata.loadablePartsFileNames.first) { \
        auto matchIt = std::find(                                             \
            mAppdata.loadablePartsFileNames.second.begin(),                   \
            mAppdata.loadablePartsFileNames.second.end(),                     \
            elemFromFirst                                                     \
        );                                                                    \
        if (matchIt != mAppdata.loadablePartsFileNames.second.end()) {        \
            return SelectorStatus::InvalidInfo;                               \
        }                                                                     \
    }                                                                         \

    CHK_LPFN_NC(decoder, encoder);
    CHK_LPFN_NC(decoder         , validator);
    CHK_LPFN_NC(decoder                    , validatorContextCreator);
    CHK_LPFN_NC(         encoder, validator);
    CHK_LPFN_NC(         encoder           , validatorContextCreator);
    CHK_LPFN_NC(                  validator, validatorContextCreator);

#undef CHK_LPFN_NC

    return SelectorStatus::Ok;
}

SelectorStatus Selector::processHints(void) {
    spdlog::debug("[Fidgety::Selector::processHints] processing hints");
    ProcessedPartLocations ppl;

    for (const auto &spath : mAppdata.searchPaths) {
        spdlog::trace("[Fidgety::Selector::processHints] searching through '{}'", spath);
        BoostFs::path dir(spath);
        if (BoostFs::exists(dir) && BoostFs::is_directory(dir)) {
            spdlog::trace("[Fidgety::Selector::processHints] valid search path: {}", spath);

            spdlog::trace("[Fidgety::Selector::processHints] finding all files in search path");
            std::vector<std::string> dirFiles;
            BoostFs::directory_iterator itEnd;
            for (BoostFs::directory_iterator node(dir); node != itEnd; ++node) {
                BoostFs::path tentativePath = node->path();
                if (BoostFs::is_regular_file(tentativePath)) {
                    dirFiles.push_back(tentativePath.leaf().string());
                }
            }

            spdlog::trace("[Fidgety::Selector::processHints] looking through module file names");

#define INTERSECT_PPL(part)                                                         \
    for (const auto &moduleFileName : mAppdata.loadablePartsFileNames.part) {       \
        auto mfnDfIt = std::find(dirFiles.begin(), dirFiles.end(), moduleFileName); \
        if (mfnDfIt != dirFiles.end()) {                                            \
            ppl.part.push_back((dir / *mfnDfIt).string());                          \
        }                                                                           \
    }                                                                               \

            INTERSECT_PPL(decoder);
            INTERSECT_PPL(encoder);
            INTERSECT_PPL(validator);
            INTERSECT_PPL(validatorContextCreator);

#undef INTERSECT_PPL

        } else {
            spdlog::trace("[Fidgety::Selector::processHints] invalid search path");
        }
    }

    spdlog::debug("[Fidgety::Selector::processHints] using new ppl");

    mLocations = std::move(ppl);
    return SelectorStatus::Ok;
}

std::string Selector::getDecoderLocation(void) {
    if (mLocations.decoder.size() == 0) {
        return "";
    } else {
        return mLocations.decoder.at(0);
    }
}

std::string Selector::getEncoderLocation(void) {
    if (mLocations.encoder.size() == 0) {
        return "";
    } else {
        return mLocations.encoder.at(0);
    }
}

std::string Selector::getValidatorLocation(void) {
    if (mLocations.validator.size() == 0) {
        return "";
    } else {
        return mLocations.validator.at(0);
    }
}

std::string Selector::getValidatorContextCreatorLocation(void) {
    if (mLocations.validatorContextCreator.size() == 0) {
        return "";
    } else {
        return mLocations.validatorContextCreator.at(0);
    }
}
