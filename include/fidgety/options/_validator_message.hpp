/**
 * @file include/fidgety/options/_validator_message.hpp
 * @author RenoirTan
 * @brief Header file for validator messages.
 * @version 0.1
 * @date 2022-04-14
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef _FIDGETY_OPTIONS_VALIDATOR_MESSAGE_HPP
#   define _FIDGETY_OPTIONS_VALIDATOR_MESSAGE_HPP

#   include "_fwd.hpp"

namespace Fidgety {
    enum class ValidatorMessageType : int32_t {
        Valid = 0,
        Problematic = 1,
        Invalid = 2,
        Unexpected = 3
    };

    class ValidatorMessage {
        public:
            ValidatorMessage(
                ValidatorMessageType messageType = ValidatorMessageType::Valid,
                std::string &&message = ""
            );
            ~ValidatorMessage(void);

            const std::string &getMessage(void) const noexcept;
            ValidatorMessageType getMessageType(void) const noexcept;
            std::string fullMessage(void) const;

            friend std::ostream &operator<<(std::ostream &stream, const ValidatorMessage &message);
        
        protected:
            std::string mMessage;
            ValidatorMessageType mType;
    };
}

#endif
