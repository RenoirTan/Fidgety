/**
 * @file src/decoder/decoder.cpp
 * @author RenoirTan
 * @brief Translation unit containing the implementation for the base decoder
 * class.
 * @version 0.1
 * @date 2022-02-17
 * 
 * @copyright Copyright (c) 2022
 */

#include <sstream>
#include "spdlog/spdlog.h"
#include <fidgety/decoder.hpp>

using namespace Fidgety;

std::string DecoderException::codeAsErrorType(void) const {
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
        default: return "Other";
    }
}

const char *DecoderException::getSimpleWhat(void) const noexcept {
    return "A Fidgety::DecoderException occurred.";
}

Decoder::Decoder(void) noexcept {
    spdlog::debug("Decoder opened");
}

Decoder::~Decoder(void) {
    spdlog::trace("destroying Decoder");
    if (isConfOpened()) {
        closeConf();
    }
    if (isIntermediateOpened()) {
        closeIntermediate();
    }
    spdlog::debug("Decoder files closed");
}

bool Decoder::isConfOpened(void) noexcept {
    spdlog::trace("checking if Decoder::mConfFile is open");
    return mConfFile.is_open();
}

void Decoder::openConf(const std::string &inPath) {
    spdlog::trace("opening Decoder::mConfFile with filepath: {0}", inPath);
    if (isConfOpened()) {
        const std::string error_msg = "Decoder::mConfFile already open";
        spdlog::error(error_msg);
        DecoderException exception((int32_t) DecoderStatus::CannotOpenMultipleFiles, error_msg);
        throw exception;
    } else {
        mConfFile.open(inPath, std::ifstream::in);
        if (mConfFile.fail()) {
            spdlog::error("could not open Decoder::mConfFile with filepath: {0}", inPath);
            DecoderException exception((int32_t) DecoderStatus::CannotReadFile);
            throw exception;
        } else {
            spdlog::debug("Decoder::mConfFile opened with filepath: {0}", inPath);
        }
    }
}

void Decoder::closeConf(void) {
    spdlog::trace("closing Decoder::mConfFile");
    if (isConfOpened()) {
        mConfFile.close();
        if (mConfFile.fail()) {
            const std::string error_msg = "could not close Decoder::mConfFile";
            spdlog::error(error_msg);
            throw DecoderException((int32_t) DecoderStatus::CannotCloseFile, error_msg);
        } else {
            spdlog::debug("closed Decoder::mConfFile");
        }
    } else {
        spdlog::debug("Decoder::mConfFile already closed");
    }
}

void Decoder::useNewConf(std::ifstream &&newConf) {
    spdlog::trace("switching out Decoder::mConfFile using std::ifstream");
    if (isConfOpened()) {
        const std::string error_msg = "Decoder::mConfFile already open";
        spdlog::error(error_msg);
        throw DecoderException((int32_t) DecoderStatus::CannotOpenMultipleFiles, error_msg);
    } else {
        mConfFile = std::move(newConf);
        spdlog::debug("Decoder::mConfFile switched out for std::ifstream");
    }
}

bool Decoder::isIntermediateOpened(void) noexcept {
    spdlog::trace("checking if Decoder::mIntermediateFile is open");
    return mIntermediateFile.is_open();
}

void Decoder::openIntermediate(const std::string &outPath) {
    spdlog::trace("opening Decoder::mIntermediateFile with filepath: {0}", outPath);
    if (isIntermediateOpened()) {
        const std::string error_msg = "Decoder::mIntermediateFile already open";
        spdlog::error(error_msg);
        throw DecoderException((int32_t) DecoderStatus::CannotOpenMultipleFiles, error_msg);
    } else {
        mIntermediateFile.open(outPath, std::ofstream::trunc);
        if (mIntermediateFile.fail()) {
            std::ostringstream oss;
            oss << "could not open Decoder::mIntermediateFile with filepath: " << outPath;
            const std::string error_msg = oss.str();
            spdlog::error(error_msg);
            throw DecoderException((int32_t) DecoderStatus::CannotReadFile, error_msg);
        } else {
            spdlog::debug("Decoder::mIntermediateFile opened with filepath: {0}", outPath);
        }
    }
}

void Decoder::closeIntermediate(void) {
    spdlog::trace("closing Decoder::mIntermediateFile");
    if (isIntermediateOpened()) {
        mIntermediateFile.close();
        if (mIntermediateFile.fail()) {
            const std::string error_msg = "could not close Decoder::mIntermediateFile";
            spdlog::error(error_msg);
            throw DecoderException((int32_t) DecoderStatus::CannotCloseFile, error_msg);
        } else {
            spdlog::debug("close Decoder::mIntermediateFile");
        }
    } else {
        spdlog::debug("Decoder::mIntermediateFile already closed");
    }
}

void Decoder::useNewIntermediate(std::ofstream &&newIntermediate) {
    spdlog::trace("switching out Decoder::mIntermediateFile using std::ofstream");
    if (isIntermediateOpened()) {
        const std::string error_msg = "Decoder::mIntermediateFile already opened";
        spdlog::error(error_msg);
        throw DecoderException((int32_t) DecoderStatus::CannotOpenMultipleFiles, error_msg);
    } else {
        spdlog::trace("Decoder::mIntermediate switched out for std::ofstream");
        mIntermediateFile = std::move(newIntermediate);
    }
}

void Decoder::dumpToIntermediate(void) { }
