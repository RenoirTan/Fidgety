#include <fidgety/decoder.hpp>

using namespace Fidgety;

Decoder::Decoder(void) { }

Decoder::~Decoder(void) {
    if (isConfOpened()) {
        closeConf();
    }
    if (isIntermediateOpened()) {
        closeIntermediate();
    }
}

bool Decoder::isConfOpened(void) {
    return mConfFile.is_open();
}

DecoderStatus Decoder::openConf(std::string &inPath) {
    if (isConfOpened()) {
        return DecoderStatus::CannotOpenMultipleFiles;
    } else {
        mConfFile.open(inPath, std::ifstream::in);
        if (mConfFile.fail()) {
            return DecoderStatus::CannotReadFile;
        } else {
            return DecoderStatus::Ok;
        }
    }
}

DecoderStatus Decoder::closeConf(void) {
    if (!isConfOpened()) {
        mConfFile.close();
        return mConfFile.fail() ? DecoderStatus::CannotCloseFile : DecoderStatus::Ok;
    } else {
        return DecoderStatus::Ok;
    }
}

DecoderStatus Decoder::useNewConf(std::ifstream &&newConf) {
    if (isConfOpened()) {
        return DecoderStatus::CannotOpenMultipleFiles;
    } else {
        mConfFile = std::move(newConf);
        return DecoderStatus::Ok;
    }
}

bool Decoder::isIntermediateOpened(void) {
    return mIntermediateFile.is_open();
}

DecoderStatus Decoder::openIntermediate(std::string &outPath) {
    if (isIntermediateOpened()) {
        return DecoderStatus::CannotOpenMultipleFiles;
    } else {
        mIntermediateFile.open(outPath, std::ofstream::trunc);
        if (mIntermediateFile.fail()) {
            return DecoderStatus::CannotReadFile;
        } else {
            return DecoderStatus::Ok;
        }
    }
}

DecoderStatus Decoder::closeIntermediate(void) {
    if (!isIntermediateOpened()) {
        mIntermediateFile.close();
        return mIntermediateFile.fail() ? DecoderStatus::CannotCloseFile : DecoderStatus::Ok;
    } else {
        return DecoderStatus::Ok;
    }
}

DecoderStatus Decoder::useNewIntermediate(std::ofstream &&newIntermediate) {
    if (isIntermediateOpened()) {
        return DecoderStatus::CannotOpenMultipleFiles;
    } else {
        mIntermediateFile = std::move(newIntermediate);
        return DecoderStatus::Ok;
    }
}
