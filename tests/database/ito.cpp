/**
 * @file tests/database/ito.cpp
 * @author RenoirTan
 * @brief Tests for Ito.
 * @version 0.1
 * @date 2022-03-19
 * 
 * @copyright Copyright (c) 2022
 */

#include <fidgety/config.h>

#include <fstream>
#include <string>
#include <vector>
#include <fidgety/_tests.hpp>
#include <fidgety/database/ito_database.hpp>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

using namespace Fidgety;

TEST(DatabaseIto, GetCandidateIto) {
    _FIDGETY_INIT_TEST();

    std::vector<std::string> appNames = {"ito_0"};
    std::vector<std::string> allowedFileExts = {".json"};
    std::vector<std::string> searchPaths = {"../../../resources/tests/database"};
    std::vector<std::string> itoCandidates = getCandidateIto(
        appNames,
        allowedFileExts,
        searchPaths,
        true
    );

    ASSERT_FALSE(itoCandidates.empty());
    const std::string &candidate = itoCandidates[0];
    std::ifstream itoFile;
    itoFile.open(candidate);
    ASSERT_TRUE(itoFile.good());
    nlohmann::json intermediate;
}
