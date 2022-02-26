/**
 * @file src/encoder/encoder.cpp
 * @author RenoirTan
 * @brief Translation unit for the base encoder class.
 * @version 0.1
 * @date 2022-02-17
 * 
 * @copyright Copyright (c) 2022
 */

#include <sstream>
#include "spdlog/spdlog.h"
#include <fidgety/encoder.hpp>

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

void Encoder::openConf(const std::string &outPath) {
    spdlog::trace("opening Encoder::mConfFile with filepath: {0}", outPath);
    if (isConfOpened()) {
        const std::string error_msg = "Encoder::mConfFile already open";
        spdlog::error(error_msg);
        throw EncoderException((int32_t) EncoderStatus::CannotOpenMultipleFiles, error_msg);
    } else {
        mConfFile.open(outPath, std::ofstream::trunc);
        if (mConfFile.fail()) {
            std::ostringstream oss;
            oss << "could not open Encoder::mConfFile with filepath: " << outPath;
            const std::string error_msg = oss.str();
            spdlog::error(error_msg);
            throw EncoderException((int32_t) EncoderStatus::CannotReadFile, error_msg);
        } else {
            spdlog::debug("Encoder::mConfFile opened with filepath: {0}", outPath);
        }
    }
}

void Encoder::closeConf(void) {
    spdlog::trace("closing Encoder::mConfFile");
    if (isConfOpened()) {
        mConfFile.close();
        if (mConfFile.fail()) {
            const std::string error_msg = "could not close Encoder::mConfFile";
            spdlog::error(error_msg);
            throw EncoderException((int32_t) EncoderStatus::CannotCloseFile, error_msg);
        } else {
            spdlog::debug("closed Encoder::mConfFile");
        }
    } else {
        spdlog::debug("Encoder::mConfFile already closed");
    }
}

void Encoder::useNewConf(std::ofstream &&newConf) {
    spdlog::trace("switching out Encoder::mConfFile using std::ifstream");
    if (isConfOpened()) {
        const std::string error_msg = "Encoder::mConfFile already open";
        spdlog::error(error_msg);
        throw EncoderException((int32_t) EncoderStatus::CannotOpenMultipleFiles, error_msg);
    } else {
        mConfFile = std::move(newConf);
        spdlog::debug("Encoder::mConfFile switched out for std::ifstream");
    }
}

bool Encoder::isIntermediateOpened(void) noexcept {
    spdlog::trace("checking if Encoder::mIntermediateFile is open");
    return mIntermediateFile.is_open();
}

void Encoder::openIntermediate(const std::string &inPath) {
    spdlog::trace("opening Encoder::mIntermediateFile with filepath: {0}", inPath);
    if (isIntermediateOpened()) {
        const std::string error_msg = "Encoder::mIntermediateFile already open";
        spdlog::error(error_msg);
        throw EncoderException((int32_t) EncoderStatus::CannotOpenMultipleFiles, error_msg);
    } else {
        mIntermediateFile.open(inPath, std::ifstream::in);
        if (mIntermediateFile.fail()) {
            std::ostringstream oss;
            oss << "could not open Encoder::mIntermediateFile with filepath: " << inPath;
            const std::string error_msg = oss.str();
            spdlog::error(error_msg);
            throw EncoderException((int32_t) EncoderStatus::CannotReadFile, error_msg);
        } else {
            spdlog::debug("Encoder::mIntermediateFile opened with filepath: {0}", inPath);
        }
    }
}

void Encoder::closeIntermediate(void) {
    spdlog::trace("closing Encoder::mIntermediateFile");
    if (isIntermediateOpened()) {
        mIntermediateFile.close();
        if (mIntermediateFile.fail()) {
            const std::string error_msg = "could not close Encoder::mIntermediateFile";
            spdlog::error(error_msg);
            throw EncoderException((int32_t) EncoderStatus::CannotCloseFile, error_msg);
        } else {
            spdlog::debug("close Encoder::mIntermediateFile");
        }
    } else {
        spdlog::debug("Encoder::mIntermediateFile already closed");
    }
}

void Encoder::useNewIntermediate(std::ifstream &&newIntermediate) {
    spdlog::trace("switching out Encoder::mINtermediateFile using std::ofstream");
    if (isIntermediateOpened()) {
        const std::string error_msg = "Encoder::mIntermediateFile already opened";
        spdlog::error(error_msg);
        throw EncoderException((int32_t) EncoderStatus::CannotOpenMultipleFiles, error_msg);
    } else {
        spdlog::trace("Encoder::mIntermediate switched out for std::ofstream");
        mIntermediateFile = std::move(newIntermediate);
    }
}

void Encoder::dumpToConf(void) { }
