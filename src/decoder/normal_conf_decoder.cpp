#include <nlohmann/json.hpp>
#include "spdlog/spdlog.h"
#include <fidgety/decoder.hpp>

using namespace Fidgety;

// https://stackoverflow.com/a/217605
// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// empty or all spaces
static inline bool is_effectively_empty(const std::string &s) {
    return (
        s.empty() ||
        std::all_of(s.begin(), s.end(), [](char c) { return std::isspace(c); })
    );
}

DecoderStatus NormalConfDecoder::dumpToIntermediate(void) {
    spdlog::trace("dumping NormalConfDecoder::mConfFile to NormalConfDecoder::mIntermediateFile");
    if (!isConfOpened() || !isIntermediateOpened()) {
        spdlog::error(
            "NormalConfDecoder::mConfFile and NormalConfDecoder::mIntermediateFile not open ({0})",
            (((uint8_t) isConfOpened()) << 1) | ((uint8_t) isIntermediateOpened())
        );
        return DecoderStatus::FilesNotOpen;
    }
    spdlog::trace("NormalConfDecoder::mConfFile and NormalConfDecoder::mIntermediateFile opened");

    // PARSING PART

    nlohmann::json intermediate;
    size_t line_no = 0;
    while (mConfFile.good()) {
        std::string line;
        std::getline(mConfFile, line);
        ++line_no;
        
        size_t pound_index = line.find('#');
        std::string no_comment;
        if (pound_index == std::string::npos) {
            no_comment = line;
        } else {
            no_comment = line.substr(0, pound_index);
        }
        if (is_effectively_empty(no_comment)) {
            continue;
        }

        size_t equals_index = no_comment.find('=');
        if (equals_index == std::string::npos) {
            spdlog::error("could not find '=' at line {0}", line_no);
        }
        std::string key = no_comment.substr(0, equals_index);
        std::string value = no_comment.substr(equals_index + 1);
        trim(key); trim(value);
        
        if (intermediate.contains(key)) {
            spdlog::warn(
                "{0} already set. However, the config file has another definition for {0} at {1}",
                key,
                line_no
            );
            spdlog::warn("overriding previous value of {0}", key);
        }
        intermediate[key] = value;
    }

    mIntermediateFile << intermediate;
    spdlog::debug(
        "successfully dumped NormalConfDecoder::mConfFile "
        "to NormalConfDecoder::mIntermediateFile"
    );
    return DecoderStatus::Ok;
}
