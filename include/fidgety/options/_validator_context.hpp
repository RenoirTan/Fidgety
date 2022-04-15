/**
 * @file include/fidgety/options/_validator_context.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-04-14
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef _FIDGETY_OPTIONS_VALIDATOR_CONTEXT_HPP
#   define _FIDGETY_OPTIONS_VALIDATOR_CONTEXT_HPP

#   include "_fwd.hpp"

namespace Fidgety {
    class ValidatorContext {
        public:
            ValidatorContext(void);
            ValidatorContext(ValidatorContextInner &&map);

            bool optionExists(const OptionIdentifier &identifier) const noexcept;
            const Option &getOption(const OptionIdentifier &identifier) const;

            const ValidatorContextInner &getInnerMap(void) const noexcept;
        
        protected:
            ValidatorContextInner mMap;
    };
}

#endif
