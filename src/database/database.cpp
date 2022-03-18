/**
 * @file src/database/database.cpp
 * @author RenoirTan
 * @brief Implementation of the database module of Fidgety.
 * @version 0.1
 * @date 2022-03-18
 * 
 * @copyright Copyright (c) 2022
 */

#include <fidgety/database.hpp>

using namespace Fidgety;

std::string DatabaseException::codeAsErrorType(void) const {
    switch (mCode) {
        case 0: return "Ok";
        case 1: return "FileNotFound";
        case 2: return "CannotReadFile";
        case 3: return "CannotWriteFile";
        case 4: return "CannotCloseFile";
        case 5: return "ResourceBusy";
        case 6: return "CannotOpenMultipleFiles";
        case 7: return "FilesNotOpen";
        case 8: return "SyntaxError";
        case 9: return "InvalidData";
        case 10: return "MultipleCandidates";
        case 11: return "NoCandidates";
        default: return "Other";
    }
}

const char *DatabaseException::getSimpleWhat(void) const noexcept {
    return "A Fidgety::DatabaseException occurred.";
}
