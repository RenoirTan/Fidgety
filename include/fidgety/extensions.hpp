/**
 * @file include/fidgety/extensions.hpp
 * @author RenoirTan
 * @brief Common declarations for extension modules.
 * @version 0.1
 * @date 2022-03-24
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef FIDGETY_EXTENSIONS_HPP
#   define FIDGETY_EXTENSIONS_HPP

#   define FIDGETY_DECODER_ALLOC_SYMBOL fidgetyDecoderAlloc
#   define FIDGETY_DECODER_DELETE_SYMBOL fidgetyDecoderDelete
#   define FIDGETY_ENCODER_ALLOC_SYMBOL fidgetyEncoderAlloc
#   define FIDGETY_ENCODER_DELETE_SYMBOL fidgetyEncoderDelete
#   define FIDGETY_VALIDATOR_ALLOC_SYMBOL fidgetyValidatorAlloc
#   define FIDGETY_VALIDATOR_DELETE_SYMBOL fidgetyValidatorDelete
#   define FIDGETY_VCC_ALLOC_SYMBOL fidgetyVccAlloc
#   define FIDGETY_VCC_DELETE_SYMBOL fidgetyVccDelete

#   define FIDGETY_DELETE_PARAM_NAME __fidgety_obj

#   define FIDGETY_DECODER_ALLOC_PROT() Fidgety::Decoder *FIDGETY_DECODER_ALLOC_SYMBOL(void)
#   define FIDGETY_DECODER_DELETE_PROT() \
    void FIDGETY_DECODER_DELETE_SYMBOL(Fidgety::Decoder *FIDGETY_DELETE_PARAM_NAME)

#   define FIDGETY_ENCODER_ALLOC_PROT() Fidgety::Encoder *FIDGETY_ENCODER_ALLOC_SYMBOL(void)
#   define FIDGETY_ENCODER_DELETE_PROT() \
    void FIDGETY_ENCODER_DELETE_SYMBOL(Fidgety::Encoder *FIDGETY_DELETE_PARAM_NAME)

#   define FIDGETY_VALIDATOR_ALLOC_PROT() Fidgety::Validator *FIDGETY_VALIDATOR_ALLOC_SYMBOL(void)
#   define FIDGETY_VALIDATOR_DELETE_PROT() \
    void FIDGETY_VALIDATOR_DELETE_SYMBOL(Fidgety::Validator *FIDGETY_DELETE_PARAM_NAME)

#   define FIDGETY_VCC_ALLOC_PROT() Fidgety::ValidatorContextCreator *FIDGETY_VCC_ALLOC_SYMBOL(void)
#   define FIDGETY_VCC_DELETE_PROT() \
    void FIDGETY_VCC_DELETE_SYMBOL(Fidgety::ValidatorContextCreator *FIDGETY_DELETE_PARAM_NAME)

#   define FIDGETY_ALLOC(prot, cls, ...) prot { \
    return new cls(__VA_ARGS__); \
}
#   define FIDGETY_DELETE(prot) prot { \
    delete FIDGETY_DELETE_PARAM_NAME; \
}

namespace Fidgety {
    constexpr char DECODER_ALLOC_SYMBOL[] = "fidgetyDecoderAlloc";
    constexpr char DECODER_DELETE_SYMBOL[] = "fidgetyDecoderDelete";

    constexpr char ENCODER_ALLOC_SYMBOL[] = "fidgetyEncoderAlloc";
    constexpr char ENCODER_DELETE_SYMBOL[] = "fidgetyEncoderDelete";

    constexpr char VALIDATOR_ALLOC_SYMBOL[] = "fidgetyValidatorAlloc";
    constexpr char VALIDATOR_DELETE_SYMBOL[] = "fidgetyValidatorDelete";

    constexpr char VCC_ALLOC_SYMBOL[] = "fidgetyVccAlloc";
    constexpr char VCC_DELETE_SYMBOL[] = "fidgetyVccDelete";
}

#endif
