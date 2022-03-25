/**
 * @file tests/selector/selector_vcc.cpp
 * @author RenoirTan
 * @brief Implementation of the validator context creator for the selector test.
 * @version 0.1
 * @date 2022-03-25
 * 
 * @copyright Copyright (c) 2022
 */

#include <fidgety/_utils.hpp>
#include <fidgety/extensions.hpp>
#include <fidgety/options.hpp>
#include <fidgety/verifier.hpp>

using namespace Fidgety;

class Exp2ValidatorContextCreator : public ValidatorContextCreator {
    ValidatorContext createContext(
        const VerifierManagedOptionList &verifier,
        const OptionIdentifier &identifier
    ) {
        ValidatorContextInner vci;
        for (const auto &idoptPair : verifier) {
            if (identifier == idoptPair.first) {
                continue;
            }
            vci.emplace(idoptPair);
        }
        return ValidatorContext(std::move(vci));
    }
};

#ifdef __cplusplus

extern "C" {
    FIDGETY_ALLOC(FIDGETY_VCC_ALLOC_PROT(), Exp2ValidatorContextCreator);
    FIDGETY_DELETE(FIDGETY_VCC_DELETE_PROT());
}

#endif
