#include <fidgety/decoder.hpp>

using namespace Fidgety;

DecoderStatus NormalConfDecoder::dumpToIntermediate(void) {
    if (isConfOpened() && isIntermediateOpened()) {
        return DecoderStatus::Ok;
    } else {
        return DecoderStatus::FilesNotOpen;
    }
}
