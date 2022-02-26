/**
 * @file include/fidgety/encoder/normal_conf_encoder.hpp
 * @author RenoirTan
 * @brief Header file for the default config file encoder
 * (Fidgety::NormalConfEncoder).
 * @version 0.1
 * @date 2022-02-18
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef FIDGETY_ENCODER_NORMAL_CONF_ENCODER_HPP
#   define FIDGETY_ENCODER_NORMAL_CONF_ENCODER_HPP

#   include <fidgety/encoder.hpp>

namespace Fidgety {
    class NormalConfEncoder : public Encoder {
        public:
            void dumpToConf(void);
    };
}

#endif
