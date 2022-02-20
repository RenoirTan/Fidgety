/**
 * @file src/encoder/encoder.cpp
 * @author RenoirTan
 * @brief Translation unit for the base encoder class.
 * @version 0.1
 * @date 2022-02-17
 * 
 * @copyright Copyright (c) 2022
 */

#include "spdlog/spdlog.h"
#include <fidgety/encoder.hpp>

using namespace Fidgety;

Encoder::Encoder(void) {
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

bool Encoder::isConfOpened(void) {
    spdlog::trace("checking if Encoder::mConfFile is open");
    return mConfFile.is_open();
}

EncoderStatus Encoder::openConf(const std::string &outPath) {
    spdlog::trace("opening Encoder::mConfFile with filepath: {0}", outPath);
    if (isConfOpened()) {
        spdlog::error("Encoder::mConfFile already open");
        return EncoderStatus::CannotOpenMultipleFiles;
    } else {
        mConfFile.open(outPath, std::ofstream::trunc);
        if (mConfFile.fail()) {
            spdlog::error("could not open Encoder::mConfFile with filepath: {0}", outPath);
            return EncoderStatus::CannotReadFile;
        } else {
            spdlog::debug("Encoder::mConfFile opened with filepath: {0}", outPath);
            return EncoderStatus::Ok;
        }
    }
}

EncoderStatus Encoder::closeConf(void) {
    spdlog::trace("closing Encoder::mConfFile");
    if (isConfOpened()) {
        mConfFile.close();
        if (mConfFile.fail()) {
            spdlog::error("could not close Encoder::mConfFile");
            return EncoderStatus::CannotCloseFile;
        } else {
            spdlog::debug("closed Encoder::mConfFile");
            return EncoderStatus::Ok;
        }
    } else {
        spdlog::debug("Encoder::mConfFile already closed");
        return EncoderStatus::Ok;
    }
}

EncoderStatus Encoder::useNewConf(std::ofstream &&newConf) {
    spdlog::trace("switching out Encoder::mConfFile using std::ifstream");
    if (isConfOpened()) {
        spdlog::error("Encoder::mConfFile already open");
        return EncoderStatus::CannotOpenMultipleFiles;
    } else {
        mConfFile = std::move(newConf);
        spdlog::debug("Encoder::mConfFile switched out for std::ifstream");
        return EncoderStatus::Ok;
    }
}

bool Encoder::isIntermediateOpened(void) {
    spdlog::trace("checking if Encoder::mIntermediateFile is open");
    return mIntermediateFile.is_open();
}

EncoderStatus Encoder::openIntermediate(const std::string &inPath) {
    spdlog::trace("opening Encoder::mIntermediateFile with filepath: {0}", inPath);
    if (isIntermediateOpened()) {
        spdlog::error("Encoder::mIntermediateFile already open");
        return EncoderStatus::CannotOpenMultipleFiles;
    } else {
        mIntermediateFile.open(inPath, std::ifstream::in);
        if (mIntermediateFile.fail()) {
            spdlog::error("could not open Encoder::mIntermediateFile with filepath: {0}", inPath);
            return EncoderStatus::CannotReadFile;
        } else {
            spdlog::debug("Encoder::mIntermediateFile opened with filepath: {0}", inPath);
            return EncoderStatus::Ok;
        }
    }
}

EncoderStatus Encoder::closeIntermediate(void) {
    spdlog::trace("closing Encoder::mIntermediateFile");
    if (isIntermediateOpened()) {
        mIntermediateFile.close();
        if (mIntermediateFile.fail()) {
            spdlog::error("could not close Encoder::mIntermediateFile");
            return EncoderStatus::CannotCloseFile;
        } else {
            spdlog::debug("close Encoder::mIntermediateFile");
            return EncoderStatus::Ok;
        }
    } else {
        spdlog::debug("Encoder::mIntermediateFile already closed");
        return EncoderStatus::Ok;
    }
}

EncoderStatus Encoder::useNewIntermediate(std::ifstream &&newIntermediate) {
    spdlog::trace("switching out Encoder::mINtermediateFile using std::ofstream");
    if (isIntermediateOpened()) {
        spdlog::error("Encoder::mIntermediateFile already opened");
        return EncoderStatus::CannotOpenMultipleFiles;
    } else {
        spdlog::trace("Encoder::mIntermediate switched out for std::ofstream");
        mIntermediateFile = std::move(newIntermediate);
        return EncoderStatus::Ok;
    }
}

EncoderStatus Encoder::dumpToConf(void) { return EncoderStatus::Ok; }
