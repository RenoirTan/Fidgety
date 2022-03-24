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

#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <fidgety/decoder.hpp>
#include <fidgety/extensions.hpp>
#include <fidgety/_utils.hpp>

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

DecoderStatus Decoder::openConf(const std::string &inPath) {
    spdlog::trace("opening Decoder::mConfFile with filepath: {0}", inPath);
    if (isConfOpened()) {
        FIDGETY_ERROR(
            DecoderException,
            DecoderStatus::CannotOpenMultipleFiles,
            "Decoder::mConfFile already open"
        );
    } else {
        mConfFile.open(inPath, std::ifstream::in);
        if (mConfFile.fail()) {
            FIDGETY_ERROR(
                DecoderException,
                DecoderStatus::CannotReadFile,
                "could not open Decoder::mConfFile with filepath: {0}",
                inPath
            );
        } else {
            spdlog::debug("Decoder::mConfFile opened with filepath: {0}", inPath);
        }
    }
    return DecoderStatus::Ok;
}

DecoderStatus Decoder::closeConf(void) {
    spdlog::trace("closing Decoder::mConfFile");
    if (isConfOpened()) {
        mConfFile.close();
        if (mConfFile.fail()) {
            FIDGETY_CRITICAL(
                DecoderException,
                DecoderStatus::CannotCloseFile,
                "could not close Decoder::mConfFile"
            );
        } else {
            spdlog::debug("closed Decoder::mConfFile");
        }
    } else {
        spdlog::warn("Decoder::mConfFile already closed");
    }
    return DecoderStatus::Ok;
}

DecoderStatus Decoder::useNewConf(std::ifstream &&newConf) {
    spdlog::trace("switching out Decoder::mConfFile using std::ifstream");
    if (isConfOpened()) {
        FIDGETY_ERROR(
            DecoderException,
            DecoderStatus::CannotOpenMultipleFiles,
            "Decoder::mConfFile already open"
        );
    } else {
        mConfFile = std::move(newConf);
        spdlog::debug("Decoder::mConfFile switched out for std::ifstream");
    }
    return DecoderStatus::Ok;
}

bool Decoder::isIntermediateOpened(void) noexcept {
    spdlog::trace("checking if Decoder::mIntermediateFile is open");
    return mIntermediateFile.is_open();
}

DecoderStatus Decoder::openIntermediate(const std::string &outPath) {
    spdlog::trace("opening Decoder::mIntermediateFile with filepath: {0}", outPath);
    if (isIntermediateOpened()) {
        FIDGETY_ERROR(
            DecoderException,
            DecoderStatus::CannotOpenMultipleFiles,
            "Decoder::mIntermediateFile already open"
        );
    } else {
        mIntermediateFile.open(outPath, std::ofstream::trunc);
        if (mIntermediateFile.fail()) {
            FIDGETY_ERROR(
                DecoderException,
                DecoderStatus::CannotReadFile,
                "could not open Decoder::mIntermediateFile with filepath: {0}",
                outPath
            );
        } else {
            spdlog::debug("Decoder::mIntermediateFile opened with filepath: {0}", outPath);
        }
    }
    return DecoderStatus::Ok;
}

DecoderStatus Decoder::closeIntermediate(void) {
    spdlog::trace("closing Decoder::mIntermediateFile");
    if (isIntermediateOpened()) {
        mIntermediateFile.close();
        if (mIntermediateFile.fail()) {
            FIDGETY_CRITICAL(
                DecoderException,
                DecoderStatus::CannotCloseFile,
                "could not close Decoder::mIntermediateFile"
            );
        } else {
            spdlog::debug("close Decoder::mIntermediateFile");
        }
    } else {
        spdlog::warn("Decoder::mIntermediateFile already closed");
    }
    return DecoderStatus::Ok;
}

DecoderStatus Decoder::useNewIntermediate(std::ofstream &&newIntermediate) {
    spdlog::trace("switching out Decoder::mIntermediateFile using std::ofstream");
    if (isIntermediateOpened()) {
        FIDGETY_ERROR(
            DecoderException,
            DecoderStatus::CannotOpenMultipleFiles,
            "Decoder::mIntermediateFile already opened"
        );
    } else {
        spdlog::trace("Decoder::mIntermediate switched out for std::ofstream");
        mIntermediateFile = std::move(newIntermediate);
    }
    return DecoderStatus::Ok;
}

DecoderStatus Decoder::dumpToIntermediate(void) { return DecoderStatus::Ok; }
