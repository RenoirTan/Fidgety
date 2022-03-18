/**
 * @file src/database/ito_database.cpp
 * @author RenoirTan
 * @brief Implentation of the Intermediate-to-Objects utilities.
 * @version 0.1
 * @date 2022-03-18
 * 
 * @copyright Copyright (c) 2022
 */

// Temporarily pacify VS Code's shoddy intellisense because it can't read
// .vscode/c_cpp_properties.json
#define SPDLOG_FMT_EXTERNAL

#include <algorithm>
#include <iterator>
#include <boost/filesystem.hpp>
#include <spdlog/spdlog.h>
#include <fidgety/database/ito_database.hpp>
#include <fidgety/_utils.hpp>

using namespace Fidgety;
namespace BoostFs = boost::filesystem;

std::vector<std::string> Fidgety::getCandidateIto(
    const std::vector<std::string> &appNames,
    const std::vector<std::string> &allowedFileExts,
    const std::vector<std::string> &searchPaths,
    bool selectFirst
) {
    spdlog::trace("[Fidgety::getCandidateIto] selectFirst = {0}", selectFirst);

    // Validation
    spdlog::trace("[Fidgety::getCandidateIto] validating allowedFileExts");
    for (const auto &fe : allowedFileExts) {
        auto it = std::find_if(
            std::begin(ALLOWED_ITO_FILEEXTS),
            std::end(ALLOWED_ITO_FILEEXTS),
            [fe](const char *const available) {
                return fe == available;
            }
        );
        if (it == std::end(ALLOWED_ITO_FILEEXTS)) {
            FIDGETY_CRITICAL(
                DatabaseException,
                DatabaseStatus::InvalidData,
                "[Fidgety::getCandidateIto] invalid file extension detected: {0}",
                fe
            );
        }
    }

    std::vector<std::string> candidates;
    for (const auto &strSpath : searchPaths) {
        BoostFs::path searchPath(strSpath);
        if (!BoostFs::exists(searchPath)) {
            spdlog::trace("[Fidgety::getCandidateIto] '{0}' does not exist", searchPath.string());
            continue;
        }
        if (!BoostFs::is_directory(searchPath)) {
            spdlog::trace(
                "[Fidgety::getCandidateIto] '{0}' is not a directory",
                searchPath.string()
            );
            continue;
        }

        for (auto deChild : BoostFs::directory_iterator(searchPath)) {
            BoostFs::path child(deChild);
            std::string sChild = child.string();

            if (!BoostFs::is_regular_file(child)) {
                spdlog::trace("[Fidgety::getCandidateIto] '{0}' is not a file", sChild);
                continue;
            }
            if (std::find(appNames.cbegin(), appNames.cend(), child) == appNames.cend()) {
                spdlog::trace(
                    "[Fidgety::getCandidateIto] '{0}' is not part of appNames",
                    sChild
                );
                continue;
            }
            if (
                std::find(appNames.cbegin(), appNames.cend(), BoostFs::extension(child))
                == appNames.cend()
            ) {
                spdlog::trace(
                    "[Fidgety::getCandidateIto] '{0}' does not have accepted file extensions",
                    sChild
                );
            }

            spdlog::trace("[Fidgety::getCandidateIto] candidate found: '{0}'", sChild);
            candidates.emplace_back(sChild);
            if (selectFirst) {
                spdlog::trace(
                    "[Fidgety::getCandidateIto] returning early because selectFirst is true"
                );
                return candidates;
            }
        }
    }

    return candidates;
}
