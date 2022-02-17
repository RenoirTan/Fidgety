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

#include "spdlog/spdlog.h"
#include <fidgety/decoder.hpp>

using namespace Fidgety;

Decoder::Decoder(void) {
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

bool Decoder::isConfOpened(void) {
    spdlog::trace("checking if Decoder::mConfFile is open");
    return mConfFile.is_open();
}

DecoderStatus Decoder::openConf(const std::string &inPath) {
    spdlog::trace("opening Decoder::mConfFile with filepath: {0}", inPath);
    if (isConfOpened()) {
        spdlog::error("Decoder::mConfFile already open");
        return DecoderStatus::CannotOpenMultipleFiles;
    } else {
        mConfFile.open(inPath, std::ifstream::in);
        if (mConfFile.fail()) {
            spdlog::error("could not open Decoder::mConfFile with filepath: {0}", inPath);
            return DecoderStatus::CannotReadFile;
        } else {
            spdlog::debug("Decoder::mConfFile opened with filepath: {0}", inPath);
            return DecoderStatus::Ok;
        }
    }
}

DecoderStatus Decoder::closeConf(void) {
    spdlog::trace("closing Decoder::mConfFile");
    if (isConfOpened()) {
        mConfFile.close();
        if (mConfFile.fail()) {
            spdlog::error("could not close Decoder::mConfFile");
            return DecoderStatus::CannotCloseFile;
        } else {
            spdlog::debug("closed Decoder::mConfFile");
            return DecoderStatus::Ok;
        }
    } else {
        spdlog::debug("Decoder::mConfFile already closed");
        return DecoderStatus::Ok;
    }
}

DecoderStatus Decoder::useNewConf(std::ifstream &&newConf) {
    spdlog::trace("switching out Decoder::mConfFile using std::ifstream");
    if (isConfOpened()) {
        spdlog::error("Decoder::mConfFile already open");
        return DecoderStatus::CannotOpenMultipleFiles;
    } else {
        mConfFile = std::move(newConf);
        spdlog::debug("Decoder::mConfFile switched out for std::ifstream");
        return DecoderStatus::Ok;
    }
}

bool Decoder::isIntermediateOpened(void) {
    spdlog::trace("checking if Decoder::mIntermediateFile is open");
    return mIntermediateFile.is_open();
}

DecoderStatus Decoder::openIntermediate(const std::string &outPath) {
    spdlog::trace("opening Decoder::mIntermediateFile with filepath: {0}", outPath);
    if (isIntermediateOpened()) {
        spdlog::error("Decoder::mIntermediateFile already open");
        return DecoderStatus::CannotOpenMultipleFiles;
    } else {
        mIntermediateFile.open(outPath, std::ofstream::trunc);
        if (mIntermediateFile.fail()) {
            spdlog::error("could not open Decoder::mIntermediateFile with filepath: {0}", outPath);
            return DecoderStatus::CannotReadFile;
        } else {
            spdlog::debug("Decoder::mIntermediateFile opened with filepath: {0}", outPath);
            return DecoderStatus::Ok;
        }
    }
}

DecoderStatus Decoder::closeIntermediate(void) {
    spdlog::trace("closing Decoder::mIntermediateFile");
    if (isIntermediateOpened()) {
        mIntermediateFile.close();
        if (mIntermediateFile.fail()) {
            spdlog::error("could not close Decoder::mIntermediateFile");
            return DecoderStatus::CannotCloseFile;
        } else {
            spdlog::debug("close Decoder::mIntermediateFile");
            return DecoderStatus::Ok;
        }
    } else {
        spdlog::debug("Decoder::mIntermediateFile already closed");
        return DecoderStatus::Ok;
    }
}

DecoderStatus Decoder::useNewIntermediate(std::ofstream &&newIntermediate) {
    spdlog::trace("switching out Decoder::mIntermediateFile using std::ofstream");
    if (isIntermediateOpened()) {
        spdlog::error("Decoder::mIntermediateFile already opened");
        return DecoderStatus::CannotOpenMultipleFiles;
    } else {
        spdlog::trace("Decoder::mIntermediate switched out for std::ofstream");
        mIntermediateFile = std::move(newIntermediate);
        return DecoderStatus::Ok;
    }
}
