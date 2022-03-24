/**
 * @file src/encoder/encoder.cpp
 * @author RenoirTan
 * @brief Translation unit for the base encoder class.
 * @version 0.1
 * @date 2022-02-17
 * 
 * @copyright Copyright (c) 2022
 */

#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <fidgety/encoder.hpp>
#include <fidgety/extensions.hpp>
#include <fidgety/_utils.hpp>

using namespace Fidgety;

std::string EncoderException::codeAsErrorType(void) const {
    switch (mCode) {
        case 0: return "Ok";
        case 1: return "FileNotFound";
        case 2: return "CannotReadFile";
        case 3: return "CannotWriteFile";
        case 4: return "CannotCloseFile";
        case 5: return "ResourceBusy";
        case 6: return "CannotOpenMultipleFiles";
        case 7: return "FilesNotOpen";
        case 8: return "SyntaxError";
        case 9: return "InvalidDataType";
        case 10: return "VerifierError";
        default: return "Other";
    }
}

const char *EncoderException::getSimpleWhat(void) const noexcept {
    return "A Fidgety::EncoderException occurred.";
}

Encoder::Encoder(void) noexcept {
    spdlog::debug("Encoder opened.");
}

Encoder::~Encoder(void) {
    spdlog::trace("destroying Encoder");
    if (isConfOpened()) {
        closeConf();
    }
    if (isIntermediateOpened()) {
        closeIntermediate();
    }
    spdlog::debug("Encoder files closed");
}

bool Encoder::isConfOpened(void) noexcept {
    spdlog::trace("checking if Encoder::mConfFile is open");
    return mConfFile.is_open();
}

EncoderStatus Encoder::openConf(const std::string &outPath) {
    spdlog::trace("opening Encoder::mConfFile with filepath: {0}", outPath);
    if (isConfOpened()) {
        FIDGETY_ERROR(
            EncoderException,
            EncoderStatus::CannotOpenMultipleFiles,
            "Encoder::mConfFile already open"
        );
    } else {
        mConfFile.open(outPath, std::ofstream::trunc);
        if (mConfFile.fail()) {
            FIDGETY_ERROR(
                EncoderException,
                EncoderStatus::CannotReadFile,
                "could not open Encoder::mConfFile with filepath: {0}",
                outPath
            );
        } else {
            spdlog::debug("Encoder::mConfFile opened with filepath: {0}", outPath);
        }
    }
    return EncoderStatus::Ok;
}

EncoderStatus Encoder::closeConf(void) {
    spdlog::trace("closing Encoder::mConfFile");
    if (isConfOpened()) {
        mConfFile.close();
        if (mConfFile.fail()) {
            FIDGETY_CRITICAL(
                EncoderException,
                EncoderStatus::CannotCloseFile,
                "could not close Encoder::mConfFile"
            );
        } else {
            spdlog::debug("closed Encoder::mConfFile");
        }
    } else {
        spdlog::warn("Encoder::mConfFile already closed");
    }
    return EncoderStatus::Ok;
}

EncoderStatus Encoder::useNewConf(std::ofstream &&newConf) {
    spdlog::trace("switching out Encoder::mConfFile using std::ifstream");
    if (isConfOpened()) {
        FIDGETY_ERROR(
            EncoderException,
            EncoderStatus::CannotOpenMultipleFiles,
            "Encoder::mConfFile already open"
        );
    } else {
        mConfFile = std::move(newConf);
        spdlog::debug("Encoder::mConfFile switched out for std::ifstream");
    }
    return EncoderStatus::Ok;
}

bool Encoder::isIntermediateOpened(void) noexcept {
    spdlog::trace("checking if Encoder::mIntermediateFile is open");
    return mIntermediateFile.is_open();
}

EncoderStatus Encoder::openIntermediate(const std::string &inPath) {
    spdlog::trace("opening Encoder::mIntermediateFile with filepath: {0}", inPath);
    if (isIntermediateOpened()) {
        FIDGETY_ERROR(
            EncoderException,
            EncoderStatus::CannotOpenMultipleFiles,
            "Encoder::mIntermediateFile already open"
        );
    } else {
        mIntermediateFile.open(inPath, std::ifstream::in);
        if (mIntermediateFile.fail()) {
            FIDGETY_ERROR(
                EncoderException,
                EncoderStatus::CannotReadFile,
                "could not open Encoder::mIntermediateFile with filepath: {0}",
                inPath
            );
        } else {
            spdlog::debug("Encoder::mIntermediateFile opened with filepath: {0}", inPath);
        }
    }
    return EncoderStatus::Ok;
}

EncoderStatus Encoder::closeIntermediate(void) {
    spdlog::trace("closing Encoder::mIntermediateFile");
    if (isIntermediateOpened()) {
        mIntermediateFile.close();
        if (mIntermediateFile.fail()) {
            FIDGETY_CRITICAL(
                EncoderException,
                EncoderStatus::CannotCloseFile,
                "could not close Encoder::mIntermediateFile"
            );
        } else {
            spdlog::debug("close Encoder::mIntermediateFile");
        }
    } else {
        spdlog::warn("Encoder::mIntermediateFile already closed");
    }
    return EncoderStatus::Ok;
}

EncoderStatus Encoder::useNewIntermediate(std::ifstream &&newIntermediate) {
    spdlog::trace("switching out Encoder::mINtermediateFile using std::ofstream");
    if (isIntermediateOpened()) {
        FIDGETY_ERROR(
            EncoderException,
            EncoderStatus::CannotOpenMultipleFiles,
            "Encoder::mIntermediateFile already opened"
        );
    } else {
        spdlog::trace("Encoder::mIntermediate switched out for std::ofstream");
        mIntermediateFile = std::move(newIntermediate);
    }
    return EncoderStatus::Ok;
}

EncoderStatus Encoder::dumpToConf(void) { return EncoderStatus::Ok; }
