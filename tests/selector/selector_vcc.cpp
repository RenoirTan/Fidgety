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
