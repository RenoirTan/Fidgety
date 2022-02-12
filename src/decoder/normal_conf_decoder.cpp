#include "spdlog/spdlog.h"
#include <fidgety/decoder.hpp>

using namespace Fidgety;

DecoderStatus NormalConfDecoder::dumpToIntermediate(void) {
    spdlog::trace("dumping NormalConfDecoder::mConfFile to NormalConfDecoder::mIntermediateFile");
    if (isConfOpened() && isIntermediateOpened()) {
        spdlog::debug(
            "successfully dumped NormalConfDecoder::mConfFile "
            "to NormalConfDecoder::mIntermediateFile"
        );
        return DecoderStatus::Ok;
    } else {
        spdlog::error(
            "NormalConfDecoder::mConfFile and NormalConfDecoder::mIntermediateFile not open ({0})",
            (((uint8_t) isConfOpened()) << 1) | ((uint8_t) isIntermediateOpened())
        );
        return DecoderStatus::FilesNotOpen;
    }
}
