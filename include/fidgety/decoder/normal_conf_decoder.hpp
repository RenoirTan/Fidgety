/**
 * @file include/fidgety/decoder/normal_conf_decoder.hpp
 * @author RenoirTan
 * @brief Header file for the default config file decoder
 * (Fidgety::NormalConfDecoder).
 * @version 0.1
 * @date 2022-02-18
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef FIDGETY_DECODER_NORMAL_CONF_DECODER_HPP
#   define FIDGETY_DECODER_NORMAL_CONF_DECODER_HPP

#   include <fidgety/decoder.hpp>

namespace Fidgety {
    class NormalConfDecoder : public Decoder {
        public:
            void dumpToIntermediate(void);
    };
}

#endif
