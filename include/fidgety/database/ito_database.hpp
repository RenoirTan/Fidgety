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

#   include <string>
#   include <vector>
#   include <fidgety/database.hpp>

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
}

#endif
