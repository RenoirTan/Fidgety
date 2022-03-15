/**
 * @file include/fidgety/appdata.hpp
 * @author RenoirTan
 * @brief Header file for app data. In this case, app data refers to the data
 * Fidgety collects when trying selecting a config file. This data contains
 * information like the path to the config file, the app the file belongs to
 * and the search paths for the relevant validators, decoders and encoders.
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef FIDGETY_APPDATA_HPP
#   define FIDGETY_APPDATA_HPP

#   include <string>
#   include <vector>

namespace Fidgety {
    struct Appdata {
        std::string configFilePath;
        std::string appName;
        std::vector<std::string> searchPaths;
    };
}

#endif
