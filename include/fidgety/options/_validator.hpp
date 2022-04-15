/**
 * @file include/fidgety/options/_validator.hpp
 * @author RenoirTan
 * @brief Declaration of Fidgety::Validator
 * @version 0.1
 * @date 2022-04-14
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef _FIDGETY_OPTIONS_VALIDATOR_HPP
#   define _FIDGETY_OPTIONS_VALIDATOR_HPP

#   include "_fwd.hpp"

namespace Fidgety {
    class Validator {
    public:
        Validator(void);
        
        virtual ValidatorMessage validate(
            const Option &option,
            const ValidatorContext &context
        );

        virtual Validator *clone(void) const;

    protected:
    };
}

#endif
