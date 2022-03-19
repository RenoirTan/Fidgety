/**
 * @file include/fidgety/database/ito_database.hpp
 * @author RenoirTan
 * @brief Header file for Intermediate-to-Objects utilities.
 * @version 0.1
 * @date 2022-03-18
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef FIDGETY_DATABASE_ITO_DATABASE_HPP
#   define FIDGETY_DATABASE_ITO_DATABASE_HPP

#   include <memory>
#   include <string>
#   include <vector>
#   include <nlohmann/json.hpp>
#   include <fidgety/database.hpp>
#   include <fidgety/options.hpp>
#   include <fidgety/verifier.hpp>

namespace Fidgety {
    const char *const ALLOWED_ITO_FILEEXTS[] = {
        ".json"
    }; // more to come

    std::vector<std::string> getCandidateIto(
        const std::vector<std::string> &appNames,
        const std::vector<std::string> &allowedFileExts,
        const std::vector<std::string> &searchPaths,
        bool selectFirst = true
    );

    class Ito {
        public:
            Ito(void) = default;
            Ito(const Ito &ito) = default;
            Ito(Ito &&ito) = default;
            Ito &operator=(const Ito &ito) = default;
            Ito &operator=(Ito &&ito) = default;
            ~Ito(void) = default;

            virtual VerifierManagedOptionList toVmol(
                const nlohmann::json &intermediate,
                const std::unique_ptr<Validator> &validator
            ) = 0;
    };

    class ItoJson : public Ito {
        public:
            using Ito::Ito;
            using Ito::operator=;

            ItoJson(const nlohmann::json &desc);
            ItoJson(nlohmann::json &&desc);

            VerifierManagedOptionList toVmol(
                const nlohmann::json &intermediate,
                const std::unique_ptr<Validator> &validator
            );
        
        protected:
            nlohmann::json mDesc;
    };
}

#endif
